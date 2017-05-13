/*!
  \file NTerminationCycleSettingBlock.qml
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
  id: termination_cycle_setting_block

  labelText: qsTr("TerminationCycle")

  NIntSpinBox {
    id: termination_cycle_spinbox

    y: termination_cycle_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.terminationCycle] = termination_cycle_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    termination_cycle_spinbox.value = sceneData[Nanairo.terminationCycle];
  }
}
