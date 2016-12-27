/*!
  \file NLoopCountSettingBlock.qml
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
  id: loop_count_setting_block 

  labelText: qsTr("OptimizationLoop")

  NIntSpinBox {
    id: loop_count_spinbox

    y: loop_count_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: 99
    minimumValue: 1
  }

  function initialize() {
    loop_count_spinbox.value = 2;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.optimizationLoopCount] = loop_count_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    loop_count_spinbox.value = sceneData[Nanairo.optimizationLoopCount];
  }
}
