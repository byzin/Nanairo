/*!
  \file NAgglomerativeTreeletRestructuringBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: agglomerative_treelet_restructuring_bvh_item

  width: Nanairo.bvhTabItemWidth
  height: Nanairo.bvhTabItemHeight

  NTreeletSizeSettingBlock {
    id: treelet_size_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: agglomerative_treelet_restructuring_bvh_item.color
  }

  NLoopCountSettingBlock {
    id: loop_count_setting_block
    x: treelet_size_setting_block.x
    y: (treelet_size_setting_block.y + treelet_size_setting_block.height) + Nanairo.blockOffset
    color: agglomerative_treelet_restructuring_bvh_item.color
  }

  function initialize() {
    treelet_size_setting_block.initialize();
    loop_count_setting_block.initialize();
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(treelet_size_setting_block.getSceneData());
    dataList.push(loop_count_setting_block.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    treelet_size_setting_block.setSceneData(sceneData);
    loop_count_setting_block.setSceneData(sceneData);
  }
}
