/*!
  \file NComboBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../Styles"
import "../nanairo.js" as Nanairo

ComboBox {
  id: combobox_item

  property color backgroundColor

  anchors.horizontalCenter: parent.horizontalCenter
  width: Nanairo.defaultComboboxWidth
  height: Nanairo.defaultComboboxHeight

  model: []
  style: NComboBoxStyle {
    fontPixelSize: 10
    backgroundColor: combobox_item.backgroundColor
  }
}
