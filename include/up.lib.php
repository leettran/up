<?php
/**
 * @file
 * Provides API integration with the Jawbone UP.
 */

define('UP_API_HOST',  'https://jawbone.com');
define('UP_AUTH_URI',  'https://jawbone.com/auth/oauth2/auth');
define('UP_TOKEN_URI', 'https://jawbone.com/auth/oauth2/token');

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
      'scope'         => 'basic_read extended_read mood_read move_read sleep_read',
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
   * Parse a JSON response.
   */
  public function decode_response($response) {
    // Acceptable responses are in the 2xx range, so make an error out of everything else.
    if ($response->code < 200 || $response->code >= 300) {
      watchdog('up', 'A Jawbone UP API error occurred: @error', array('@error' => $response->error), WATCHDOG_ERROR);
      drupal_set_message(t('A Jawbone UP API error occurred: %error', array('%error' => $response->error)), 'error');
      return NULL;
    }

    // Thus an OK response. Whee!
    return json_decode($response->data);
  }
}
