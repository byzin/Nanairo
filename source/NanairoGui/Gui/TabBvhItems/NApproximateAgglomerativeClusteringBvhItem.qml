/*!
  \file NApproximateAgglomerativeClusteringBvhItem.qml
  \author Sho Ikeda
  
  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: approximate_agglomerative_clustering_bvh_item

  width: Nanairo.bvhTabItemWidth
  height: Nanairo.bvhTabItemHeight

  NDeltaSettingBlock {
    id: delta_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: approximate_agglomerative_clustering_bvh_item.color
  }

  NEpsilonSettingBlock {
    id: epsilon_setting_block
    x: delta_setting_block.x
    y: (delta_setting_block.y + delta_setting_block.height) + Nanairo.blockOffset
    color: approximate_agglomerative_clustering_bvh_item.color
  }

  function initialize() {
    delta_setting_block.initialize();
    epsilon_setting_block.initialize();
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(delta_setting_block.getSceneData());
    dataList.push(epsilon_setting_block.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    delta_setting_block.setSceneData(sceneData);
    epsilon_setting_block.setSceneData(sceneData);
  }
}
