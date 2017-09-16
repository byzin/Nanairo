/*!
  \file NLayeredDiffuseSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

NSmoothDielectricSurfaceItem {
  id: layered_diffuse_surface_item

  property var textureModel: null

  outerRefractiveIndexSettingBlock.textureModel: textureModel
  innerRefractiveIndexSettingBlock.textureModel: textureModel

  NTextureIndexSettingBlock {
    id: roughness_index_setting_block
    x: innerRefractiveIndexSettingBlock.x
    y: (innerRefractiveIndexSettingBlock.y + innerRefractiveIndexSettingBlock.height) + Nanairo.blockOffset
    color: layered_diffuse_surface_item.color

    textureIndexKey: Nanairo.roughnessIndex
    labelText: qsTr("Roughness")
    textureModel: layered_diffuse_surface_item.textureModel
  }

  NTextureIndexSettingBlock {
    id: reflectance_index_setting_block
    x: roughness_index_setting_block.x
    y: (roughness_index_setting_block.y + roughness_index_setting_block.height) + Nanairo.blockOffset
    color: layered_diffuse_surface_item.color

    textureIndexKey: Nanairo.reflectanceIndex
    labelText: qsTr("Reflectance")
    textureModel: layered_diffuse_surface_item.textureModel
  }


  function initializeItem(item) {
    outerRefractiveIndexSettingBlock.initializeItem(item);
    innerRefractiveIndexSettingBlock.initializeItem(item);
    roughness_index_setting_block.initializeItem(item);
    reflectance_index_setting_block.initializeItem(item);
  }

  function reflectItem() {
    outerRefractiveIndexSettingBlock.reflectItem();
    innerRefractiveIndexSettingBlock.reflectItem();
    roughness_index_setting_block.reflectItem();
    reflectance_index_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    outerRefractiveIndexSettingBlock.setCurrentItem(item, itemChangeMode);
    innerRefractiveIndexSettingBlock.setCurrentItem(item, itemChangeMode);
    roughness_index_setting_block.setCurrentItem(item, itemChangeMode);
    reflectance_index_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(outerRefractiveIndexSettingBlock.getItemData(item));
    dataList.push(innerRefractiveIndexSettingBlock.getItemData(item));
    dataList.push(roughness_index_setting_block.getItemData(item));
    dataList.push(reflectance_index_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(outerRefractiveIndexSettingBlock.makeItem(itemData));
    dataList.push(innerRefractiveIndexSettingBlock.makeItem(itemData));
    dataList.push(roughness_index_setting_block.makeItem(itemData));
    dataList.push(reflectance_index_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
