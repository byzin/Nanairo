/*!
  \file NWavelengthSamplingSettingBlock.qml
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
  id: wavelength_sampling_setting_block

  labelText: qsTr("WavelengthSampling")
  
  NComboBox {
    id: wavelength_sampling_combobox

    y: wavelength_sampling_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: wavelength_sampling_setting_block.color

    model: [Nanairo.regularSampling,
            Nanairo.randomSampling,
            Nanairo.stratifiedSampling,
            Nanairo.lightsBasedSampling]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.wavelengthSampling] = wavelength_sampling_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    wavelength_sampling_combobox.currentIndex =
        wavelength_sampling_combobox.find(sceneData[Nanairo.wavelengthSampling]);
  }
}
