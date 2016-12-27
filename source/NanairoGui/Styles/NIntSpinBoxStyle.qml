/*!
  \file NIntSpinBoxStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../nanairo.js" as Nanairo

SpinBoxStyle {
  background: Rectangle {
    width: control.width
    height: control.height
    color: "white"
    border.color: Qt.darker(color, Nanairo.defaultDarkerScale)
    border.width: 1
  }
}
