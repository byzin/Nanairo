/*!
  \file NDeltaSettingBlock.qml
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
  id: delta_setting_block_item

  labelText: qsTr("Delta")

  NIntSpinBox {
    id: delta_spinbox

    y: delta_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: 99
    minimumValue: 1
  }

  function initialize() {
    delta_spinbox.value = 4;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.delta] = delta_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    delta_spinbox.value = sceneData[Nanairo.delta];
  }
}
