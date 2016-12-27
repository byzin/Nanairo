/*!
  \file NStandardIlluminationSettingBlock.qml
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
  id: standard_illumination_setting_block

  labelText: "StandardIllumination"

  NComboBox {
    id: standard_illumination_combobox

    y: standard_illumination_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: standard_illumination_setting_block.color

    model: [Nanairo.cieD65,
            Nanairo.cieA]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.standardIllumination] = standard_illumination_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    standard_illumination_combobox.currentIndex =
        standard_illumination_combobox.find(sceneData[Nanairo.standardIllumination]);
  }
}
