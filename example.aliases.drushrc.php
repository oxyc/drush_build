<?php
/**
 * This is an example to abstract webbserver settings that keep
 * repeating themselves. To get drush access on a remote host
 * you'll have to add path-aliases. They are relative to where
 * ssh brings you on connection, remember these should NOT be in
 * your public_html directory.
 */
$aliases['cindy'] = array(
  'uri' => 'dev.example.com',
);

$aliases['webholder'] = array(
  'path-aliases' => array(
    '%drush' => 'drush/',
    '%drush-script' => 'drush/drush',
    '%dump-dir' => '.sqldumps',
    '%dump' => '.sqldumps/main_sync.sql',
  ),
);
