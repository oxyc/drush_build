<?php

/**
 * Scaffolds an array declaration to be used for aliases.
 *
 * @param $options
 *   Requires an 'alias' key which contains the name of the alias. This will
 *   then be removed and not contained within the array declaration. The
 *   rest of the elements will be iterated and scaffolded.
 *
 * @return
 *   The scaffolded array as a string.
 */
function drush_build_generate_alias_contents($options) {

  $name = $options['alias'];
  unset($options['alias']);

  $content = "\n\$aliases['$name'] = "
    . _drush_build_generate_alias_contents_value($options)
    . ";\n";

  return $content;
}

/**
 * Scaffolds the value passed in by recursing on arrays and outputing
 * either 'value' or 'key' => array('
 *
 * @param $value
 *   The value to scaffold into a value or key => value
 *
 * @return
 *   The scaffolded value as a string.
 */
function _drush_build_generate_alias_contents_value($value) {

  if (is_array($value)) {
    $content = "array(";

    foreach ($value as $k => $v) {
      // @todo figure out why one cant pass remote host as an option
      $k = ($k == "remote-hostname") ? "remote-host" : $k;
      $content .= "\n '$k' => " . drush_build_generate_alias_contents_value($v);
    }

    $content .= "\n)";

    return $content;
  }
  else {
    return "'$value',";
  }
}

/**
 * Get all context options for the running command.
 *
 * @param $prompt
 *   If TRUE the user will be prompted to input the options available to the
 *   command but not set.
 *
 * @return
 *   An array of all options merged.
 */
function _drush_build_options($prompt = FALSE) {
  if ($prompt) {
    $available_options = _drush_build_parse_arglist(drush_get_option('options', array(), 'command'));
    $options = drush_get_merged_options();

    foreach ($available_options as $option => $description) {
      if (!array_key_exists($option, $options)) {
        $stdin = drush_prompt($description);
        drush_set_default($option, trim($stdin));
      }
    }
  }
  $options = drush_get_merged_options();

  return $options;
}

/**
 * Parse the argument list available for a command and return them as
 * 'argument' => 'description'.
 *
 * @param $arglist
 *   An argument list.
 *
 * @return
 *  A clean associative array contining argument as key and description as
 *  value.
 */
function _drush_build_parse_arglist($arglist) {
  $exclude = array('version');
  $options = array();
  foreach($arglist as $arg => $value) {

    $arg = preg_replace('/^--([^=]+)/', '$1', $arg);
    if (!in_array($arg, $exclude)) {
      $options[$arg] = $value;
    }
  }
  return $options;
}
