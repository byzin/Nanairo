/*!
  \file NImageTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../../nanairo.js" as Nanairo

Rectangle {
  id: image_texture_item

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NImageSettingBlock {
    id: image_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: image_texture_item.color
  }

  function initializeItem(item) {
    image_setting_block.initializeItem(item);
  }

  function reflectItem() {
    image_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    image_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    return image_setting_block.getItemData(item);
  }

  function makeItem(itemData) {
    return image_setting_block.makeItem(itemData);
  }
}
