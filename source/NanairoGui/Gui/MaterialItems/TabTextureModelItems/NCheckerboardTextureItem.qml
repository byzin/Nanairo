/*!
  \file NCheckerboardTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

Rectangle {
  id: checkerboard_texture_item

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NCheckerboardResolutionSettingBlock {
    id: checkerboard_resolution_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: checkerboard_texture_item.color
  }

  NColorSettingBlock {
    id: color1_setting_block
    labelText: qsTr("Color1")
    x: checkerboard_resolution_setting_block.x
    y: (checkerboard_resolution_setting_block.y + checkerboard_resolution_setting_block.height) + Nanairo.blockOffset
    color: checkerboard_texture_item.color
    keyPrefix: Nanairo.color1
  }

  NColorSettingBlock {
    id: color2_setting_block
    labelText: qsTr("Color2")
    x: color1_setting_block.x
    y: (color1_setting_block.y + color1_setting_block.height) + Nanairo.blockOffset
    color: checkerboard_texture_item.color
    keyPrefix: Nanairo.color2
  }

  function initializeItem(item) {
    checkerboard_resolution_setting_block.initializeItem(item);
    color1_setting_block.initializeItem(item);
    color2_setting_block.initializeItem(item);
  }

  function reflectItem(item) {
    checkerboard_resolution_setting_block.reflectItem();
    color1_setting_block.reflectItem();
    color2_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    checkerboard_resolution_setting_block.setCurrentItem(item, itemChangeMode);
    color1_setting_block.setCurrentItem(item, itemChangeMode);
    color2_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(checkerboard_resolution_setting_block.getItemData(item));
    dataList.push(color1_setting_block.getItemData(item));
    dataList.push(color2_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(checkerboard_resolution_setting_block.makeItem(itemData));
    dataList.push(color1_setting_block.makeItem(itemData));
    dataList.push(color2_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
