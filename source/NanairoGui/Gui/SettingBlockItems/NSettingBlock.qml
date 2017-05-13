/*!
  \file NSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: setting_block

  property string labelText
  readonly property int labelHeight: block_label.height

  width: Nanairo.defaultBlockWidth
  height: Nanairo.defaultBlockHeight
  border.width: 1
  border.color: Qt.darker(color, Nanairo.defaultDarkerScale)
  radius: 3.0

  Label {
    id: block_label

    x: 8
    y: 2

    text: labelText
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }
}
