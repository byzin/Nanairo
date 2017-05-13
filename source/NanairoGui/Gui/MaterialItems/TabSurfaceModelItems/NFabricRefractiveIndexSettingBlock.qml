/*!
  \file NFabricRefractiveIndexSettingBlock.qml
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
  id: fabric_refractive_index_setting_block_item

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("Refractive index")

  NSpinBox {
    id: fabric_refractive_index_spinbox

    y: fabric_refractive_index_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0.0000001

    onValueChanged: setCurrentItemValue(Nanairo.fabricRefractiveIndex, value)
  }

  function initializeItem(item) {
    item[Nanairo.fabricRefractiveIndex] = 1.0;
  }

  function reflectItem() {
    var item = currentItem;
    fabric_refractive_index_spinbox.value = item[Nanairo.fabricRefractiveIndex];
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

  function setValue(n) {
    fabric_refractive_index_spinbox.value = n;
  }

  function getItemData(item) {
    var itemData = {};
    itemData[Nanairo.fabricRefractiveIndex] = item[Nanairo.fabricRefractiveIndex];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.fabricRefractiveIndex] = itemData[Nanairo.fabricRefractiveIndex];
    return item;
  }

}
