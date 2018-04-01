/*!
  \file NTextField.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import "../definitions.js" as Definitions

TextField {
  id: textField

  Material.elevation: 0

  padding: 0
  leftPadding: 0
  rightPadding: 0
  topPadding: 0
  bottomPadding: 0
  font.pixelSize: Definitions.defaultTextFontSize
  font.weight: Font.Light
}
