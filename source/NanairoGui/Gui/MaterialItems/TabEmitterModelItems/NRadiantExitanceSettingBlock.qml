/*!
  \file NRadiantExitanceSettingBlock.qml
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
  id: radiant_exitance_setting_block

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("RadiantExitance")

  NSpinBox {
    id: radiant_exitance_spinbox 

    y: radiant_exitance_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0.0000001

    onEditingFinished: {
      setCurrentItemValue(Nanairo.radiantExitance, value);
    }
  }

  function initializeItem(item) {
    item[Nanairo.radiantExitance] = 128.0;
  }

  function reflectItem() {
    var item = currentItem;
    // Radiant exitance
    radiant_exitance_spinbox.value = item[Nanairo.radiantExitance];
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
    itemData[Nanairo.radiantExitance] = item[Nanairo.radiantExitance];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.radiantExitance] = itemData[Nanairo.radiantExitance];
    return item;
  }
}
