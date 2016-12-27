/*!
  \file NSmoothDiffuseSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

Rectangle {
  id: smooth_diffuse_surface_item

  property var textureModel: null

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NTextureIndexSettingBlock {
    id: texture_index_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: smooth_diffuse_surface_item.color

    textureIndexKey: Nanairo.reflectanceIndex
    labelText: qsTr("ReflectanceIndex")
    textureModel: smooth_diffuse_surface_item.textureModel
  }

  function initializeItem(item) {
    texture_index_setting_block.initializeItem(item);
  }

  function reflectItem() {
    texture_index_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    texture_index_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    return texture_index_setting_block.getItemData(item);
  }

  function makeItem(itemData) {
    return texture_index_setting_block.makeItem(itemData);
  }
}
