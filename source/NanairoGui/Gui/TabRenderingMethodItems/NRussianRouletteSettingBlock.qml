/*!
  \file NRussianRouletteSettingBlock.qml
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
  id: russian_roulette_setting_block 

  readonly property int defaultPathLength: 3

  labelText: qsTr("RussianRoulette")

  NComboBox {
    id: russian_roulette_combobox

    y: russian_roulette_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: russian_roulette_setting_block.color

    model: [Nanairo.rouletteMaxReflectance,
            Nanairo.rouletteAverageReflectance,
            Nanairo.roulettePathLength]

    onCurrentIndexChanged: {
      path_length_spinbox.value = defaultPathLength;
    }
  }

  NIntSpinBox {
    id: path_length_spinbox

    y: (russian_roulette_combobox.y + russian_roulette_combobox.height) + Nanairo.itemOffset
    enabled: russian_roulette_combobox.currentText == Nanairo.roulettePathLength

    maximumValue: 99
    minimumValue: 1
    suffix: " " + qsTr("path")
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.russianRoulette] = russian_roulette_combobox.currentText;
    sceneData[Nanairo.pathLength] = path_length_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    russian_roulette_combobox.currentIndex =
        russian_roulette_combobox.find(sceneData[Nanairo.russianRoulette]);
    path_length_spinbox.value = sceneData[Nanairo.pathLength];
  }
}
