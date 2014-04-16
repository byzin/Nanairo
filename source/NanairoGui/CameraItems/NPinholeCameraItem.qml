/*!
  \file NPinholeCameraItem.qml
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
  width: 422
  height: 178

  readonly property real defaultAngleOfView: 45.0
  property var currentObjectItem

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  Label {
    id: angle_of_view_label
    x: 8
    y: 8
    text: qsTr("Angle of view")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: angle_of_view_spinbox
    x: 16
    y: 30
    width: 100
    height: 20
    font.pixelSize: 11
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    minimumValue:  0.0000001
    maximumValue: 89.9999999
    decimals: 7

    onValueChanged: {
      if (currentObjectItem != null)
        currentObjectItem.angleOfView = value
    }
  }

  onCurrentObjectItemChanged: {
    if (currentObjectItem != null) {
      angle_of_view_spinbox.value = currentObjectItem.angleOfView
    }
  }

  function setDefault(objectItem) {
    objectItem.angleOfView = defaultAngleOfView
  }

  function loadSceneSettings(item, p) {
    var prefix = p + Keyword.pinholeCamera + "/"
    item.angleOfView = scene_settings.realValue(prefix + Keyword.angleOfView, 
                                                defaultAngleOfView)
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.pinholeCamera + "/"
    scene_settings.setRealValue(prefix + Keyword.angleOfView, item.angleOfView)
  }
}
