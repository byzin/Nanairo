/*!
  \file NImageButtonStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

ButtonStyle {
  property color backgroundColor
  background: Rectangle {
    color: !control.pressed ? backgroundColor : Qt.darker(backgroundColor, 1.2)
    radius: 4
    border.color: control.enabled ? Qt.darker(color, 1.2) : Qt.darker(color, 1.1)
    border.width: 1
  }
}
