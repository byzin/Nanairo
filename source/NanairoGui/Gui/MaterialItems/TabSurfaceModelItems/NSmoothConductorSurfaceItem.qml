/*!
  \file NSmoothConductorSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

Rectangle {
  id: smooth_conductor_surface_item

  property var textureModel: null
  readonly property alias outerRefractiveIndexSettingBlock: outer_refractive_index_setting_block
  readonly property alias innerRefractiveIndexSettingBlock: inner_refractive_index_setting_block
  readonly property alias innerExtinctionSettingBlock: inner_extinction_setting_block

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NTextureIndexSettingBlock {
    id: outer_refractive_index_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: smooth_conductor_surface_item.color

    textureIndexKey: Nanairo.outerRefractiveIndex
    labelText: qsTr("Outer refractive index")
    textureModel: smooth_conductor_surface_item.textureModel
  }

  NTextureIndexSettingBlock {
    id: inner_refractive_index_setting_block
    x: outer_refractive_index_setting_block.x
    y: (outer_refractive_index_setting_block.y + outer_refractive_index_setting_block.height) + Nanairo.blockOffset
    color: smooth_conductor_surface_item.color

    textureIndexKey: Nanairo.innerRefractiveIndex
    labelText: qsTr("Inner refractive index")
    textureModel: smooth_conductor_surface_item.textureModel
  }

  NTextureIndexSettingBlock {
    id: inner_extinction_setting_block
    x: inner_refractive_index_setting_block.x
    y: (inner_refractive_index_setting_block.y + inner_refractive_index_setting_block.height) + Nanairo.blockOffset
    color: smooth_conductor_surface_item.color

    textureIndexKey: Nanairo.innerExtinction
    labelText: qsTr("Inner extinction")
    textureModel: smooth_conductor_surface_item.textureModel
  }

  function initializeItem(item) {
    outer_refractive_index_setting_block.initializeItem(item);
    inner_refractive_index_setting_block.initializeItem(item);
    inner_extinction_setting_block.initializeItem(item);
  }

  function reflectItem() {
    outer_refractive_index_setting_block.reflectItem();
    inner_refractive_index_setting_block.reflectItem();
    inner_extinction_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    outer_refractive_index_setting_block.setCurrentItem(item, itemChangeMode);
    inner_refractive_index_setting_block.setCurrentItem(item, itemChangeMode);
    inner_extinction_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(outer_refractive_index_setting_block.getItemData(item));
    dataList.push(inner_refractive_index_setting_block.getItemData(item));
    dataList.push(inner_extinction_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(outer_refractive_index_setting_block.makeItem(itemData));
    dataList.push(inner_refractive_index_setting_block.makeItem(itemData));
    dataList.push(inner_extinction_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
