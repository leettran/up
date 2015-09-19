<?php
/**
 * @file
 * Provides API integration with the Jawbone UP.
 */

define('UP_API_HOST',  'https://jawbone.com');
define('UP_AUTH_URI',  'https://jawbone.com/auth/oauth2/auth');
define('UP_TOKEN_URI', 'https://jawbone.com/auth/oauth2/token');
define('UP_CACHE',     'public://up');

/**
 * Because why not?
 */
class JawboneAuthException extends Exception {}

/**
 * Basic class for JAwbone UP API Oauth authentication.
 */
class JawboneAuth {
  private $client_id  = null;
  private $app_secret = null;

  /**
   * Constructor for the JawboneAuth class.
   */
  public function __construct($client_id, $app_secret) {
    $this->client_id  = $client_id;
    $this->app_secret = $app_secret;
  }

  /**
   * Return the UP OAuth2 code URL.
   */
  public function get_code_url() {
    $params = array(
      'response_type' => 'code',
      'client_id'     => $this->client_id,
      'scope'         => $this->access_string(),
      'redirect_uri'  => url('up/auth', array('absolute' => TRUE))
    );
    return url(UP_AUTH_URI, array('query' => $params, 'absolute' => TRUE));
  }

  /**
   * Return the UP OAuth2 token URL.
   */
  public function get_token_url($code) {
    $params = array(
      'grant_type'    => 'authorization_code',
      'client_id'     => $this->client_id,
      'client_secret' => $this->app_secret,
      'code'          => $code,
    );
    return url(UP_TOKEN_URI, array('query' => $params, 'absolute' => TRUE));
  }

  /**
   * Return the UP OAuth2 token refresh URL.
   */
  public function get_token_refresh_url($refresh_token) {
    $params = array(
      'grant_type'    => 'refresh_token',
      'client_id'     => $this->client_id,
      'client_secret' => $this->app_secret,
      'refresh_token' => $refresh_token,
    );
    return url(UP_TOKEN_URI, array('query' => $params, 'absolute' => TRUE));
  }

  /**
   * Parse a JSON response.
   */
  public function decode_response($response) {
    // Acceptable responses are in the 2xx range, so make an error out of everything else.
    if ($response->code < 200 || $response->code >= 300) {
      watchdog('up', 'Jawbone UP API error: @error', array('@error' => $response->error), WATCHDOG_ERROR);
      drupal_set_message(t('A Jawbone UP API error occurred: %error', array('%error' => $response->error)), 'error');
      throw new JawboneAuthException($response->error);
    }

    // Thus an OK response. Whee!
    return json_decode($response->data);
  }

  /**
   * Create a permissions string to access enabled types.
   */
  private function access_string() {
    $permissions = array();
    $types = up_summary_types(NULL, NULL, TRUE);

    foreach ($types as $type) {
      $permissions[] = $type['permission'];
    }

    return implode(' ', $permissions);
  }
}

/**
 * Return a list of summary types and sub-types that we can deal with.
 *
 * @param String $type
 *   Return only info for this type.
 * @param Integer sub_type
 *   Returen only info for this sub_type.
 * @param Boolean $filter
 *   Return only info for enabled types.
 *
 * @return
 *   An multi-dimension array, or a string, or NULL.
 */
function up_summary_types($type = NULL, $sub_type = NULL, $filter = FALSE) {

  $types = array(
    'user' => array(
      'endpoint' => 'user',
      'permission' => 'basic_read',
      'title' => t('User'),
      'sub_types' => array(),
    ),
    'goal' => array(
      'endpoint' => 'goals',
      'permission' => '',
      'title' => t('Goals'),
      'sub_types' => array(),
    ),
    'mood' => array(
      'endpoint' => 'mood',
      'permission' => 'read_mood',
      'title' => t('Mood'),
      'sub_types' => array(
        1 => t('Amazing'),
        2 => t('Pumped UP'),
        3 => t('Energized'),
        4 => t('Meh'),
        5 => t('Dragging,'),
        6 => t('Exhausted'),
        7 => t('Totally Done'),
        8 => t('Good'),
      ),
    ),
    'move' => array(
      'endpoint' => 'moves',
      'permission' => 'move_read',
      'title' => t('Move'),
      'sub_types' => array(),
    ),
    'sleep' => array(
      'endpoint' => 'sleeps',
      'permission' => 'sleep_read',
      'title' => t('Sleep'),
      'sub_types' => array(
        0 => t('Normal'),
        1 => t('Power nap'),
        2 => t('Nap'),
      ),
    ),
  );

  // Return all info for all enabled types.
  if (!empty($filter)) {
    return array_intersect_key($types, array_filter(variable_get('up_types', array())));
  }

  // Return all info for all types.
  if (empty($type)) {
    return $types;
  }

  // You asked for what? 404 not found.
  if (empty($types[$type])) {
    return NULL;
  }

  // Return all info for the specified type.
  if ($sub_type == NULL) {
    return $types[$type];
  }

  // Return the name of the sub-type.
  return (!empty($types[$type]['sub_types'][$sub_type])) ? $types[$type]['sub_types'][$sub_type] : NULL;
}

/**
 * Helper to make the summary types consistent. Because the API sure doesn't.
 */
function _up_summary_type($type) {
  switch ($type) {
    case 'sleeps':
      return 'sleep';
  }

  return $type;
}
