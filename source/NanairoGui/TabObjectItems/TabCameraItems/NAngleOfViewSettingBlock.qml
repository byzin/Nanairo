/*!
  \file NAngleOfViewSettingBlock.qml
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
  id: angle_of_view_setting_block

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("AngleOfView")

  NSpinBox {
    id: angle_of_view_spinbox

    y: angle_of_view_setting_block.labelHeight + Nanairo.itemOffset

    suffix: qsTr(" deg")
    maximumValue: 89.9999999
    minimumValue:  0.0000001

    onEditingFinished: setCurrentItemValue(Nanairo.angleOfView, value)
  }

  function initializeItem(item) {
    item[Nanairo.angleOfView] = 45.0;
  }

  function reflectItem() {
    var item = currentItem;
    angle_of_view_spinbox.value = item[Nanairo.angleOfView];
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
    itemData[Nanairo.angleOfView] = item[Nanairo.angleOfView];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.angleOfView] = itemData[Nanairo.angleOfView];
    return item;
  }
}
