/*!
  \file NNumOfPhotonsSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: num_of_photons_setting_block

  labelText: qsTr("Num of photons")

  NIntSpinBox {
    id: num_of_photons_spinbox

    y: num_of_photons_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 1024
  }

  function initialize() {
    num_of_photons_spinbox.value = 131072;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.numOfPhotons] = num_of_photons_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    num_of_photons_spinbox.value = sceneData[Nanairo.numOfPhotons];
  }
}
