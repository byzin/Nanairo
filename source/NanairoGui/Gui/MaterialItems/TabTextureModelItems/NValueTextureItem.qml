/*!
  \file NValueTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

Rectangle {
  id: value_texture_item

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NValueSettingBlock {
    id: value_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: value_texture_item.color
  }

  function initializeItem(item) {
    value_setting_block.initializeItem(item);
  }

  function reflectItem() {
    value_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    value_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var itemData = {};
    itemData[Nanairo.value] = item[Nanairo.value];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.value] = itemData[Nanairo.value];
    return item;
  }
}
