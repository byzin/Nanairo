/*!
  \file NPane.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import "../definitions.js" as Definitions

Pane {
  id: pane

  padding: 0

  Component.onCompleted: {
    if (nanairoManager.isDebugMode()) {
      background.border.width = 1;
    }
  }
}
