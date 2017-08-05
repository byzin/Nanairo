/*!
  \file NEyePathLightSamplerSettingBlock.qml
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
  id: light_sampler_setting_block 

  labelText: qsTr("EyePathLightSsampler")

  NComboBox {
    id: light_sampler_combobox

    y: light_sampler_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: light_sampler_setting_block.color

    model: [Nanairo.uniformLightSampler,
            Nanairo.powerWeightedLightSampler,
            Nanairo.contributionWeightedLightSampler]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.eyePathLightSampler] = light_sampler_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    light_sampler_combobox.currentIndex =
        light_sampler_combobox.find(sceneData[Nanairo.eyePathLightSampler]);
  }
}
