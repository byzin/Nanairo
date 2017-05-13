/*!
  \file NClothSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../Styles"
import "../../nanairo.js" as Nanairo

Rectangle {
  id: cloth_surface_item

  property var textureModel: null

  NSettingBlock {
    id: preset_setting_block

    labelText: qsTr("Preset")

    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: cloth_surface_item.color

    NComboBox {
      id: preset_comobobox

      y: preset_setting_block.labelHeight + Nanairo.itemOffset

      style: preset_comobobox_style
      model: ["---",
              // Presets
              "Linen plain: 1.46 0.3 12 24",
              "Linen plain: 1.46 0.1 6 12",
              "Silk crepe: 1.345 0.2 5 10",
              "Silk crepe: 1.345 0.3 18 32",
              "Polyester satin: 1.539 0.1 2.5 5",
              "Polyester satin: 1.539 0.7 30 60"]

      onCurrentIndexChanged: {
        var fabricIsSpecified = 0 < preset_comobobox.currentIndex;
        if (fabricIsSpecified) {
          var preset = preset_comobobox.currentText;
          var regex = /.+:\s(\d+(?:\.\d+)?)\s(\d+(?:\.\d+)?)\s(\d+(?:\.\d+)?)\s(\d+(?:\.\d+)?)/;
          preset.match(regex);
          var n = Number(RegExp.$1);
          var k = Number(RegExp.$2);
          var s = Number(RegExp.$3);
          var v = Number(RegExp.$4);
          fabric_refractive_index_setting_block.setValue(n);
          isotropic_scattering_coefficient_setting_block.setValue(k);
          surface_reflectance_gaussian_width_setting_block.setValue(s);
          volume_reflectance_gaussian_width_setting_block.setValue(v);
          preset_comobobox.currentIndex = 0;
        }
      }
    }

    Component {
      id: preset_comobobox_style
      NComboBoxStyle {
        backgroundColor: cloth_surface_item.color
        label: Label {
          text: qsTr("Preset")
          font.pixelSize: 10
          horizontalAlignment: Text.AlignHLeft
          verticalAlignment: Text.AlignVCenter
          textFormat: Text.PlainText
        }
      }
    }
  }

  NTextureIndexSettingBlock {
    id: texture_index_setting_block
    x: preset_setting_block.x
    y: (preset_setting_block.y + preset_setting_block.height) + Nanairo.blockOffset
    color: cloth_surface_item.color

    textureIndexKey: Nanairo.reflectanceIndex
    labelText: qsTr("Reflectance")
    textureModel: cloth_surface_item.textureModel
  }

  NFabricRefractiveIndexSettingBlock {
    id: fabric_refractive_index_setting_block
    x: (preset_setting_block.x + preset_setting_block.width) + Nanairo.blockOffset
    y: preset_setting_block.y
    color: cloth_surface_item.color
  }

  NIsotropicScatteringCoefficientSettingBlock {
    id: isotropic_scattering_coefficient_setting_block
    x: fabric_refractive_index_setting_block.x
    y: (fabric_refractive_index_setting_block.y + fabric_refractive_index_setting_block.height) + Nanairo.blockOffset
    color: cloth_surface_item.color
  }

  NSurfaceReflectanceGaussianWidthSettingBlock {
    id: surface_reflectance_gaussian_width_setting_block
    x: isotropic_scattering_coefficient_setting_block.x
    y: (isotropic_scattering_coefficient_setting_block.y + isotropic_scattering_coefficient_setting_block.height) + Nanairo.blockOffset
    color: cloth_surface_item.color
  }

  NVolumeReflectanceGaussianWidthSettingBlock {
    id: volume_reflectance_gaussian_width_setting_block
    x: surface_reflectance_gaussian_width_setting_block.x
    y: (surface_reflectance_gaussian_width_setting_block.y + surface_reflectance_gaussian_width_setting_block.height) + Nanairo.blockOffset
    color: cloth_surface_item.color
  }

  NBandwidthParameterSettingBlock {
    id: bandwidth_parameter_setting_block
    x: (fabric_refractive_index_setting_block.x + fabric_refractive_index_setting_block.width) + Nanairo.blockOffset
    y: fabric_refractive_index_setting_block.y
    color: cloth_surface_item.color
  }

  function initializeItem(item) {
    texture_index_setting_block.initializeItem(item);
    fabric_refractive_index_setting_block.initializeItem(item);
    isotropic_scattering_coefficient_setting_block.initializeItem(item);
    surface_reflectance_gaussian_width_setting_block.initializeItem(item);
    volume_reflectance_gaussian_width_setting_block.initializeItem(item);
    bandwidth_parameter_setting_block.initializeItem(item);
    preset_comobobox.currentIndex = 1;
  }

  function reflectItem() {
    texture_index_setting_block.reflectItem();
    fabric_refractive_index_setting_block.reflectItem();
    isotropic_scattering_coefficient_setting_block.reflectItem();
    surface_reflectance_gaussian_width_setting_block.reflectItem();
    volume_reflectance_gaussian_width_setting_block.reflectItem();
    bandwidth_parameter_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    texture_index_setting_block.setCurrentItem(item, itemChangeMode);
    fabric_refractive_index_setting_block.setCurrentItem(item, itemChangeMode);
    isotropic_scattering_coefficient_setting_block.setCurrentItem(item, itemChangeMode);
    surface_reflectance_gaussian_width_setting_block.setCurrentItem(item, itemChangeMode);
    volume_reflectance_gaussian_width_setting_block.setCurrentItem(item, itemChangeMode);
    bandwidth_parameter_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(texture_index_setting_block.getItemData(item));
    dataList.push(fabric_refractive_index_setting_block.getItemData(item));
    dataList.push(isotropic_scattering_coefficient_setting_block.getItemData(item));
    dataList.push(surface_reflectance_gaussian_width_setting_block.getItemData(item));
    dataList.push(volume_reflectance_gaussian_width_setting_block.getItemData(item));
    dataList.push(bandwidth_parameter_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(texture_index_setting_block.makeItem(itemData));
    dataList.push(fabric_refractive_index_setting_block.makeItem(itemData));
    dataList.push(isotropic_scattering_coefficient_setting_block.makeItem(itemData));
    dataList.push(surface_reflectance_gaussian_width_setting_block.makeItem(itemData));
    dataList.push(volume_reflectance_gaussian_width_setting_block.makeItem(itemData));
    dataList.push(bandwidth_parameter_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
