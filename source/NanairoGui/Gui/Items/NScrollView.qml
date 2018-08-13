/*!
  \file NScrollView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import "../definitions.js" as Definitions

ScrollView {
  id: scrollView

  background: Rectangle {}
  clip: true
  ScrollBar.vertical.width: Definitions.defaultScrollBarSize
  ScrollBar.horizontal.height: Definitions.defaultScrollBarSize

  Component.onCompleted: {
    if (nanairoManager.isDebugMode()) {
      background.border.width = 1;
    }
  }
}
