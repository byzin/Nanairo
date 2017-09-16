/*!
  \file NEyePathLightSamplerSettingBlock.qml
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
  id: light_sampler_setting_block 

  height: 2 * Nanairo.defaultBlockHeight

  labelText: qsTr("EyePathLightSsampler")

  NComboBox {
    id: light_sampler_combobox

    y: light_sampler_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: light_sampler_setting_block.color

    model: [Nanairo.uniformLightSampler,
            Nanairo.powerWeightedLightSampler,
            Nanairo.contributionWeightedLightSampler]
  }

  NIntSpinBox {
    id: max_surface_split_spinbox

    y: (light_sampler_combobox.y + light_sampler_combobox.height) + Nanairo.itemOffset
    enabled: light_sampler_combobox.currentText == Nanairo.contributionWeightedLightSampler

    maximumValue: 99
    minimumValue: 1
    prefix: "max split: "
  }

  NIntSpinBox {
    id: num_of_photons_spinbox 

    y: (max_surface_split_spinbox.y + max_surface_split_spinbox.height) + Nanairo.itemOffset
    enabled: light_sampler_combobox.currentText == Nanairo.contributionWeightedLightSampler

    maximumValue: Nanairo.integer32Max
    minimumValue: 1024
    prefix: "num photons: "
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.eyePathLightSampler] = light_sampler_combobox.currentText;
    sceneData[Nanairo.lightSamplerMaxSurfaceSplit] = max_surface_split_spinbox.value;
    sceneData[Nanairo.lightSamplerNumOfPhotons] = num_of_photons_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    light_sampler_combobox.currentIndex =
        light_sampler_combobox.find(sceneData[Nanairo.eyePathLightSampler]);
    max_surface_split_spinbox.value =
        sceneData[Nanairo.lightSamplerMaxSurfaceSplit];
    num_of_photons_spinbox.value =
        sceneData[Nanairo.lightSamplerNumOfPhotons];
  }
}
