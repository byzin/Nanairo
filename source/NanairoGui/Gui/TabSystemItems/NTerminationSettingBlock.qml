/*!
  \file NTerminationSettingBlock.qml
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
  id: termination_setting_block

  labelText: qsTr("Termination")

  NIntSpinBox {
    id: termination_cycle_spinbox

    height: 18
    y: termination_setting_block.labelHeight + Nanairo.itemOffset

    suffix: " cycle"
    maximumValue: Nanairo.integer32Max
    minimumValue: 0
  }

  NIntSpinBox {
    id: termination_time_spinbox

    height: 18
    y: (termination_cycle_spinbox.y + termination_cycle_spinbox.height) + Nanairo.itemOffset
    suffix: " ms"
    maximumValue: Nanairo.integer32Max
    minimumValue: 0
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.terminationCycle] = termination_cycle_spinbox.value;
    sceneData[Nanairo.terminationTime] = termination_time_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    termination_cycle_spinbox.value = sceneData[Nanairo.terminationCycle];
    termination_time_spinbox.value = sceneData[Nanairo.terminationTime];
  }
}
