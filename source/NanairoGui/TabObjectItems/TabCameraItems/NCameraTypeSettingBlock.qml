/*!
  \file NCameraTypeSettingBlock.qml
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
  id: camera_type_setting_block

  readonly property alias cameraTypeIndex: camera_type_combobox.currentIndex
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("CameraType")

  NComboBox {
    id: camera_type_combobox

    y: camera_type_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: camera_type_setting_block.color

    model: [Nanairo.pinholeCamera]

    onCurrentIndexChanged: setCurrentItemValue(Nanairo.cameraType, currentText);
  }

  function initializeItem(item) {
    item[Nanairo.cameraType] = camera_type_combobox.textAt(0);
  }

  function reflectItem() {
    var item = currentItem;
    var typeIndex = camera_type_combobox.find(item[Nanairo.cameraType]);
    console.assert(typeIndex != -1,
                   "Camera type isn't found: ", item[Nanairo.cameraType]);
    camera_type_combobox.currentIndex = typeIndex;
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
    itemData[Nanairo.cameraType] = item[Nanairo.cameraType];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.cameraType] = itemData[Nanairo.cameraType];
    return item;
  }
}
