<?php

/**
 * @file
 * Definition of up_views_handler_field_image.
 */

/**
 * Field handler to provide simple renderer that allows using a themed up picture.
 *
 * @ingroup views_field_handlers
 */
class up_views_handler_field_image extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function element_type($none_supported = FALSE, $default_empty = FALSE, $inline = FALSE) {
    if ($inline) {
      return 'span';
    }
    if ($none_supported) {
      if ($this->options['element_type'] === '0') {
        return '';
      }
    }
    if ($this->options['element_type']) {
      return check_plain($this->options['element_type']);
    }
    if ($default_empty) {
      return '';
    }
    if (isset($this->definition['element type'])) {
      return $this->definition['element type'];
    }

    return 'div';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['image_style'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (module_exists('image')) {
      $styles = image_styles();
      $style_options = array('' => t('Original'));
      foreach ($styles as $style) {
        $style_options[$style['name']] = $style['name'];
      }

      $form['image_style'] = array(
        '#title' => t('Image style'),
        '#description' => t('Using <em>Original</em> will use the unmodified image.'),
        '#type' => 'select',
        '#options' => $style_options,
        '#default_value' => $this->options['image_style'],
      );
    }
  }

  function render($values) {
    // Early return.
    $value = $this->get_value($values);
    if (empty($value)) {
      return NULL;
    }

    $image = up_image($value);

    if ($this->options['image_style'] && module_exists('image')) {
        $output = theme('up_image_style', array('style_name' => $this->options['image_style'], 'path' => $image));
    }
    else {
      $output = theme('image', array('path' => $image));
    }

    return $output;
  }
}
