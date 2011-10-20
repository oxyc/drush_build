<?php

include_once 'drush_build.utilities.inc';

/**
 * Implements drush_hook_init().
 */
function drush_drush_build_init() {
  // URL to drush source for downloading on remote host
  define('DRUSH_DOWNLOAD_URL', 'http://ftp.drupal.org/files/projects/drush-7.x-4.5.tar.gz');
}

/**
 * Implements hook_drush_command().
 */
function drush_build_drush_command() {
  $items['build'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Installs a Drupal site with specified configurations'),
    'arguments' => array(
      // @todo, implement
      'site name' => dt('The name of the site'),
    ),
    'examples' => array(
      'drush build' => dt('Installs the Drupal site within the current working directory'),
    ),
    'options' => array(
      '--version' => dt('Print the version and exit'),
      '--generate-alias' => dt('Generate a a local alias'),
      '--make-file' => dt('The path to the makefile'),
      '--make-flags' => dt('Specific flags to be used by drush make'),
      '--site-name' => dt('The name of the site'),
      '--site-uri' => dt('The uri of the site, will be used as folder name and alias'),
      '--site-lang' => dt('The language of the website, separeted by commas'),
      '--site-profile' => dt('The installation profile to use'),
      '--root' => dt('The root directory of the local installation'),
      '--db-protocol' => dt('The %location database protocol', array('%location' => 'local')),
      '--db-host' => dt('The hostname of the %location database', array('%location' => 'local')),
      '--db-database' => dt('The name of the %location database', array('%location' => 'local')),
      '--db-username' => dt('The username to access the %location database with', array('%location' => 'local')),
      '--db-password' => dt('The password to access the %location database', array('%location' => 'local')),
      '--host' => dt('The host url of the website on the %location machine', array('%location' => 'local')),
      '--account-name' => dt('The username of the Drupal UID1'),
      '--account-pass' => dt('The password of the Drupal UID1'),
      '--account-mail' => dt('The email of the Drupal UID1'),
    ),
  );
  $items['deploy'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_MAX,
    'description' => dt('Deploys a Drupal site to a live host'),
    'arguments' => array(
      // @todo, implement
      'host' => dt('The remote host address'),
    ),
    'examples' => array(
      'drush deploy' => dt('Deploys the Drupal site in the working directory to a remote host'),
    ),
    'options' => array(
      '--version' => dt('Print the version and exit'),
      '--root' => dt('The root directory of the remote installation'),
      '--host' => dt('The remote host address'),
      '--user' => dt('The username of the remote host'),
      '--db-protocol' => dt('The %location database protocol', array('%location' => 'remote')),
      '--db-host' => dt('The hostname of the %location database', array('%location' => 'remote')),
      '--db-database' => dt('The name of the %location database', array('%location' => 'remote')),
      '--db-username' => dt('The username to access the %location database with', array('%location' => 'remote')),
      '--db-password' => dt('The password to access the %location database', array('%location' => 'remote')),
      '--public-key' => dt('The public key to transfer to the remote host'),
    ),
  );
  $items['generate-alias'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => dt('Generates an alias file'),
    'arguments' => array(
      'name' => dt('Name of the alias group'),
      'location' => dt('Whether the alias points to a local or a remote location'),
    ),
    'examples' => array(
      'drush generate-alias' => dt('Generates an alias file'),
    ),
    'options' => array(
      '--version' => dt('Print the version and exit'),
      '--uri' => dt('The URI of the remote host'),
      '--db-protocol' => dt('The %location database protocol', array('%location' => 'alias')),
      '--db-host' => dt('The hostname of the %location database', array('%location' => 'alias')),
      '--db-database' => dt('The name of the %location database', array('%location' => 'alias')),
      '--db-username' => dt('The username to access the %location database with', array('%location' => 'alias')),
      '--db-password' => dt('The password to access the %location database', array('%location' => 'alias')),
      '--remote-hostname' => dt('The host address of the remote site'),
      '--remote-user' => dt('The username of the remote host'),
    ),
  );
  return $items;
}

/**
 * Implements of hook_drush_help().
 */
function drush_build_drush_help($section) {
  switch ($section) {
    case 'drush:build':
      return dt('Installs a Drupal site with specified configurations.');
    case 'drush:deploy':
      return dt('Deploys the Drupal site to a live host');
    case 'drush:generate-alias':
      return dt('Generates alias files.');
  }
}

/**
 * Implements drush_hook_command().
 *
 * Builds a Drupal site in the current working directory using drush make and
 * and drush site-install.
 */
function drush_drush_build_build($site_name = NULL) {

  // Read settings
  $settings = _drush_build_options(TRUE);
  $settings['install_path'] = $settings['root'] . '/' . $settings['site-uri'];
  $settings['local-url'] = $settings['host'] . '/' . $settings['site-uri'];

  // @todo, Look into: drush_convert_db_from_db_url()
  $settings['db-url'] = $settings['db-protocol']
    . '://' . $settings['db-username']
    . ':' . $settings['db-password']
    . '@' . $settings['db-host']
    . '/' . $settings['db-database'];

  $none = array('root' => NULL, 'uri' => NULL);

  if (is_dir($settings['install_path'])) {
    return drush_user_abort(dt('The installation path already exists.'));

    // @todo, chmod +w to settings file and and remove directory.
    $continue = drush_confirm(dt('The installation directory (%path) already exists, do you wish to remove it', array('%path' => $install_path));

    if ($continue) {
      drush_op('drush_shell_exec', 'rm -rf %s', $settings['install_path']);
    } else {
      return drush_user_abort(dt('The old installation was kept, exiting.'));
    }
  }

  // Drush make
  $result = drush_invoke_process($none, 'make', array(
    $settings['make-file'],
    $settings['install_path'],
  ), array(
    'translations' => $settings['site-lang'], 
  ));

  if (!drush_valid_drupal_root($settings['install_path'])) {
    return drush_set_error('DRUSH_BUILD_MAKE', dt('The make build failed.'));
  }
  else {
    drush_log(dt('Drush make completed successfully.'), 'success')
  }

  // Move into the newly created installation directory
  drush_op('chdir', $install_path);

  // Drush site-install
  $result = drush_invoke_process($none, 'site-install', array(
    $settings['site-profile'],
  ), array(
    'yes' => TRUE,
    'db-url' => $settings['db-url'],
    'locale' => $settings['site-lang'],
    'site-name' => $settings['site-name'],
    'account-name' => $settings['account-name'],
    'account-pass' => $settings['account-pass'],
    'account-mail' => $settings['account-mail'],
  ));

  if ($result === FALSE) {
    return drush_set_error('DRUSH_BUILD_SITEINSTALL', dt('The installation process failed.'));
  }
  else {
    drush_log(dt('Drupal installation completed successfully.'), 'success');
  }

  // Change file permissions
  drush_op('drush_shell_exec', 'chmod -R a+w %s', './sites/default/files');

  // Generate alias
  if ($settings['generate-alias']) {
    $result = drush_invoke_process($none, 'generate-alias', array(
      $settings['site-uri'],
      'local'
    ), array(
      'uri' => $settings['local-url'],
      'db-url' => $settings['db-url'],
    ));

    if ($result) {
      drush_log(dt('Alias was generated succesfully.'), 'success');
    }
    else {
      drush_log(dt('Alias could not be generated'));
    }
  }

  // Launch a login page in the browser
  if (!empty($settings['host'])) {
    $login_url = drush_invoke_process($none, 'user-login', array(
    ), array(
      'uri' => $local_url,
    ));

    if (is_array($login_url)) {
      drush_shell_exec('google-chrome %s', $login_url['output']);
    }
  }
}

/**
 * Implements drush_hook_command().
 *
 * Scaffolds an alias file based on configurations passed.
 */
function drush_drush_build_generate_alias($name = NULL, $location = 'local') {
  
  if (is_null($name)) {
    return drush_set_error('DRUSH_BUILD_ALIAS', dt('You have to specifiy a name for the alias file.'));
  }

  // @todo, remove this
  switch ($location) {
    case 'local':
      $context = "alias-$location";
      break;
    case 'remote':
      $context = "alias-$location";
      break;
    default:
      return drush_set_error('DRUSH_BUILD_ALIAS', dt('Your location has to be either local or remote'));
      break;
  }

  // Read settings
  $settings = _drush_build_options();

  // Merge in location specific configurations from drushrc file.
  if (array_key_exists($context, $settings)) {
    $cli = drush_get_context('cli');

    // Remove drush generated elements.
    unset($cli['php'], $cli['backend']);

    $settings = array_merge($settings[$context], $cli);
  }
  else {
    return drush_log(dt('No alias template could be found for %context, please define it in your drushrc file.', array('%context' => $context)));
  }

  $alias_file = $settings['directory'] . '/' . $name . '.aliases.drushrc.php';

  // Remove the directory element as it's not supposed be scaffoled.
  unset($settings['directory']);

  // Create the directory if it doesn't exist
  if (!is_dir(dirname($alias_file))) {
    drush_op('drush_shell_exec', 'mkdir %s', dirname($alias_file));
  }

  // Scaffold the content
  $content = drush_build_generate_alias_contents($settings);
  
  // Write the content
  if (!file_exists($alias_file) && file_put_contents($alias_file, "<?php\n\n$content")) {
    drush_log(dt('Wrote alias file %file', array('%file' => $alias_file)), 'success');
  }
  elseif (file_put_contents($alias_file, $content, FILE_APPEND)) {
    drush_log(dt('Wrote alias file %file', array('%file' => $alias_file)), 'success');
  }
  else {
    drush_log(dt('Writing alias file %file failed.', array('%file' => $alias_file)), 'warning');
  }
}


/**
 * Implements drush_hook_command().
 *
 * Deploy the drupal site to a remote host.
 */
function drush_drush_build_deploy() {
  
  $alias = drush_sitealias_get_record('@self');
  drush_set_default('site-uri', basename($alias['root']));

  $none = array('root' => NULL, 'uri' => NULL);

  // Read settings
  $settings = _drush_build_options(TRUE);

  // Send over the public key
  if (!empty($settings['public-key'])) {
    $result = drush_invoke_process($none, 'pushkey', array(
      $settings['user'] . '@' . $settings['host']
    ), array(
      'keyname' => $settings['public-key']
    ));

    if ($result === FALSE) {
      return drush_set_error('DRUSH_BUILD_PUSHKEY', dt('Could not push the public key.'));
    }
    else {
      drush_log(dt('Public key was pushed succesfully'), 'success');
    }
  }
  
  $db_url = $settings['db-protocol']
    . '://' . $settings['db-username']
    . ':' . $settings['db-password']
    . '@' . $settings['db-host']
    . '/' . $settings['db-database'];

  // Generate remote alias
  $result = drush_invoke_process(array('root' => NULL, 'uri' => NULL), 'generate-alias', array(
    $settings['site-uri'],
    'remote'
  ), array(
    'uri' => 'http://' . $settings['host'],
    'db-url' => $db_url,
    'remote-hostname' => $settings['host'],
    'remote-user' => $settings['user'],
  ));

  if ($result === FALSE) {
    return drush_set_error('DRUSH_BUILD_ALIAS', dt('Alias could not be generated'));
  }
  else {
    drush_log(dt('Alias was generated succesfully'), 'success');
  }

  // Download drush on remote host
  $ssh_login = $settings['user'] . '@' . $settings['host'];
  $url = DRUSH_DOWNLOAD_URL;
  $exec = "ssh $ssh_login /bin/sh -c \"'wget -O - $url | tar -xvzf - && chmod +x drush/drush && echo \\\"PATH=\\\$PATH:\\\$HOME/drush/\\\" >> .bashrc'\"";
  $result = drush_op_system($exec);
  if ($result !== FALSE) {
    drush_log(dt('Drush was downloaded on the remote server successfully.'), 'success');
  }

  // @todo, errors:
  // deploy doesnt generate alias!?
  // add --include-conf to rsync for the first time
  // find a way to change mysql settings on remote host
}

/**
 * Implements drush_hook_command_rollback().
 *
 * If an error occured during make or siteinstall, remove the files generated.
 */
function drush_drush_build_build_rollback() {
  $error_codes = array(
    'DRUSH_BUILD_MAKE',
    'DRUSH_BUILD_SITEINSTALL',
  );

  if (in_array(drush_get_error(), $error_codes)) {
    // @todo, fix.
    /*
    $settings = drush_get_context('build');
    $install_path = $settings['root'] . '/' . $settings['site-uri'];

    if (drush_confirm(dt('Do you wish to remove %path', array('%path' => $install_path)))) {
      drush_op('drush_shell_exec', 'rm -rf %s', $install_path);
    }*/
  }

  drush_op('chdir', drush_cwd());
}

/**
 * Implements drush_hook_command_validate().
 *
 * Make sure the Drush version is higher than 4.5.
 */
function drush_drush_build_build_validate() {
  $info = drush_read_drush_info();

  if (isset($info['drush_version']) && $info['drush_version'] < 4.5) {
    return drush_set_error('DRUSH_BUILD_CORE_VERSION', dt('Drush version 4.5 or higher is required for drush build to work.'));
  }
}

/**
 * Implements hook_version().
 */
function drush_build_version() {
  return '0.1';
}

