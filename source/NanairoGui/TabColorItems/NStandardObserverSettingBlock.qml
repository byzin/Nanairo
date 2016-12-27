/*!
  \file NStandardObserverSettingBlock.qml
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
  id: standard_observer_setting_block

  labelText: "StandardObserver"

  NComboBox {
    id: standard_observer_combobox

    y: standard_observer_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: standard_observer_setting_block.color

    model: [Nanairo.cie2Deg,
            Nanairo.cie10Deg]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.standardObserver] = standard_observer_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    standard_observer_combobox.currentIndex = 
        standard_observer_combobox.find(sceneData[Nanairo.standardObserver]);
  }
}
