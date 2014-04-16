/*!
  \file NRgbColorPalette.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import "keyword.js" as Keyword

Rectangle {
  id: rgb_color_palette
  anchors.fill: parent

  property var currentItem: null

  ColorDialog {
    id: color_palette
    color: "white"
    currentColor: "white"

    onColorChanged: {
      if (currentItem != null)
        setItemColor(currentItem, color)
    }
  }

  Component {
    id: palette_button_style
    ButtonStyle {
      background: Rectangle {
        color: color_palette.color
        radius: 4
        border.color: Qt.darker(rgb_color_palette.color, 1.2)
        border.width: 1
      }
    }
  }

  Button {
    id: palette_button
    anchors.fill: parent
    text: qsTr("")
    style: palette_button_style
    onClicked: color_palette.open()
  }

  onCurrentItemChanged: {
    if (currentItem != null) {
      var c = getItemColor(currentItem)
      color_palette.color = makeColor(c)
    }
  }

  function makeColor(c) {
    return Qt.rgba(c.r, c.g, c.b, c.a)
  }

  function getItemColor(item) {
    return item[1]
  }

  function setItemColor(item, c) {
    item[1] = makeColor(c)
  }

  function setDefaultColor(item) {
    setItemColor(item, Qt.rgba(1, 1, 1, 1))
  }

  function loadSceneSettings(item, prefix) {
    var rgbColor = scene_settings.colorValue(prefix + Keyword.rgbColor, 
                                             Qt.rgba(1, 1, 1, 1))
    setItemColor(item, rgbColor)
    currentItem = null
    currentItem = item
  }

  function saveSceneSettings(item, prefix) {
    var rgbColor = getItemColor(item)
    scene_settings.setColorValue(prefix + Keyword.rgbColor, rgbColor)
  }
}
