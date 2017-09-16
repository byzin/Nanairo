/*!
  \file NSceneNameSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: scene_name_setting_block

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("SceneName")

  NTextField {
    id: scene_name_editor

    y: scene_name_setting_block.labelHeight + Nanairo.itemOffset

    placeholderText: qsTr("Name")
  }

  function getSceneName() {
    return scene_name_editor.text;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.sceneName] = scene_name_editor.text;
    return sceneData;
  }

  function setSceneData(sceneData) {
    scene_name_editor.text = sceneData[Nanairo.sceneName];
  }
}
