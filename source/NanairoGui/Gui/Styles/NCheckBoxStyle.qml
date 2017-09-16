/*!
  \file NCheckBoxStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../nanairo.js" as Nanairo

CheckBoxStyle {
  id: check_box_style
  property color backgroundColor

  background: Rectangle {
    id: check_box_area

    color: backgroundColor
  }

  indicator: Rectangle {
    implicitWidth: 16
    implicitHeight: 16
    radius: 1
    color: backgroundColor
    border.color: Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)
    border.width: 1
    Rectangle {
      visible: control.checked
      color: "#555"
      border.color: "#333"
      radius: 1
      anchors.margins: 4
      anchors.fill: parent
    }
  }
}
