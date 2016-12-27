/*!
  \file NKNearestNeighborSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: k_nearest_neighbor_setting_block

  labelText: qsTr("KNearestNeighbor")

  NIntSpinBox {
    id: k_nearest_neighbor_spinbox

    y: k_nearest_neighbor_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 1
  }

  function initialize() {
    k_nearest_neighbor_spinbox.value = 8;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.kNearestNeighbor] = k_nearest_neighbor_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    k_nearest_neighbor_spinbox.value = sceneData[Nanairo.kNearestNeighbor];
  }
}
