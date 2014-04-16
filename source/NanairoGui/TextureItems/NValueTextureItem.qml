/*!
  \file NValueTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import ".."
import "../keyword.js" as Keyword

Rectangle {
  anchors.fill: parent
//  width: 476
//  height: 240

  readonly property real defaultValue: 0.8
  property var currentTextureItem: null

  Label {
    id: value_label
    x: 8
    y: 8
    text: qsTr("Value")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  Component {
    id: spinbox_button_style
    NSpinBoxStyle {
    }
  }

  SpinBox {
    id: value_spinbox
    x: 16
    y: 27
    width: 100
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 1.0
    minimumValue: 0.0000001
    decimals: 7

    onValueChanged: {
      if (currentTextureItem != null) {
        currentTextureItem.value = value
      }
    }
  }

  onCurrentTextureItemChanged: {
    if (currentTextureItem != null) {
      value_spinbox.value = currentTextureItem.value
    }
  }

  function setDefaultTexture(textureItem) {
    textureItem.value = defaultValue
  }

  function loadSceneSettings(item, p) {
    var prefix = p + Keyword.valueTexture + "/"
    item.value = scene_settings.realValue(prefix + Keyword.value, defaultValue)
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.valueTexture + "/"
    scene_settings.setRealValue(prefix + Keyword.value, item.value)
  }
}
