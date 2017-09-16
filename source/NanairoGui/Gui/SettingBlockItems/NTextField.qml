/*!
  \file NTextField.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../Styles"
import "../nanairo.js" as Nanairo

TextField {
  id: text_field_item

  anchors.horizontalCenter: parent.horizontalCenter
  width: Nanairo.defaultTextFieldWidth
  height: Nanairo.defaultTextFieldHeight

  placeholderText: ""
  style: NTextFieldStyle {
  }
}
