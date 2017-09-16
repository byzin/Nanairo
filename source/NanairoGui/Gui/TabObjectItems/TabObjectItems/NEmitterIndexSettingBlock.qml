/*!
  \file NEmitterIndexSettingBlock.qml
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
  id: emitter_index_setting_block 

  property var emitterModel: null
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("EmitterIndex")

  NCheckBox {
    id: emissive_object_checkbox

    y: emitter_index_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: emitter_index_setting_block.color

    text: qsTr("Emissive object")
    onCheckedChanged: setCurrentItemValue(Nanairo.isEmissiveObject, checked)
  }

  NComboBox {
    id: emitter_model_combobox

    y: (emissive_object_checkbox.y + emissive_object_checkbox.height) + Nanairo.itemOffset
    enabled: emissive_object_checkbox.checked
    backgroundColor: emitter_index_setting_block.color

    textRole: "name"
    model: emitter_index_setting_block.emitterModel

    onCurrentIndexChanged: setCurrentItemValue(Nanairo.emitterIndex, currentIndex)
  }

  function initializeItem(item) {
    item[Nanairo.isEmissiveObject] = false;
    item[Nanairo.emitterIndex] = 0;
  }

  function reflectItem() {
    var item = currentItem;
    emissive_object_checkbox.checked = item[Nanairo.isEmissiveObject];
    emitter_model_combobox.currentIndex = item[Nanairo.emitterIndex];
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
    itemData[Nanairo.isEmissiveObject] = item[Nanairo.isEmissiveObject];
    itemData[Nanairo.emitterIndex] = item[Nanairo.emitterIndex];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.isEmissiveObject] = itemData[Nanairo.isEmissiveObject];
    item[Nanairo.emitterIndex] = itemData[Nanairo.emitterIndex];
    return item;
  }
}
