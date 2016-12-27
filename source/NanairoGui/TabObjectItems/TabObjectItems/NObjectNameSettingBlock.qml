/*!
  \file NObjectNameSettingBlock.qml
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
  id: object_name_setting_block

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("ObjectName")

  NTextField {
    id: object_name_editor

    y: object_name_setting_block.labelHeight + Nanairo.itemOffset

    placeholderText: qsTr("Name")
    onEditingFinished: setCurrentItemValue("name", text);
  }

  function reflectItem() {
    var item = currentItem;
    object_name_editor.text = item["name"];
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
