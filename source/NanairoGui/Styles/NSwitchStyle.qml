/*!
  \file NSwitchStyle.qml
  \author Sho Ikeda
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../nanairo.js" as Nanairo

SwitchStyle {
  property color backgroundColor
  property int switchWidth: 128
  property int switchHeight: 16
  property string falseText: ""
  property string trueText: ""
  property int fontPixelSize: 11

  groove: Rectangle {
    id: switch_groove

    implicitWidth: switchWidth
    implicitHeight: switchHeight
    radius: 1
    color: backgroundColor
    border.color: Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)
    border.width: 1
  }

  handle: Rectangle {
    id: switch_handle

    implicitWidth: switchWidth / 2
    implicitHeight: switchHeight
    radius: 1
    color: Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)

    Label {
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.verticalCenter: parent.verticalCenter
      text: control.checked ? trueText : falseText
      font.pixelSize: fontPixelSize
    }
  }
}
