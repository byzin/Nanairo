/*!
  \file NGroupObjectTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "TabObjectItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: group_tab_item

  width: Nanairo.objectSettingTabWidth
  height: Nanairo.objectSettingTabWidth

  NObjectNameSettingBlock {
    id: group_name_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: group_tab_item.color

    labelText: qsTr("GroupName")
  }

  function initializeItem(item) {
  }

  function reflectItem() {
    group_name_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    group_name_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var itemData = {};
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    return item;
  }
}
