/*!
  \file NNonDirectionalEmitterItem.qml
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

  readonly property real defaultRadiantExitance: 128.0
  property var currentEmitterItem: null

  Label {
    id: color_palette_label
    x: 8
    y: 8
    text: qsTr("Color")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  NColorView {
    id: color_palette
    x: 16
    y: 27
    color: parent.color
  }

  Label {
    id: radiant_exitance_label
    x: 8
    y: 64
    text: qsTr("Radiant exitance")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  Component {
    id: spinbox_button_style
    NSpinBoxStyle {
    }
  }

  SpinBox {
    id: radiant_exitance_spinbox
    x: 16
    y: 83
    width: 100
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 0.0000001
    decimals: 7

    onValueChanged: {
      if (currentEmitterItem != null)
        currentEmitterItem.radiantExitance = value
    }
  }

  onCurrentEmitterItemChanged: {
    if (currentEmitterItem != null) {
      color_palette.currentItem = null
      color_palette.currentItem = currentEmitterItem.lightColor
      radiant_exitance_spinbox.value = currentEmitterItem.radiantExitance
    }
  }

  function setDefaultEmitter(emitterItem) {
    emitterItem.lightColor = [true, Qt.rgba(1, 1, 1, 1), Qt.resolvedUrl("")]
    emitterItem.radiantExitance = defaultRadiantExitance
  }

  function loadSceneSettings(item, p) {
    setDefaultEmitter(item)
    var prefix = p + Keyword.nonDirectionalEmitter + "/"
    color_palette.loadSceneSettings(item.lightColor, prefix + Keyword.color + "/")
    item.radiantExitance = scene_settings.realValue(prefix + Keyword.radiantExitance, 
                                                    defaultRadiantExitance)
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.nonDirectionalEmitter + "/"
    color_palette.saveSceneSettings(item.lightColor, prefix + Keyword.color + "/")
    scene_settings.setRealValue(prefix + Keyword.radiantExitance, 
                                item.radiantExitance)
  }
}
