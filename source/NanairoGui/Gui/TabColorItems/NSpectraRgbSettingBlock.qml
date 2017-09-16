/*!
  \file NSpectraRgbSettingBlock.qml
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
  id: spectra_rgb_setting_block
  
  labelText: qsTr("ColorMode")

  NSwitch {
    id: spectra_rgb_switch

    y: spectra_rgb_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: spectra_rgb_setting_block.color

    falseText: "RGB"
    trueText: "Spectra"
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.colorMode] = spectra_rgb_switch.checked ? Nanairo.spectra
                                                              : Nanairo.rgb;
    return sceneData;
  }

  function setSceneData(sceneData) {
    var mode = sceneData[Nanairo.colorMode];
    spectra_rgb_switch.checked = (mode == Nanairo.spectra);
  }
}
