/*!
  \file NProbabilisticPpmMethodItem.qml
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
  id: methodItem

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "light path light sampler"
    }

    NLightSampler {
      id: lightSampler

      Layout.fillWidth: true
      isEyePathSampler: false
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "number of photons"
    }

    NSpinBox {
      id: numOfPhotonsSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1024
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "k nearest neighbor"
    }

    NSpinBox {
      id: kNearestNeighborSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1
      to: Definitions.intMax
    }
  }

  function initSceneData() {
    lightSampler.initSceneData();
    numOfPhotonsSpinBox.value = 131072;
    kNearestNeighborSpinBox.value = 8;
  }

  function getSceneData() {
    var sceneData = lightSampler.getSceneData();
    sceneData[Definitions.numOfPhotons] = numOfPhotonsSpinBox.value;
    sceneData[Definitions.kNearestNeighbor] = kNearestNeighborSpinBox.value;

    return sceneData;
  }

  function setSceneData(sceneData) {
    numOfPhotonsSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.numOfPhotons);
    kNearestNeighborSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.kNearestNeighbor);

    lightSampler.setSceneData(sceneData);
  }
}
