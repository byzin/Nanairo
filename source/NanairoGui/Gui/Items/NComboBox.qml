/*!
  \file NComboBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import "../definitions.js" as Definitions

ComboBox {
  id: comboBox

  Material.elevation: 0

  padding: 0
  leftPadding: 0
  rightPadding: 0
  topPadding: 0
  bottomPadding: 0
  font.weight: Font.Light
  font.pixelSize: Definitions.defaultTextFontSize

  Component.onCompleted: {
    background.anchors.fill = comboBox;
    popup.width = 2 * width;
    popup.font.weight = Font.Light;
    popup.font.pixelSize = Definitions.defaultTextFontSize;
  }
}
