/*!
  \file NComboBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../definitions.js" as Definitions

ComboBox {
  id: comboBox

  property int popupWidthScale: 2

  Material.elevation: 0

  padding: 0
  leftPadding: 0
  rightPadding: 0
  topPadding: 0
  bottomPadding: 0
  font.weight: Font.Normal
  font.pixelSize: Definitions.defaultTextFontSize
  background.anchors.fill: comboBox

  onPressedChanged: {
    initComboBox();
  }

  Component.onCompleted: {
    initComboBox();
  }

  function initComboBox() {
    popup.width = width * popupWidthScale;
    popup.font.weight = Font.Normal;
    popup.font.pixelSize = Definitions.defaultTextFontSize;
  }
}
