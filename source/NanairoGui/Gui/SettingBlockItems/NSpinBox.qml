/*!
  \file NSpinBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../Styles"
import "../nanairo.js" as Nanairo

SpinBox {
  id: spinbox_item

  anchors.horizontalCenter: parent.horizontalCenter
  width: Nanairo.defaultSpinboxWidth
  height: Nanairo.defaultSpinboxHeight

  font.pixelSize: 10
  decimals: 7
  horizontalAlignment: Qt.AlignRight

  style: NSpinBoxStyle {
  }
}
