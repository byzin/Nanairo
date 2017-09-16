/*!
  \file NTextureIndexSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NSettingBlock {
  id: texture_index_setting_block

  property string textureIndexKey: Nanairo.value
  property var textureModel: null
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("TextureIndex")

  NComboBox {
    id: texture_index_combobox

    y: texture_index_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: texture_index_setting_block.color

    onCurrentIndexChanged: {
      setCurrentItemValue(textureIndexKey, currentIndex);
    }

    textRole: "name"
    model: textureModel
  }

  function initializeItem(item) {
    item[textureIndexKey] = 0;
  }

  function reflectItem() {
    var item = currentItem;
    // Reflectance index
    texture_index_combobox.currentIndex = item[textureIndexKey];
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function setCurrentItemValue(key, value) {
    if (currentItem != null) {
      currentItem[key] = value;
    }
  }

  function getItemData(item) {
    var itemData = {};
    itemData[textureIndexKey] = item[textureIndexKey];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[textureIndexKey] = itemData[textureIndexKey];
    return item;
  }
}
