<?php
/**
 * @file
 * Page callbacks for the UP module.
 */

/**
 * User settings form.
 */
function up_user_settings($account = NULL) {
  // Verify OAuth keys.
  if (!up_api_keys()) {
    $variables = array('@up-settings' => url('admin/config/services/up/settings'));
    $output = '<p>' . t('You need to authenticate at least one Jawbone UP band in order to use the UP API. Please fill out the Auth fields at <a href="@up-settings">Jawbone UP Settings</a> and then return here.', $variables) . '</p>';
  }

  $output['add_band'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add UP band'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if (user_access('add up bands')) {
    $output['add_band']['form'] = drupal_get_form('up_band_form');
  }

  return $output;
}

/**
 * Wrapper to call drupal_form_submit().
 */
function up_auth_callback() {
  if (isset($_GET['error']) || empty($_GET['code'])) {
    drupal_set_message(t('The connection to Jawbone failed. Please try again. "@error"', array('@error' => $_GET['error'])), 'error');
    global $user;
    if ($user->uid) {
      // User is logged in, was attempting to OAuth a Jawbone UP account.
      drupal_goto('admin/config/services/up');
    }
    else {
      // Anonymous user, redirect to front page.
      drupal_goto('<front>');
    }
  }
  $form_state['values']['code'] = $_GET['code'];
  drupal_form_submit('up_auth_callback_form', $form_state);
}

/**
 * Form builder function.
 */
function up_auth_callback_form($form, &$form_state) {
  $form['#post']['code'] = $_GET['code'];
  $form['code'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['code'],
  );
  return $form;
}

/**
 * Validate results from UP OAuth return request.
 */
function up_auth_callback_form_validate($form, &$form_state) {
  if (empty($form_state['values']['code'])) {
    form_set_error('code', 'Invalid Jawbone UP authentication code');
  }
}

/**
 * Handle a Jawbone UP OAuth return request and store the account creds
 * in the DB.
 */
function up_auth_callback_form_submit($form, &$form_state) {
  $key = variable_get('up_client_id', '');
  $secret = variable_get('up_app_secret', '');
  $code = $form_state['values']['code'];

  module_load_include('php', 'up', 'up.lib');

  try {
    $auth = new JawboneAuth($key, $secret);
    $response = drupal_http_request($auth->get_token_url($code));

    // Hurray, we have a response!
    $json = $auth->decode_response($response);
  } catch (Exception $e) {
    watchdog('up', 'Jawbone UP API: @error', array('@error' => (string)$e), WATCHDOG_ERROR);
    drupal_set_message(t('Jawbone UP authentication request failed'), 'error');
    drupal_goto('user');;
  }

  global $user;

  // Create an entry in the token table.
  $token = up_token_create($json, $user);
  up_token_save($token, FALSE);

  // Fetch band info.
  $band = up_band_create($token);
  up_band_save($band);

  // Import data for this band now.
  foreach (up_summary_types(NULL, NULL, TRUE) as $type) {
    up_fetch_summaries($band, $type['endpoint']);
  }

  // Redirect to the profile page. We can display band info there. Huzzah!
  drupal_goto('user');
}
