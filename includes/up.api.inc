<?php
/**
 * @file
 * Provides API integration with the Jawbone UP.
 */

define ('UP_API_HOST', 'https://jawbone.com');
define ('UP_API_AUTH', UP_API_HOST . '/user/signin/login');
define ('UP_API_SIZE', 100);
define ('UP_API_TIMEOUT', 20);

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

  $response = drupal_http_request(UP_API_HOST . '/nudge/api/users/' . $band->xid . '/social', $options);
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

  $response = drupal_http_request(UP_API_HOST . '/nudge/api/users/' . $band->xid . '/healthCredits', $options);
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
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' => array('x-nudge-token' => $token->token),
  );

  $url = UP_API_HOST . '/nudge/api/users/' . $band->xid . '/band';

  $response = drupal_http_request($url, $options);
  return up_api_response($response);
}

/**
 * Fetch workouts, optionally specifying an API uri.
 */
function _up_api_workouts_uri($band, $epoch = 0, $uri = NULL) {
  $token = up_token_load($band->bid);

  // Create a start URI if we weren't passed one.
  if (empty($uri)) {
    $uri = '/nudge/api/users/' . $band->xid . '/workouts';
  }

  $data = array(
    'start_time' => $epoch,
    'end_time' => time(),
    'limit' => UP_API_SIZE,
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' => array('x-nudge-token' => $token->token),
  );

  $url = UP_API_HOST . $uri;

  $response = drupal_http_request($url, $options);
  return up_api_response($response);
}

/**
 * Wrapper that can recursively fetch all available workouts.
 */
function up_api_workouts($band, $epoch = 0) {
  $workouts = array();
  $next = NULL;
  $timeout = 0;

  do {
    $data = _up_api_workouts_uri($band, $epoch, $next);
    if (!empty($data->data->size)) {
      $workouts = array_merge($workouts, (array)$data->data->items);
    }
    if (!empty($data->data->links->next)) {
      $next = strtr($data->data->links->next, array('&limit=10' => '&limit=' . UP_API_SIZE));
    }
    else {
      $next = NULL;
    }
    $timeout++;
  } while(!empty($next) && $timeout < UP_API_TIMEOUT);

  return $workouts;
}

/**
 * Fetch sleeps, optionally specifying an API uri.
 */
function _up_api_sleeps_uri($band, $epoch = 0, $uri = NULL) {
  $token = up_token_load($band->bid);

  // Create a start URI if we weren't passed one.
  if (empty($uri)) {
    $uri = '/nudge/api/users/' . $band->xid . '/sleeps';
  }

  $data = array(
    'start_time' => $epoch,
    'end_time' => time(),
    'limit' => UP_API_SIZE,
  );

  $options = array(
    'method'  => 'GET',
    'data' => drupal_http_build_query($data),
    'headers' => array('x-nudge-token' => $token->token),
  );

  $response = drupal_http_request(UP_API_HOST . $uri, $options);
  return up_api_response($response);
}

/**
 * Wrapper that can recursively fetch all available sleeps.
 */
function up_api_sleeps($band, $epoch = 0) {
  $sleeps = array();
  $next = NULL;
  $timeout = 0;

  do {
    $data = _up_api_sleeps_uri($band, $epoch, $next);
    if (!empty($data->data->size)) {
      $sleeps = array_merge($sleeps, (array)$data->data->items);
    }
    if (!empty($data->data->links->next)) {
      $next = strtr($data->data->links->next, array('&limit=10' => '&limit=' . UP_API_SIZE));
    }
    else {
      $next = NULL;
    }
    $timeout++;
  } while(!empty($next) && $timeout < UP_API_TIMEOUT);

  return $sleeps;
}
