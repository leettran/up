<?php
/**
 * @file
 * Views integration for up.
 */

/**
 * Implements hook_views_data().
 */
function up_views_data() {
  $data['up_band'] = array(
    'table' => array(
      'group' => t('UP Band'),
      'base'  => array(
        'field' => 'bid',
        'title' => t('Jawbone UP Bands'),
        'help' => t('Jawbone UP bands associated with user accounts.'),
      ),
      'join' => array(
        'users' => array(
          'left_field' => 'uid',
          'field' => 'uid',
        ),
      ),
    ),
    'bid' => array(
      'title' => t('BID'),
      'help' => t('The unique band id.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'xid' => array(
      'title' => t('XID'),
      'help' => t('The unique Jawbone API identifier for the band.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'uid' => array(
      'title' => t('Uid'),
      'help' => t('The user who added the band. If you need more fields than the uid add the band: user relationship'),
      'relationship' => array(
        'title' => t('User'),
        'help' => t('Relate a band to the user who created it.'),
        'handler' => 'views_handler_relationship',
        'base' => 'users',
        'field' => 'uid',
        'label' => t('User'),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_user_name',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'field' => array(
        'handler' => 'views_handler_field_user',
      ),
    ),
    'first_name' => array(
      'title' => t('First name'),
      'help' => t('The first name for the band.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'last_name' => array(
      'title' => t('Last name'),
      'help' => t('The last name for the band.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'image_url' => array(
      'title' => t('Image'),
      'help' => t('The profile image for the band.'),
      'field' => array(
        'handler' => 'up_views_handler_field_image',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  $data['up_summary'] = array(
    'table' => array(
      'group' => t('UP Summary'),
      'base'  => array(
        'field' => 'xid',
        'title' => t('Jawbone UP Summaries'),
        'help' => t('Jawbone UP activity summaries.'),
      ),
      'join' => array(
        'up_bands' => array(
          'left_field' => 'bid',
          'field' => 'bid',
        ),
      ),
    ),
    'xid' => array(
      'title' => t('XID'),
      'help' => t('The unique Jawbone API identifier for the summary.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'bid' => array(
      'title' => t('BID'),
      'help' => t('The unique band id.'),
      'relationship' => array(
        'title' => t('Band'),
        'help' => t('Relate a summary to the band that created it.'),
        'handler' => 'views_handler_relationship',
        'base' => 'up_band',
        'field' => 'bid',
        'label' => t('Band'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'title' => array(
      'title' => t('Title'),
      'help' => t('The summary title.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'type' => array(
      'title' => t('Type'),
      'help' => t('The type of summary.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'updated' => array(
      'title' => t('Updated date'),
      'help' => t('The date the summary was updated.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'updated_fulldate' => array(
      'title' => t('Update date'),
      'help' => t('Date in the form of CCYYMMDD.'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_fulldate',
      ),
    ),
    'updated_year_month' => array(
      'title' => t('Updated year + month'),
      'help' => t('Date in the form of YYYYMM.'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_year_month',
      ),
    ),
    'updated_year' => array(
      'title' => t('Updated year'),
      'help' => t('Date in the form of YYYY.'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_year',
      ),
    ),
    'updated_month' => array(
      'title' => t('Updated month'),
      'help' => t('Date in the form of MM (01 - 12).'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_month',
      ),
    ),
    'updated_day' => array(
      'title' => t('Updated day'),
      'help' => t('Date in the form of DD (01 - 31).'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_day',
      ),
    ),
    'updated_week' => array(
      'title' => t('Updated week'),
      'help' => t('Date in the form of WW (01 - 53).'),
      'argument' => array(
        'field' => 'updated',
        'handler' => 'views_handler_argument_node_created_week',
      ),
    ),
    'created' => array(
      'title' => t('Created date'),
      'help' => t('The date the summary was created.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'created_fulldate' => array(
      'title' => t('Created date'),
      'help' => t('Date in the form of CCYYMMDD.'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_fulldate',
      ),
    ),
    'created_year_month' => array(
      'title' => t('Created year + month'),
      'help' => t('Date in the form of YYYYMM.'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_year_month',
      ),
    ),
    'created_year' => array(
      'title' => t('Created year'),
      'help' => t('Date in the form of YYYY.'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_year',
      ),
    ),
    'created_month' => array(
      'title' => t('Created month'),
      'help' => t('Date in the form of MM (01 - 12).'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_month',
      ),
    ),
    'created_day' => array(
      'title' => t('Created day'),
      'help' => t('Date in the form of DD (01 - 31).'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_day',
      ),
    ),
    'created_week' => array(
      'title' => t('Created week'),
      'help' => t('Date in the form of WW (01 - 53).'),
      'argument' => array(
        'field' => 'created',
        'handler' => 'views_handler_argument_node_created_week',
      ),
    ),
    'completed' => array(
      'title' => t('Completed date'),
      'help' => t('The date the summary was completed.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'completed_fulldate' => array(
      'title' => t('Completed date'),
      'help' => t('Date in the form of CCYYMMDD.'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_fulldate',
      ),
    ),
    'completed_year_month' => array(
      'title' => t('Completed year + month'),
      'help' => t('Date in the form of YYYYMM.'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_year_month',
      ),
    ),
    'completed_year' => array(
      'title' => t('Completed year'),
      'help' => t('Date in the form of YYYY.'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_year',
      ),
    ),
    'completed_month' => array(
      'title' => t('Completed month'),
      'help' => t('Date in the form of MM (01 - 12).'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_month',
      ),
    ),
    'completed_day' => array(
      'title' => t('Completed day'),
      'help' => t('Date in the form of DD (01 - 31).'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_day',
      ),
    ),
    'completed_week' => array(
      'title' => t('Completed week'),
      'help' => t('Date in the form of WW (01 - 53).'),
      'argument' => array(
        'field' => 'completed',
        'handler' => 'views_handler_argument_node_created_week',
      ),
    ),
    'snapshot' => array(
      'title' => t('Snapshot'),
      'help' => t('The snapshot for the summary.'),
      'field' => array(
        'handler' => 'up_views_handler_field_image',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'image' => array(
      'title' => t('Image'),
      'help' => t('The image for the summary.'),
      'field' => array(
        'handler' => 'up_views_handler_field_image',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
  );

  return $data;
}
