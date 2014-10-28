<?php

/**
 * @file
 * Miscellaneous functions
 */


/**
 * Return a list those models that are content models
 *
 * Expects a content models array produced by $object->models
 * (array values are the pids for those content models)
 *
 * @param $models
 * @return array
 */
function _iw_content_models($models) {
  return array_intersect(array_keys(islandora_get_content_models()), array_values($models));
}


/**
 * Get a list of object pids and their labels found in this repository.
 *
 * Filter by type: 'collections' or 'cmodels'
 *
 * @string $type
 *  'collections' or 'cmodels'
 * @return array
 *  A list of object pids and their labels
 */
function iw_get_collections_or_content_models($type) {
  $types = array(
    'collections' => 'islandora:collectionCModel',
    'cmodels' => 'fedora-system:ContentModel-3.0'
  );
  $type_for_query = '<info:fedora/' . $types[$type] . '>';
  // Query the rindex to get all collections.
  $tuque = islandora_get_tuque_connection();
  $ri_query = "select \$object \$label from <#ri>
     where \$object <fedora-model:hasModel> $type_for_query
      and \$object <fedora-model:label> \$label";
  $results = $tuque->repository->ri->itqlQuery($ri_query, 'unlimited');
  $ret = array();
  foreach ($results as $member) {
    $pid = preg_replace('/info:fedora\//', '', $member['object']['value']);
    $label = $member['label']['value'];
    // We don't want the root collection.
    if ($pid == 'islandora:root') {
      $label = 'Islandora Root Collection';
    }
    $ret[$pid] = $label;
  }
  return $ret;
}