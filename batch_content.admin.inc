<?php

/**
 * @file
 * Contains all the admin forms for the batch_content module.
 */

/**
 * The form for altering all the path aliases.
 *
 * @param array $form
 * @param array $form_state
 *
 * @return array
 */
function batch_content_update_path(array $form, array &$form_state) {

  $form['content_type'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Content type',
    '#options' => _batch_content_options(),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Submit handler for the path alias update form.
 *
 * @see batch_content_update_path().
 *
 * @param array $form
 * @param array $form_state
 */
function batch_content_update_path_submit(array &$form, array &$form_state) {
  // Get the content types from the form_state array.
  $types = _batch_content_form_content_types($form_state);

  // Create the batch array.
  $batch = array(
    'title' => t('Updating content type(s)'),
    'operations' => array(
      array('_batch_content_update_path_alias', array($types)),
    ),
    // This functions is inside the batch_content.module.
    'finished' => 'batch_content_batch_finished',
    'file' => drupal_get_path('module', 'batch_content') . '/batch_content.batch.inc',
  );

  // Execute the batch.
  batch_set($batch);
}
