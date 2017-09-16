/*!
  \file NRandomSeedSettingBlock.qml
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
  id: random_seed_setting_block

  labelText: qsTr("RandomSeed")

  NIntSpinBox {
    id: random_seed_spinbox

    y: random_seed_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0
  }

  NButton {
    id: random_set_button

    y: (random_seed_spinbox.y + random_seed_spinbox.height) + Nanairo.itemOffset
    width: 80
    backgroundColor: random_seed_setting_block.color

    text: qsTr("Random set")

    onClicked: random_seed_spinbox.value = nanairoManager.random()
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.randomSeed] = random_seed_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    random_seed_spinbox.value = sceneData[Nanairo.randomSeed];
  }
}
