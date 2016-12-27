/*!
  \file NBandwidthParameterSettingBlock.qml
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
  id: bandwidth_parameter_setting_block_item

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("Bandwidth parameter")

  NSpinBox {
    id: bandwidth_parameter_spinbox

    y: bandwidth_parameter_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0.0000001

    onValueChanged: setCurrentItemValue(Nanairo.bandwidthParameter, value)
  }

  function initializeItem(item) {
    item[Nanairo.bandwidthParameter] = 1.0;
  }

  function reflectItem() {
    var item = currentItem;
    bandwidth_parameter_spinbox.value = item[Nanairo.bandwidthParameter];
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
    itemData[Nanairo.bandwidthParameter] = item[Nanairo.bandwidthParameter];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.bandwidthParameter] = itemData[Nanairo.bandwidthParameter];
    return item;
  }
}
