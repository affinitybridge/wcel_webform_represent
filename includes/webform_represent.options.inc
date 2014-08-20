<?php

/**
 * @file
 *   A collection of built-in select list options for Webform Represent.
 */

/**
 * Option list containing the representatives in a representative set.
 */
function webform_represent_options_representatives($component, $flat, $filter, $arguments) {
  $options = array();

  $resources = represent_representatives_by_set($arguments);
  if (!empty($resources)) {
    foreach ($resources as $resource) {
      if (isset($resource->email)) {
        $value = $resource->name;
        if (!empty($resource->district_name)) {
          $value .= " ($resource->district_name)";
        }
        $options[$resource->email] = $value;
      }
    }

    // Remove representatives with non-unique emails.
    foreach (array_count_values(array_keys($options)) as $email => $count) {
      if ($count > 1) {
        unset($options[$email]);

        watchdog('Represent API', 'Duplicate email "@email" in set "@set"', array(
          '@email' => $email,
          '@set' => $arguments,
        ), WATCHDOG_WARNING);
      }
    }

    asort($options);
  }

  return $options;
}
