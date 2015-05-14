<?php
/**
 * @file
 * Admin page callbacks for the UP module.
 */

/**
 * UP settings form.
 */
function up_admin_form($form, &$form_state) {

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication Settings'),
    '#description' => t('To enable access for UP, you must <a href="@url">register your application</a> with Jawbone and add the provided keys here.', array('@url' => 'https://jawbone.com/up/developer/account')),
    '#collapsible' => TRUE,
    '#collapsed' => up_api_keys(),
  );
  $form['auth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Redirect URL'),
    '#markup' => url('up/auth', array('absolute' => TRUE)),
  );
  $form['auth']['up_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('up_client_id', NULL),
  );
  $form['auth']['up_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App secret'),
    '#default_value' => variable_get('up_app_secret', NULL),
  );

  $form['up'] = array(
    '#type' => 'fieldset',
    '#title' => t('Activities'),
    '#description' => t('Choose which activity types to import.'),
    '#collapsible' => TRUE,
  );
  $form['up']['up_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import UP ticks of site users who have added their bands.'),
    '#default_value' => variable_get('up_import', 1),
  );

  $options = array();
  foreach (up_summary_types() as $type => $info) {
    $options[$type] = $info['title'];
  }

  $form['up']['up_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Import data for these types of activity.'),
    '#options' => $options,
    '#default_value' => variable_get('up_types', array_keys(up_summary_types())),
    '#description' => t('Data will only be imported for the activity types you check here. Note that the module only requests permission for activity types that are enabled at the time a user adds a band. If you add additional types, a user may need to obtain a new authentication token from Jawbone by removing and re-adding their band.'),
    '#states' => array(
      'invisible' => array(
        ':input[name="up_import"]' => array('checked' => FALSE),
      ),
    ),
  );


  return system_settings_form($form);
}
