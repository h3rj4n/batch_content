<?php

/**
 * @file
 * Contains all the batch functions for the batch_content module.
 */

/**
 * Update the path aliases for nodes.
 *
 * @param array $node_types
 * @param array $context
 */
function _batch_content_update_path_alias(array $node_types, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;

    // Count the maximum amount of nodes.
    $count = db_select('node', 'n')
      ->condition('n.type', $node_types, 'IN')
      ->countQuery()->execute()->fetchField();

    $context['sandbox']['max'] = $count;
  }

  // Select the nids we want to process now.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', $node_types, 'IN')
    ->condition('n.nid', $context['sandbox']['current_node'], '>')
    ->orderBy('n.nid')
    ->range(0, 10);
  $nids = $query->execute()->fetchCol();

  // Update the aliases.
  pathauto_node_update_alias_multiple($nids, 'bulkupdate');

  // Use the last nid as the current_node value.
  $context['sandbox']['current_node'] = $nids[9];
  // Add 10 nodes to the progress.
  $context['sandbox']['progress'] += 10;

  $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];

  $context['message'] = 'Current processing ' . $context['sandbox']['progress'] . ' of max ' . $context['sandbox']['max'];
}
