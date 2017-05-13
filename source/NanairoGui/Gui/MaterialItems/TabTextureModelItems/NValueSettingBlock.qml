/*!
  \file NValueSettingBlock.qml
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
  id: value_setting_block 

  property var currentItem: null
  property bool isItemChangeMode: false
  
  labelText: qsTr("Value")

  NSpinBox {
    id: value_spinbox

    y: value_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: 99.9999999
    minimumValue: 0.0000001

    onEditingFinished: {
      setCurrentItemValue(Nanairo.value, value);
    }
  }

  function initializeItem(item) {
    item[Nanairo.value] = 0.8;
  }

  function reflectItem() {
    var item = currentItem;
    // Value
    value_spinbox.value = item[Nanairo.value];
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
}
