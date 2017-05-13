/*!
  \file NSwitch.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../Styles"
import "../nanairo.js" as Nanairo

Switch {
  id: switch_item

  property color backgroundColor
  property string falseText
  property string trueText

  anchors.horizontalCenter: parent.horizontalCenter

  style: NSwitchStyle {
    backgroundColor: switch_item.backgroundColor
    switchWidth: Nanairo.defaultSwitchWidth
    switchHeight: Nanairo.defaultSwitchHeight
    fontPixelSize: 10
    falseText: switch_item.falseText
    trueText: switch_item.trueText
  }
}
