/*!
  \file NButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../Styles"
import "../nanairo.js" as Nanairo

Button {
  id: button_item

  property color backgroundColor

  anchors.horizontalCenter: parent.horizontalCenter
  width: Nanairo.defaultButtonWidth
  height: Nanairo.defaultButtonHeight

  text: ""
  style: NButtonStyle {
    backgroundColor: button_item.backgroundColor
    fontPixelSize: 10
  }
}
