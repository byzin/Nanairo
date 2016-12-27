/*!
  \file NGammaSettingBlock.qml
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
  id: gamma_setting_block
  
  labelText: qsTr("GammaCorrection")

  NSpinBox {
    id: gamma_spinbox 

    y: gamma_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: 99.9999999
    minimumValue:  0.0000001
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.gamma] = gamma_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    gamma_spinbox.value = sceneData[Nanairo.gamma];
  }
}
