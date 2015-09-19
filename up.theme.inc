<?php
/**
 * @file
 * Theme functions for Jawbone UP.
 */

/**
 * Display a pretty band name.
 */
function theme_up_band_name($variables) {
  return t('@band\'s UP', array('@band' => $variables['band']->first_name . ' ' . $variables['band']->last_name));
}

/**
 * Display a locally cached preformatted image.
 */
function theme_up_image_style($variables) {
  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  image_style_transform_dimensions($variables['style_name'], $dimensions);

  $variables['width']  = $dimensions['width'];
  $variables['height'] = $dimensions['height'];

  // Cache the image locally.
  $cache = up_cache_image($variables['path']);

  // Determine the URL for the styled image.
  $variables['path'] = image_style_url($variables['style_name'], $cache);

  return theme('image', $variables);
}
