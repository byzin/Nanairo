/*!
  \file NImageButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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

  width: Nanairo.defaultImageButtonSize
  height: Nanairo.defaultImageButtonSize

  text: ""
  style: NImageButtonStyle {
    backgroundColor: button_item.backgroundColor
  }
}
