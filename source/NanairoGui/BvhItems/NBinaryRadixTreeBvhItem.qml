/*!
  \file NBinaryRadixTreeBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import "../keyword.js" as Keyword

Rectangle {
  id: binary_radix_tree_bvh_item
  anchors.fill: parent
//  width: 480
//  height: 274

  function setDefault() {
  }

  function loadSceneSettings(p) {
    var prefix = p + Keyword.binaryRadixTreeBvh + "/"
  }

  function saveSceneSettings(p) {
    var prefix = p + Keyword.binaryRadixTreeBvh + "/"
  }
}
