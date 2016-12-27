/*!
  \file NJitteringSettingBlock.qml
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
  id: jittering_setting_block 

  property var currentItem: null
  property var isItemChangeMode: false

  labelText: qsTr("Jittering")

  NCheckBox {
    id: jittering_checkbox

    y: jittering_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: jittering_setting_block.color

    text: qsTr("Jittering")
    onCheckedChanged: setCurrentItemValue(Nanairo.jittering, checked)
  }

  function initializeItem(item) {
    item[Nanairo.jittering] = true;
  }

  function reflectItem() {
    var item = currentItem;
    jittering_checkbox.checked = item[Nanairo.jittering];
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
    itemData[Nanairo.jittering] = item[Nanairo.jittering];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.jittering] = itemData[Nanairo.jittering];
    return item;
  }
}
