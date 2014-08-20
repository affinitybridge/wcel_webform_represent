<?php

/**
 * @file
 *  Represent module Represent component.
 */

/**
 * Specify the default properties of a component.
 *
 * @return
 *   An array defining the default structure of a component.
 */
function _webform_defaults_represent() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'title_display' => 0,
      'description' => '',
      'private' => FALSE,
      'postal_code_component' => '',
      // Quack like a select component.
      'aslist' => 1,
      'multiple' => 0,
      'optrand' => 0,

      'other_option' => 0,
      'options_source' => '',
    ),
  );
}

/**
 * Generate the form for editing a component.
 *
 * Create a set of form elements to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they
 * may be overridden if desired).
 *
 * @param $component
 *   A Webform component array.
 * @return
 *   An array of form items to be displayed on the edit component page.
 */
function _webform_edit_represent($component) {
  $form = array();

  drupal_add_js(drupal_get_path('module', 'webform') . '/js/select-admin.js', 'module', 'header', FALSE, TRUE, FALSE);
  drupal_add_js(array('webform' => array('selectOptionsUrl' => url('webform/ajax/options/' . $component['nid']))), 'setting');

  webform_component_include('select');
  if ($info = _webform_select_options_info()) {
    $names = array_keys(webform_represent_representative_sets());

    $options = array('' => t('- Select -'));
    foreach ($info as $name => $source) {
      if (in_array($name, $names)) {
        $options[$name] = $source['title'];
      }
    }

    $form['extra']['options_source'] = array(
      '#title'         => t('Representative set'),
      '#type'          => 'select',
      '#options'       => $options,
      '#default_value' => $component['extra']['options_source'],
      '#description'   => t("The elected official from this representative set whose electoral district intersects the supporter's postal code will receive a copy of the submission."),
      '#required'      => TRUE,
    );
  }

  $node = node_load($component['nid']);
  $options = array('' => t('- Select -'));
  if (!empty($node->webform['components'])) {
    foreach ($node->webform['components'] as $array) {
      if ($array['type'] == 'textfield') {
        $options[$array['form_key']] = $array['name'];
      }
    }
  }

  $form['extra']['postal_code_component'] = array(
    '#title'         => t('Postal code component'),
    '#type'          => 'select',
    '#options'       => $options,
    '#default_value' => $component['extra']['postal_code_component'],
    '#description'   => t('If you have not yet created a postal code component, go back and create one.'),
    '#required'      => TRUE,
  );

  return $form;
}

function _webform_render_represent($component, $value = NULL, $filter = TRUE) {
  static $done = false;

  if (!$done) {
    drupal_add_js(drupal_get_path('module', 'webform_represent') . '/assets/webform_represent.js');
    drupal_add_css(drupal_get_path('module', 'webform_represent') . '/assets/webform_represent.css');
    drupal_add_js(array('webform_represent' => array('spinner' => drupal_get_path('module', 'webform_represent') . '/assets/webform_represent.gif')), 'setting');
    $done = true;
  }

  drupal_add_js(array('webform_represent' => array(
    'lookups' => array(
      $component['form_key'] => array(
        'options_source'        => $component['extra']['options_source'],
        'postal_code_component' => $component['extra']['postal_code_component'],
      ),
    ),
  )), 'setting');

  webform_component_include('select');
  return _webform_render_select($component, $value, $filter);
}

function _webform_display_represent($component, $value, $format = 'html') {
  webform_component_include('select');
  return _webform_display_select($component, $value, $format);
}
function _webform_submit_represent($component, $value) {
  webform_component_include('select');
  return _webform_submit_select($component, $value);
}
function _webform_analysis_represent($component, $sids = array(), $single = FALSE) {
  webform_component_include('select');
  return _webform_analysis_select($component, $sids, $single);
}
function _webform_csv_headers_represent($component, $export_options) {
  webform_component_include('select');
  $headers = _webform_csv_headers_select($component, $export_options);
  $headers[0][] = '';
  $headers[1][] = '';
  $headers[2][] = t('@name Email', array('@name' => $component['name']));
  return $headers;
}
function _webform_csv_data_represent($component, $export_options, $value) {
  webform_component_include('select');
  $return = _webform_csv_data_select($component, $export_options, $value);
  if (!is_array($return)) {
    $return = array($return);
  }
  $return[] = $value[0];
  return $return;
}
function _webform_table_represent($component, $value) {
  webform_component_include('select');
  return _webform_table_select($component, $value);
}

/*
 * We do not need to define:
 * - _webform_attachments_component
 * - _webform_delete_component
 * - _webform_help_component
 * - _webform_submit_component
 * - _webform_theme_component
 */
