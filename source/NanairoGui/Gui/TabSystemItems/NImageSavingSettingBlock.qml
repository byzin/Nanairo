/*!
  \file NImageSavingSettingBlock.qml
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
  id: image_saving_setting_block

  labelText: qsTr("StoringImage")

  NCheckBox {
    id: power2_cycle_saving_checkbox

    y: image_saving_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: image_saving_setting_block.color

    text: qsTr("Power2CycleSaving")
  }

  NIntSpinBox {
    id: saving_interval_spinbox

    y: (power2_cycle_saving_checkbox.y + power2_cycle_saving_checkbox.height) + Nanairo.itemOffset
    width: 140

    suffix: " ms " + qsTr("interval")
    maximumValue: Nanairo.integer32Max
    minimumValue: 1
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.power2CycleSaving] = power2_cycle_saving_checkbox.checked;
    sceneData[Nanairo.savingInterval] = saving_interval_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    power2_cycle_saving_checkbox.checked = sceneData[Nanairo.power2CycleSaving];
    saving_interval_spinbox.value = sceneData[Nanairo.savingInterval];
  }
}
