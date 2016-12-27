/*!
  \file NTreeletSizeSettingBlock.qml
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
  id: treelet_size_setting_block 

  labelText: qsTr("TreeletSize")

  NIntSpinBox {
    id: treelet_size_spinbox

    y: treelet_size_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: 99
    minimumValue: 4
  }

  function initialize() {
    treelet_size_spinbox.value = 9;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.treeletSize] = treelet_size_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    treelet_size_spinbox.value = sceneData[Nanairo.treeletSize];
  }
}
