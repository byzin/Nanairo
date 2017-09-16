/*!
  \file NImageResolutionSettingBlock.qml
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
  id: checkerboard_resolution_setting_block

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("CheckerboardResolution")

  NIntSpinBox {
    id: width_resolution_spinbox

    anchors.horizontalCenter: undefined
    x: cross_label.x - (width + 5)
    y: checkerboard_resolution_setting_block.labelHeight + Nanairo.itemOffset
    width: 60
    height: 20

    maximumValue: Nanairo.integer32Max
    minimumValue: 1

    onValueChanged: {
      setCurrentItemValue(Nanairo.imageResolution + "x", value);
    }
  }

  Label {
    id: cross_label

    anchors.horizontalCenter: checkerboard_resolution_setting_block.horizontalCenter
    y: width_resolution_spinbox.y

    text: qsTr("x")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
  }

  NIntSpinBox {
    id: height_resolution_spinbox

    anchors.horizontalCenter: undefined
    x: cross_label.x + cross_label.width + 5
    y: cross_label.y
    width: 60
    height: 20

    maximumValue: Nanairo.integer32Max
    minimumValue: 1

    onValueChanged: {
      setCurrentItemValue(Nanairo.imageResolution + "y", value);
    }
  }

  function initializeItem(item) {
    item[Nanairo.imageResolution + "x"] = 10;
    item[Nanairo.imageResolution + "y"] = 10;
  }

  function reflectItem() {
    var item = currentItem;
    // Resolution
    var width = item[Nanairo.imageResolution + "x"];
    var height = item[Nanairo.imageResolution + "y"];
    width_resolution_spinbox.value = width;
    height_resolution_spinbox.value = height;
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
    var width = item[Nanairo.imageResolution + "x"];
    var height = item[Nanairo.imageResolution + "y"];
    itemData[Nanairo.imageResolution] = [width, height];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    var resolution = itemData[Nanairo.imageResolution];
    item[Nanairo.imageResolution + "x"] = resolution[0];
    item[Nanairo.imageResolution + "y"] = resolution[1];
    return item;
  }
}
