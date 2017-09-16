/*!
  \file NColorSpaceSettingBlock.qml
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
  id: color_space_setting_block 

  labelText: qsTr("ColorSpace")

  NComboBox {
    id: color_space_combobox

    y: color_space_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: color_space_setting_block.color

    model: [Nanairo.sRgbD65,
            Nanairo.sRgbD50,
            Nanairo.adobeRgbD65,
            Nanairo.adobeRgbD50]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.colorSpace] = color_space_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    color_space_combobox.currentIndex =
        color_space_combobox.find(sceneData[Nanairo.colorSpace]);
  }
}
