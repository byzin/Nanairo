/*!
  \file NRayCastSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: ray_cast_setting_block 

  labelText: qsTr("RayCasting")

  NSpinBox {
    id: ray_cast_epsilon_spinbox

    y: ray_cast_setting_block.labelHeight + Nanairo.itemOffset

    suffix: " " + qsTr("epsilon")
    maximumValue: 1
    minimumValue: 0.0000001
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.rayCastEpsilon] = ray_cast_epsilon_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    ray_cast_epsilon_spinbox.value = sceneData[Nanairo.rayCastEpsilon];
  }
}
