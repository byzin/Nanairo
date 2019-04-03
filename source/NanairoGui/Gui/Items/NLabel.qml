/*!
  \file NLabel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import "../definitions.js" as Definitions

Label {
  id: label

  padding: 0
  leftPadding: 0
  rightPadding: 0
  topPadding: 0
  bottomPadding: 0
  font.pixelSize: Definitions.defaultTextFontSize

  Component.onCompleted: {
    if (nanairoManager.isDebugMode()) {
      var component = Qt.createComponent("NPane.qml");
      background = component.createObject(label);
      background.anchors.fill = label;
    }
  }
}
