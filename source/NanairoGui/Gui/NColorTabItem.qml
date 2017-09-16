/*!
  \file NColorTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "Styles"
import "TabColorItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: color_tab_item
  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NSpectraRgbSettingBlock {
    id: spectra_rgb_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: color_tab_item.color
  }

  NWavelengthSamplingSettingBlock {
    id: wavelength_sampling_setting_block
    x: spectra_rgb_setting_block.x
    y: (spectra_rgb_setting_block.y + spectra_rgb_setting_block.height) + Nanairo.blockOffset
    color: color_tab_item.color
  }

  NColorSpaceSettingBlock {
    id: color_space_setting_block
    x: wavelength_sampling_setting_block.x
    y: (wavelength_sampling_setting_block.y + wavelength_sampling_setting_block.height) + Nanairo.blockOffset
    color: color_tab_item.color
  }

  NGammaSettingBlock {
    id: gamma_setting_block
    x: (spectra_rgb_setting_block.x + spectra_rgb_setting_block.width) + Nanairo.blockOffset
    y: spectra_rgb_setting_block.y
    color: color_tab_item.color
  }

  NToneMappingSettingBlock {
    id: tone_mapping_setting_block
    x: gamma_setting_block.x
    y: (gamma_setting_block.y + gamma_setting_block.height) + Nanairo.blockOffset
    color: color_tab_item.color
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(spectra_rgb_setting_block.getSceneData()); 
    dataList.push(wavelength_sampling_setting_block.getSceneData()); 
    dataList.push(color_space_setting_block.getSceneData()); 
    dataList.push(gamma_setting_block.getSceneData()); 
    dataList.push(tone_mapping_setting_block.getSceneData()); 
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    spectra_rgb_setting_block.setSceneData(sceneData);
    wavelength_sampling_setting_block.setSceneData(sceneData);
    color_space_setting_block.setSceneData(sceneData);
    gamma_setting_block.setSceneData(sceneData);
    tone_mapping_setting_block.setSceneData(sceneData);
  }
}
