/*!
  \file NNonDirectionalEmitterItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../TabSurfaceModelItems"
import "../../nanairo.js" as Nanairo

Rectangle {
  id: non_directional_emitter_item

  property var textureModel: null

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  NTextureIndexSettingBlock {
    id: texture_index_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: non_directional_emitter_item.color

    textureIndexKey: Nanairo.emissiveColorIndex
    labelText: qsTr("EmissiveColorIndex")
    textureModel: non_directional_emitter_item.textureModel
  }

  NRadiantExitanceSettingBlock {
    id: radiant_exitance_setting_block
    x: texture_index_setting_block.x
    y: (texture_index_setting_block.y + texture_index_setting_block.height) + Nanairo.blockOffset
    color: non_directional_emitter_item.color
  }

  function initializeItem(item) {
    texture_index_setting_block.initializeItem(item);
    radiant_exitance_setting_block.initializeItem(item);
  }

  function reflectItem() {
    texture_index_setting_block.reflectItem();
    radiant_exitance_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    texture_index_setting_block.setCurrentItem(item, itemChangeMode);
    radiant_exitance_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(texture_index_setting_block.getItemData(item));
    dataList.push(radiant_exitance_setting_block.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(texture_index_setting_block.makeItem(itemData));
    dataList.push(radiant_exitance_setting_block.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
