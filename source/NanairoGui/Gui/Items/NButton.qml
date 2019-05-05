/*!
  \file NButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../definitions.js" as Definitions

Button {
  id: button

  Material.elevation: 0

  padding: 0
  leftPadding: 0
  rightPadding: 0
  topPadding: 0
  bottomPadding: 0
  font.pixelSize: Definitions.defaultTextFontSize

  Component.onCompleted: {
    // Initialize the button background
    background.anchors.fill = button;
    background.border.width = 1;
    background.border.color = Material.color(Material.Grey);
  }
}
