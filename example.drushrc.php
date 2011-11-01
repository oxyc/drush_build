<?php
/**
 * Example of settings required to make drush build usage an ease.
 * 
 * Only copy these settings into your regular drushrc file, do not use the
 * file itself.
 */

// If you wish to organize your aliases consider adding a new path where
// drush looks for them.
// Note: If you specify an alias directory path for drupal_build this path
// has to be added in this array.
$options['alias-path'] = '/path/to/home/.drush/aliases';


$path = '/path/to/home/.drush';

// Global settings to use for generate-alias.
// The local value will be used by drush build and the remote value by
// drush deploy. These values has to exist as arrays bound to the key.
// See next settings.
$command_specific['generate-alias'] = array(
  'directory' => $path . '/aliases',
  'local' => 'dev',
  'remote' => 'live',
);

// Alias specific settings that will be used to generate the alias file.
// Besides these one can pass settings through generate-alias argumentlist.
// If you're developing on a local computer and pushing to a production server
// which always has the same shared host it's worth creating parent aliases
// in an aliases.drushrc.php file and inherit settings.
$command_specific['generate-alias']['dev'] = array(
  'parent' => '@cindy',
);

$command_specific['generate-alias']['live'] = array(
  'parent' => '@webholder',
);

// Build specific settings that's repeated every build. Store these not
// to be prompted all the time while building.
$command_specific['build'] = array(
  'generate-alias' => TRUE,
  'make-file' => $path . '/drush_make/example.makefile.make',
  'make-flags' => 'prepare-install',
  'site-lang' => 'en',
  'site-profile' => 'minimal',
  'root' => '/path/to/drupal/installations',
  'db-driver' => 'mysql',
  'db-host' => 'localhost',
  'db-username' => 'USERNAME',
  'db-password' => 'PASSWORD',
  'host' => 'http://localhost/drupal/installations',
  'account-name' => 'admin',
  'account-pass' => 'admin',
  'account-mail' => 'admin@example.com',
  'create-repository' => TRUE,
  'launch-browser' => 'google-chrome',
  'enable-modules' => TRUE,
  'modules' => array(
    'menu', 'field_ui', 'taxonomy', 'contextual', 'dashboard', 'shortcut', 'help', 'overlay', 'path', 'file', 'image', 'list', 'number',
  ),
);

// Deploy specific settings that keeps repeating. Most of the values have
// to be prompted though.
$command_specific['deploy'] = array(
  'root' => 'public_html',
  'db-driver' => 'mysql',
  'db-host' => 'localhost',
  'public-key' => 'id_rsa',
);

// Create a bitbucket git repository.
// API for this will change.
$command_specific['create-repository'] = array(
  'method' => 'POST',
  'username' => 'USERNAME',
  'password' => 'PASSWORD',
  'url' => 'https://api.bitbucket.org/1.0/repositories/',
  'private' => TRUE,
);
