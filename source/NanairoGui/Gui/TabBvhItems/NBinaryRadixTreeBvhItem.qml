/*!
  \file NBinaryRadixTreeBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: binary_radix_tree_bvh_item

  width: Nanairo.bvhTabItemWidth
  height: Nanairo.bvhTabItemHeight

  function initialize() {
  }

  function getSceneData() {
    return {};
  }

  function setSceneData(sceneData) {
  }
}
