/*!
  \file NLightSampler.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: lightSamplerItem

  property bool isEyePathSampler: true


  ColumnLayout {
    id: column1

    width: lightSamplerItem.width
    spacing: Definitions.defaultItemSpace

    NComboBox {
      id: samplerComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: 0

      Component.onCompleted: {
        var samplerList = [Definitions.uniformLightSampler,
                           Definitions.powerWeightedLightSampler];
        model = samplerList;
      }
    }
  }

  function getKey() {
    return (lightSamplerItem.isEyePathSampler) ? Definitions.eyePathLightSampler
                                               : Definitions.lightPathLightSampler;
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[getKey()] = samplerComboBox.currentText;

    return sceneData;
  }

  function initSceneData() {
    samplerComboBox.currentIndex =
        samplerComboBox.find(Definitions.powerWeightedLightSampler);
  }

  function setSceneData(sceneData) {
    samplerComboBox.currentIndex = samplerComboBox.find(
        Definitions.getProperty(sceneData, getKey()));
  }
}
