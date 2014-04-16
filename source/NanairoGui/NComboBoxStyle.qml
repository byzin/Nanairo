/*!
  \file NComboBoxStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ComboBoxStyle {
  property color backgroundColor
  property int fontPixelSize: 11

  background: Rectangle {
    color: backgroundColor
    border.color: control.enabled ? Qt.darker(color, 1.3) : Qt.darker(color, 1.1)
    border.width: 2

    Rectangle {
      x: parent.width - 20
      width: 20
      height: parent.height
      color: parent.border.color
    }
  }

  label: Label {
    clip: true
    text: control.currentText
    font.pixelSize: fontPixelSize
    horizontalAlignment: Text.AlignHLeft
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }
}
