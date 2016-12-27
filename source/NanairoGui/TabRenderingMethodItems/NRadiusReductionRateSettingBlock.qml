/*!
  \file NRadiusReductionRateSettingBlock.qml
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
  id: radius_reduction_rate_setting_block_item

  labelText: qsTr("Radius reduction rate")

  NSpinBox {
    id: radius_reduction_rate_spinbox

    y: radius_reduction_rate_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: 1.0
    minimumValue: 0.0000001
  }

  function initialize() {
    radius_reduction_rate_spinbox.value = 0.6666667;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.radiusReductionRate] = radius_reduction_rate_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    radius_reduction_rate_spinbox.value = sceneData[Nanairo.radiusReductionRate];
  }
}
