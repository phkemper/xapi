<?php

/**
 * @file
 * Administration page callbacks and forms.
 */

/**
 * General configuration form for controlling the XAPI behaviour.
 */
function xapi_admin_settings($form, $form_state) {
  // Get all possible verbs from ADL
  $verbs = _xapi_admin_getverbs();
  $options = _xapi_admin_getoptions();
  $viewmodes = array(
    0 => t('Register a view every time someone visits the page'),
    1 => t('Always show a button and register a view when visitors clicked it'),
    2 => t('Show a button once and register a view when visitors clicked it'),
  );
  
  $form['xapi_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#description' => t('The SCORM Cloud application ID. You can find it in the Apps section under Applications, Show App ID on <a href="https://cloud.scorm.com/sc/user/Apps" target="_blank">https://cloud.scorm.com/sc/user/Apps</a>.'),
    '#default_value' => variable_get('xapi_app_id', ''),
  );
  $form['xapi_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App secret'),
    '#description' => t('The SCORM Cloud application secret key.'),
    '#default_value' => variable_get('xapi_app_secret', ''),
  );
  $form['explain'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Define which actions to register with XAPI verbs') . '</h2>',
  );
  $form['xapi_views_mode'] = array(
    '#type' => 'radios',
    '#title' => t('View registration'),
    '#default_value' => variable_get('xapi_views_mode', 0),
    '#options' => $viewmodes,
  );
  foreach ( $options as $action => $name ) {
    $form[$action] = array(
      '#type' => 'select',
      '#title' => $name,
      '#default_value' => variable_get($action, 0),
      '#options' => $verbs,
    );
  }
/*  $form['xapi_track'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Activities to track'),
    '#default_value' => variable_get('xapi_track', array_keys($options)),
    '#options' => $options,
    '#description' => t('Select which activities you like to track. If you select one of the \'Has read\' activities, this will show an \'I have read this\' button at the bottom of the content type.'),
  );*/

  return system_settings_form($form);
}

/**
 * Parse the ADL site for approved verbs.
 * 
 * Return an option array where the key and value are both the verb.
 * 
 * @return Array
 */
function _xapi_admin_getverbs() {
  $repo = 'http://adlnet.gov/expapi/verbs/';
  $verbs = array(
    0 => t('Do nothing'),
    'experienced' => 'experienced',
    'shared' => 'shared',
    'commented' => 'commented',
  );
  $content = file_get_contents($repo);
  if ( !$content ) {
    // No repository. Just return basic verbs
    return $verbs;
  }
  // Parse the repo data
  if ( preg_match_all('~href="(http://adlnet.gov/expapi/verbs/\w+)">(.+)</a>~misU', $content, $matches) ) {
    return array(0 => t('Do nothing')) + array_combine($matches[2], $matches[2]);
  }
  return $verbs;
}

/**
 * Return a list of potential options.
 * 
 * Include comment options and options for all content types.
 * 
 * @return Array
 */
function _xapi_admin_getoptions() {
  $types = array('comment' => t('Comments'));
  foreach ( node_type_get_types() as $type => $info ) {
    $types[$type] = $info->name;
  }
  // Create the array of actions
  $actions = array();
  foreach ( $types as $type => $name ) {
    $actions['xapi_' . $type . '_insert'] = $name . ' ' . t('created');
    $actions['xapi_' . $type . '_update'] = $name . ' ' . t('edited');
    $actions['xapi_' . $type . '_delete'] = $name . ' ' . t('deleted');
    $actions['xapi_' . $type . '_view'] = $name . ' ' . t('read');
  }
  return $actions;
}