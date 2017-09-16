/*!
  \file NImageButtonStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../nanairo.js" as Nanairo

ButtonStyle {
  property color backgroundColor
  background: Rectangle {
    color: !control.pressed ? backgroundColor
                            : Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)
    radius: 4
    border.color: control.enabled ? Qt.darker(color, Nanairo.defaultDarkerScale)
                                  : Qt.darker(color, 1.1)
    border.width: 1
  }
}
