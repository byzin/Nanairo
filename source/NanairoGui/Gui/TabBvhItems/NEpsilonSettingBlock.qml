/*!
  \file NEpsilonSettingBlock.qml
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
  id: epsilon_setting_block_item

  labelText: qsTr("Epsilon")

  NSpinBox {
    id: epsilon_spinbox

    y: epsilon_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: 0.5
    minimumValue: 0.0
  }

  function initialize() {
    epsilon_spinbox.value = 0.2;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.epsilon] = epsilon_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    epsilon_spinbox.value = sceneData[Nanairo.epsilon];
  }

}
