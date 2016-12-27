/*!
  \file NToneMappingSettingBlock.qml
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
  id: tone_mapping_setting_block

  labelText: qsTr("ToneMapping")
  
  NComboBox {
    id: tone_mapping_combobox

    y: tone_mapping_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: tone_mapping_setting_block.color

    model: [Nanairo.reinhard,
            Nanairo.filmic,
            Nanairo.uncharted2Filmic]
  }

  NSpinBox {
    id: exposure_spinbox

    width: 140
    y: (tone_mapping_combobox.y + tone_mapping_combobox.height) + Nanairo.itemOffset

    suffix: " " + qsTr("exposure")
    maximumValue: Nanairo.integer32Max
    minimumValue: 0.0000001
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.toneMapping] = tone_mapping_combobox.currentText;
    sceneData[Nanairo.exposure] = exposure_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    tone_mapping_combobox.currentIndex =
        tone_mapping_combobox.find(sceneData[Nanairo.toneMapping]);
    exposure_spinbox.value = sceneData[Nanairo.exposure];
  }
}
