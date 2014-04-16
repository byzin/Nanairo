/*!
  \file NPathTracingItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import "../keyword.js" as Keyword

Rectangle {
  anchors.fill: parent
//    width: 480
//    height: 274

  function setDefault() {
  }

  function loadSceneSettings(p) {
    var prefix = p + Keyword.pathTracing + "/"
  }

  function saveSceneSettings(p) {
    var prefix = p + Keyword.pathTracing + "/"
  }
}
