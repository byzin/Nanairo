/*!
  \file NSpinBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import "../definitions.js" as Definitions

SpinBox {
  id: spinBox

  padding: 0
  topPadding: 0
  bottomPadding: 0
  editable: true
  font.weight: Font.Light
  font.pixelSize: Definitions.defaultTextFontSize
}
