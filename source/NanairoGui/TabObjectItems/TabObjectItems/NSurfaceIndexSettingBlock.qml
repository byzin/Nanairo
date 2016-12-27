/*!
  \file NSurfaceIndexSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NSettingBlock {
  id: surface_index_setting_block

  property var surfaceModel: null
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("SurfaceIndex")

  NComboBox {
    id: surface_model_combobox

    y: surface_index_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: surface_index_setting_block.color

    textRole: "name"
    model: surface_index_setting_block.surfaceModel

    onCurrentIndexChanged: setCurrentItemValue(Nanairo.surfaceIndex, currentIndex)
  }

  function initializeItem(item) {
    item[Nanairo.surfaceIndex] = 0;
  }

  function reflectItem() {
    var item = currentItem;
    surface_model_combobox.currentIndex = item[Nanairo.surfaceIndex];
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
    itemData[Nanairo.surfaceIndex] = item[Nanairo.surfaceIndex];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.surfaceIndex] = itemData[Nanairo.surfaceIndex];
    return item;
  }
}
