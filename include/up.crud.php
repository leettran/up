<?php
/**
 * @file
 * CRUD functions for UP tokens and bands.
 */

/**
 * Fetch band data for a given token.
 *
 * @param $token.
 *   A token.
 *
 * @return
 *   A band object.
 */
function up_band_create($token, $bid = NULL) {

  $config = up_api_config($token);
  $up = new \Jawbone\Up($config);

  try {
    $json = $up->get(NULL);
  } catch (Exception $e) {
    watchdog('up', 'Jawbone UP API error: @error', array('@error' => (string)$e), WATCHDOG_ERROR);
    drupal_set_message(t('@error', array('@error' => (string)$e)), 'error');
  }

  $band = new stdClass();

  $band->bid        = $bid;
  $band->xid        = $json['data']['xid'];
  $band->uid        = $token->uid;
  $band->first_name = $json['data']['first'];
  $band->last_name  = $json['data']['last'];
  $band->image_url  = $json['data']['image'];

  return $band;
}

/**
 * Save an up band to the database.
 *
 * @param $data
 *   An object containing a JSON response from the API.
 * @param $account
 *   A valid user account.
 *
 * @return
 *   The status of the drupal_write_record() call.
 */
function up_band_save(&$band) {
  return drupal_write_record('up_band', $band, (!empty($band->bid)) ? 'bid' : array());
}

/**
 * Delete an up band from the database.
 */
function up_band_delete($band) {
  db_delete('up_band')->condition('bid', $band->bid)->execute();
}

/**
 * Retrieve band data from the database.
 */
function up_band_load($bid) {
  return db_select('up_band', 'b')->fields('b')->condition('bid', $bid)->execute()->fetchObject();
}

/**
 * Retrieve band info from the database.
 */
function up_band_load_by_xid($xid) {
  return db_select('up_band', 'b')->fields('b')->condition('xid', $xid)->execute()->fetchObject();
}

/**
 * Retrieve band info from the database.
 */
function up_band_load_by_user($account) {
  return db_select('up_band', 'b')->fields('b')->condition('uid', $account->uid)->execute()->fetchObject();
}

/**
 * Retrieve band info from the database.
 */
function up_band_load_all() {
  return db_select('up_band', 'b')->fields('b')->execute()->fetchAll();
}

/**
 * Turn a JSON blob into a token object.
 */
function up_token_create($json, $account) {
  $token = new stdClass();

  $token->tid           = 0;
  $token->uid           = $account->uid;
  $token->token         = $json->access_token;
  $token->refresh_token = $json->refresh_token;
  $token->expires       = time() + $json->expires_in;

  return $token;
}

/**
 * Save a band token to the database.
 */
function up_token_save(&$token, $update = TRUE) {
  return drupal_write_record('up_token', $token, (!empty($update)) ? 'tid' : array());
}

/**
 * Delete an up band token from the database.
 */
function up_token_delete($token) {
  db_delete('up_token')->condition('tid', $token->tid)->execute();
}

/**
 * Retrieve a band authentication token from the database.
 */
function up_token_load($tid) {
  return db_select('up_token', 't')->fields('t')->condition('tid', $tid)->execute()->fetchObject();
}

/**
 * Retrieve a band authentication token from the database.
 */
function up_token_load_by_user($account) {
  return db_select('up_token', 't')->fields('t')->condition('uid', $account->uid)->execute()->fetchObject();
}

/**
 * Turn an API data blob into a summary object.
 *
 * @param $item
 * @param $type
 * @param $band
 *
 * @return
 */
function up_summary_create($item, $type, $band) {
  $summary = new stdClass();

  $summary->xid       = $item['xid'];
  $summary->bid       = $band->bid;
  $summary->updated   = $item['time_updated'];
  $summary->created   = $item['time_created'];
  $summary->completed = $item['time_completed'];
  $summary->type      = (!empty($item['type'])) ? $item['type'] : _up_summary_type($type);
  $summary->sub_type  = (!empty($item['sub_type'])) ? $item['sub_type'] : 0;
  $summary->title     = $item['title'];
  $summary->snapshot  = (!empty($item['snapshot_image'])) ? $item['snapshot_image'] : '';
  $summary->image     = (!empty($item['image'])) ? $item['image'] : '';

  return $summary;
}

/**
 * Save a summary to the database.
 */
function up_summary_save(&$summary, $update = TRUE) {
  $ret = NULL;

  try {
    $ret = drupal_write_record('up_summary', $summary, (!empty($update)) ? 'xid' : array());
  } catch (Exception $e) {
    if ($e->getCode() == 23000) {
      // Duplicate entry, rerun as UPDATE query.
      $ret = drupal_write_record('up_summary', $summary, 'xid');
    }
  }
  return $ret;
}

/**
 * Delete a summary from the database.
 *
 * @param Var $summary
 *   A summary object or a summary xid string.
 */
function up_summary_delete($summary) {
  $xid = (is_object($summary)) ? $summary->xid : $summary;
  db_delete('up_summary')->condition('xid', $xid)->execute();
}

/**
 * Delete all summaries for a specific band from the database.
 *
 * @param Var $band
 *   A band object or a band bid id.
 */
function up_summary_delete_by_band($band) {
  $bid = (is_object($band)) ? $band->bid : $band;
  db_delete('up_summary')->condition('bid', $bid)->execute();
}

/**
 * Delete all summaries for a specific user from the database.
 *
 * @param Object $account
 *   A user account object.
 */
function up_summary_delete_by_user($account) {
  $band = up_band_load_by_user($account);
  db_delete('up_summary')->condition('bid', $band->bid)->execute();
}
