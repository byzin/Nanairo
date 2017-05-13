/*!
  \file NRgbColorPalette.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: rgb_color_palette

  property string keyPrefix: ""
  property var currentItem: null
  property bool isItemChangeMode: false

  color: color_palette.color

  ColorDialog {
    id: color_palette
    color: "white"
    currentColor: "white"

    onColorChanged: setCurrentItemColor(keyPrefix + Nanairo.value, color)
  }

  NButton {
    id: palette_button

    anchors.fill: parent
    backgroundColor: color_palette.color

    text: qsTr("")
    onClicked: color_palette.open()
  }

  function makeColor(red, green, blue) {
    return Qt.rgba(red, green, blue, 1.0);
  }

  function initializeItem(item) {
    // RGB
    item[keyPrefix + Nanairo.value + "r"] = 1.0;
    item[keyPrefix + Nanairo.value + "g"] = 1.0;
    item[keyPrefix + Nanairo.value + "b"] = 1.0;
  }

  function reflectItem() {
    var item = currentItem;
    // RGB
    color_palette.color = makeColor(item[keyPrefix + Nanairo.value + "r"],
                                    item[keyPrefix + Nanairo.value + "g"],
                                    item[keyPrefix + Nanairo.value + "b"]);
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function setCurrentItemColor(key, value) {
    if (currentItem != null) {
      currentItem[key + "r"] = value.r;
      currentItem[key + "g"] = value.g;
      currentItem[key + "b"] = value.b;
    }
  }

  function getItemData(item) {
    var rgb = item[keyPrefix + Nanairo.value];
    var itemData = {};
    itemData[Nanairo.value] = [item[keyPrefix + Nanairo.value + "r"],
                               item[keyPrefix + Nanairo.value + "g"],
                               item[keyPrefix + Nanairo.value + "b"]];
    return itemData;
  }

  function makeItem(itemData) {
    var rgb = itemData[Nanairo.value];
    var item = {};
    item[keyPrefix + Nanairo.value + "r"] = rgb[0];
    item[keyPrefix + Nanairo.value + "g"] = rgb[1];
    item[keyPrefix + Nanairo.value + "b"] = rgb[2];
    return item;
  }
}
