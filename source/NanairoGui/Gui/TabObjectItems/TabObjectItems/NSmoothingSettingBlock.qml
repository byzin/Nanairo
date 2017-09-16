/*!
  \file NSmoothingSettingBlock.qml
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
  id: smoothing_setting_block 

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("Smoothing")

  NCheckBox {
    id: smoothing_checkbox

    y: smoothing_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: smoothing_setting_block.color

    text: qsTr("Smoothing")

    onCheckedChanged: setCurrentItemValue(Nanairo.smoothing, checked)
  }

  function initializeItem(item) {
    item[Nanairo.smoothing] = false;
  }

  function reflectItem() {
    var item = currentItem;
    smoothing_checkbox.checked = item[Nanairo.smoothing];
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
    itemData[Nanairo.smoothing] = item[Nanairo.smoothing];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.smoothing] = itemData[Nanairo.smoothing];
    return item;
  }
}
