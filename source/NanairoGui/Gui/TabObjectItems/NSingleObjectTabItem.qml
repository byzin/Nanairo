/*!
  \file NSingleObjectTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "TabObjectItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: single_object_tab_item

  property var emitterModel: null
  property var surfaceModel: null

  width: Nanairo.objectSettingTabWidth
  height: Nanairo.objectSettingTabHeight

  NObjectNameSettingBlock {
    id: object_name_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: single_object_tab_item.color
  }

  NShapeTypeSettingBlock {
    id: shape_type_setting_block
    x: object_name_setting_block.x
    y: (object_name_setting_block.y + object_name_setting_block.height) + Nanairo.blockOffset
    color: single_object_tab_item.color
  }

  NSmoothingSettingBlock {
    id: smoothing_setting_block
    x: shape_type_setting_block.x
    y: (shape_type_setting_block.y + shape_type_setting_block.height) + Nanairo.blockOffset
    color: single_object_tab_item.color
  }

  NEmitterIndexSettingBlock {
    id: emitter_index_setting_block
    x: smoothing_setting_block.x
    y: (smoothing_setting_block.y + smoothing_setting_block.height) + Nanairo.blockOffset
    color: single_object_tab_item.color
    emitterModel: single_object_tab_item.emitterModel
  }

  NSurfaceIndexSettingBlock {
    id: surface_index_setting_block
    x: emitter_index_setting_block.x
    y: (emitter_index_setting_block.y + emitter_index_setting_block.height) + Nanairo.blockOffset
    color: single_object_tab_item.color
    surfaceModel: single_object_tab_item.surfaceModel
  }

  function initializeItem(item) {
    shape_type_setting_block.initializeItem(item);
    smoothing_setting_block.initializeItem(item);
    emitter_index_setting_block.initializeItem(item);
    surface_index_setting_block.initializeItem(item);
  }

  function reflectItem() {
    object_name_setting_block.reflectItem();
    shape_type_setting_block.reflectItem();
    smoothing_setting_block.reflectItem();
    emitter_index_setting_block.reflectItem();
    surface_index_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    object_name_setting_block.setCurrentItem(item, itemChangeMode);
    shape_type_setting_block.setCurrentItem(item, itemChangeMode);
    smoothing_setting_block.setCurrentItem(item, itemChangeMode);
    emitter_index_setting_block.setCurrentItem(item, itemChangeMode);
    surface_index_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(shape_type_setting_block.getItemData(item));
    dataList.push(smoothing_setting_block.getItemData(item));
    dataList.push(emitter_index_setting_block.getItemData(item));
    dataList.push(surface_index_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(shape_type_setting_block.makeItem(itemData));
    dataList.push(smoothing_setting_block.makeItem(itemData));
    dataList.push(emitter_index_setting_block.makeItem(itemData));
    dataList.push(surface_index_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
