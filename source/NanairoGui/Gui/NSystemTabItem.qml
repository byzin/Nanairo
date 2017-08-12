/*!
  \file NSystemTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "TabSystemItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: system_tab_item

  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NThreadSettingBlock {
    id: thread_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: system_tab_item.color
  }

  NRandomSeedSettingBlock {
    id: random_seed_setting_block
    x: thread_setting_block.x
    y: (thread_setting_block.y + thread_setting_block.height) + Nanairo.blockOffset
    color: system_tab_item.color
  }

  NTerminationSettingBlock {
    id: termination_setting_block
    x: random_seed_setting_block.x
    y: (random_seed_setting_block.y + random_seed_setting_block.height) + Nanairo.blockOffset
    color: system_tab_item.color
  }

  NImageResolutionSettingBlock {
    id: image_resolution_setting_block 
    x: termination_setting_block.x
    y: (termination_setting_block.y + termination_setting_block.height) + Nanairo.blockOffset
    color: system_tab_item.color
  }

  NImageSavingSettingBlock {
    id: image_saving_setting_block
    x: image_resolution_setting_block.x
    y: (image_resolution_setting_block.y + image_resolution_setting_block.height) + Nanairo.blockOffset
    color: system_tab_item.color
  }

  function getImageResolution() {
    return image_resolution_setting_block.getImageResolution();
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(thread_setting_block.getSceneData());
    dataList.push(random_seed_setting_block.getSceneData());
    dataList.push(termination_setting_block.getSceneData());
    dataList.push(image_resolution_setting_block.getSceneData());
    dataList.push(image_saving_setting_block.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    thread_setting_block.setSceneData(sceneData);
    random_seed_setting_block.setSceneData(sceneData);
    termination_setting_block.setSceneData(sceneData);
    image_resolution_setting_block.setSceneData(sceneData);
    image_saving_setting_block.setSceneData(sceneData);
  }
}
