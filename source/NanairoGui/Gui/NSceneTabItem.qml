/*!
  \file NSceneTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "TabSceneItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: system_tab_item

  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NSceneNameSettingBlock {
    id: scene_name_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: system_tab_item.color
  }

  function getSceneName() {
    return scene_name_setting_block.getSceneName();
  }

  function getSceneData() {
    var sceneData = {};

    var data = scene_name_setting_block.getSceneData();
    for (var key in data) {
      sceneData[key] = data[key];
    }
    return sceneData;
  }

  function setSceneData(sceneData) {
    scene_name_setting_block.setSceneData(sceneData);
  }
}
