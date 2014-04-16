/*!
  \file NCheckerboardTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import ".."
import "../nanairo_definitions.js" as Nanairo
import "../keyword.js" as Keyword

Rectangle {
  anchors.fill: parent
//  width: 476
//  height: 240

  readonly property int defaultWidth: 2
  readonly property int defaultHeight: 2
  property var currentTextureItem: null

  Component {
    id: spinbox_button_style
    NSpinBoxButtonStyle {
    }
  }

  Label {
    id: square_label
    x: 8
    y: 8
    text: qsTr("Squares")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: width_spinbox
    x: 16
    y: 27
    width: 63
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    prefix: "w: "
    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 1

    onValueChanged: {
      if (currentTextureItem != null)
        currentTextureItem.width = value
      }
  }

  Label {
    id: cross_label
    x: 85
    y: 30
    text: qsTr("x")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: height_spinbox 
    x: 97
    y: 27
    width: 63
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    prefix: "h: "
    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 1

    onValueChanged: {
      if (currentTextureItem != null)
        currentTextureItem.height = value
    }
  }

  Label {
    id: color1_label
    x: 8
    y: 63
    text: qsTr("Color1")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  NColorView {
    id: color1_palette
    x: 16
    y: 82
    color: parent.color
  }

  Label {
    id: color2_label
    x: 8
    y: 108
    text: qsTr("Color2")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  NColorView {
    id: color2_palette
    x: 16
    y: 127
    color: parent.color
  }

  onCurrentTextureItemChanged: {
    if (currentTextureItem != null) {
      width_spinbox.value = currentTextureItem.width
      height_spinbox.value = currentTextureItem.height
      color1_palette.currentItem = null
      color1_palette.currentItem = currentTextureItem.textureColor1
      color2_palette.currentItem = null
      color2_palette.currentItem = currentTextureItem.textureColor2
    }
  }

  function setDefaultTexture(textureItem) {
    textureItem.width = defaultWidth
    textureItem.height = defaultHeight
    textureItem.textureColor1 = [true, Qt.rgba(0.8, 0.8, 0.8, 1), Qt.resolvedUrl("")]
    textureItem.textureColor2 = [true, Qt.rgba(0.2, 0.2, 0.2, 1), Qt.resolvedUrl("")]
  }

  function loadSceneSettings(item, p) {
    setDefaultTexture(item)
    var prefix = p + Keyword.checkerboardTexture + "/"
    item.width = scene_settings.intValue(prefix + Keyword.width, defaultWidth)
    item.height = scene_settings.intValue(prefix + Keyword.height, defaultHeight)
    color1_palette.loadSceneSettings(item.textureColor1, 
                                     prefix + Keyword.textureColor + "1/")
    color2_palette.loadSceneSettings(item.textureColor2, 
                                     prefix + Keyword.textureColor + "2/")
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.checkerboardTexture + "/"
    scene_settings.setIntValue(prefix + Keyword.width, item.width)
    scene_settings.setIntValue(prefix + Keyword.height, item.height)
    color1_palette.saveSceneSettings(item.textureColor1, 
                                     prefix + Keyword.textureColor + "1/")
    color2_palette.saveSceneSettings(item.textureColor2, 
                                     prefix + Keyword.textureColor + "2/")
  }
}
