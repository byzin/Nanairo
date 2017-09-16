/*!
  \file NPhotonSearchRadiusSettingBlock.qml
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
  id: photon_search_radius_setting_block_item

  labelText: qsTr("Photon search radius")

  NSpinBox {
    id: photon_search_radius_spinbox

    y: photon_search_radius_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: 99.9999999
    minimumValue:  0.0000001
  }

  function initialize() {
    photon_search_radius_spinbox.value = 0.02;
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.photonSearchRadius] = photon_search_radius_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    photon_search_radius_spinbox.value = sceneData[Nanairo.photonSearchRadius];
  }
}
