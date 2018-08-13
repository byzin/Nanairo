/*!
  \file NGroupBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import "../definitions.js" as Definitions

GroupBox {
  id: groupBox

  property var color: Material.color(Material.Grey)

  label: NLabel {
    x: groupBox.leftPadding
    width: groupBox.availableWidth
    color: Material.color(Material.BlueGrey)
    elide: Text.ElideRight
    text: groupBox.title
  }

  background: Rectangle {
    y: groupBox.topPadding - groupBox.bottomPadding
    width: parent.width
    height: parent.height - groupBox.topPadding + groupBox.bottomPadding
    color: Qt.darker(groupBox.color, 1.05)
    radius: 2
    border.color: Material.color(Material.BlueGrey)
  }
}