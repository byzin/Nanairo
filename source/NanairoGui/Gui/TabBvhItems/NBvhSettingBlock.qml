/*!
  \file NBvhSettingBlock.qml
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
  id: bvh_setting_block

  readonly property int currentBvhIndex: bvh_type_combobox.currentIndex

  labelText: qsTr("Bvh")

  NComboBox {
    id: bvh_type_combobox

    y: bvh_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: bvh_setting_block.color

    model: [Nanairo.binaryRadixTreeBvh,
            Nanairo.approximateAgglomerativeClusteringBvh,
            Nanairo.agglomerativeTreeletRestructuringBvh]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.type] = bvh_type_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    bvh_type_combobox.currentIndex = bvh_type_combobox.find(sceneData[Nanairo.type]);
  }
}
