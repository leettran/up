<?php
/**
 * @file
 * Admin page callbacks for the UP module.
 */

/**
 * UP settings form.
 */
function up_admin_form($form, &$form_state) {
  $form['up_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import UP ticks of site users who have added their bands.'),
    '#default_value' => variable_get('up_import', 1),
  );

  $form['auth'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authentication Settings'),
    '#description' => t('To enable access for UP, you must <a href="@url">register your application</a> with Jawbone and add the provided keys here.', array('@url' => 'https://jawbone.com/up/developer/account/create')),
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

  return system_settings_form($form);
}
