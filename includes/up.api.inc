<?php
/**
 * @file
 * Provides API integration with the Jawbone UP.
 */

define ('UP_API_HOST', 'https://jawbone.com');
define ('UP_API_AUTH', UP_API_HOST . '/user/signin/login');
define ('UP_API_URL',  UP_API_HOST . '/nudge');

/**
 * Parse an API response.
 *
 * @param $response.
 *  An HTTP response object returned by drupal_http_request().
 * @return
 *   NULL on error or the json_decoded response data on success.
 */
function up_api_response($response) {
  // dpm($response, __FUNCTION__);

  // Acceptable responses are in the 2xx range, so make an error out of everything else.
  if ($response->code < 200 || $response->code >= 300) {
    watchdog('up', 'A Jawbone UP API error occurred: @error', array('@error' => $response->error), WATCHDOG_ERROR);
    drupal_set_message(t('A Jawbone UP API error occurred: %error', array('%error' => $response->error)), 'error');
    return NULL;
  }

  // Thus an OK response. Whee!
  return json_decode($response->data);
}

/**
 * Authenticate to the Jawbone API.
 */
function up_api_authenticate($mail, $password) {
  // The request payload.
  $data = array(
    'email' => $mail,
    'pwd' => $password,
    'service' => 'nudge',
  );

  $options = array(
    'method' => 'POST',
    'data' => drupal_http_build_query($data),
  );
  $response = drupal_http_request(UP_API_AUTH, $options);
  return up_api_response($response);
}

/**
 * Fetch the activity summary from Jawbone.
 */
function up_api_summary($band) {
  $token = up_token_load($band->bid);

  $data = array(
    'after' => 'null',
    'limit' => 30,
    // '_token' => $token->token,
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' =>  array('X-Nudge-Token' => $token->token),
  );

  $response = drupal_http_request(UP_API_URL . '/api/users/' . $band->xid . '/social', $options);
  return up_api_response($response);
}

/**
 * Fetch today's data from Jawbone.
 */
function up_api_today($band) {
  $token = up_token_load($band->bid);

  $tz = new DateTimeZone(drupal_get_user_timezone());
  $offset = $tz->getOffset(new DateTime("now", $tz));

  $data = array(
    'date' => date('Y-m-d'),
    'timezone' => $offset,
    'move_goal' => 0,
    'sleep_goal' => 0,
    'eat_goal' => 0,
    'check_levels' => 1,
    // '_token' => $token->token,
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' => array('x-nudge-token' => $token->token),
  );

  $response = drupal_http_request(UP_API_URL . '/api/users/' . $band->xid . '/healthCredits', $options);
  return up_api_response($response);
}

/**
 * Fetch ticks.
 */
function up_api_band($band, $epoch = 0) {
  $token = up_token_load($band->bid);

  $data = array(
    'start_time' => $epoch,
    'end_time' => time(),
    '_token' => $token->token,
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' => array('x-nudge-token' => $token->token),
  );

  $url = UP_API_URL . '/api/users/' . $band->xid . '/band';
  dpm($url, 'url');
  dpm($options, 'options');

  $response = drupal_http_request($url, $options);
  return up_api_response($response);
}

