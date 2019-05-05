/*!
  \file NProbabilisticPpmMethodItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NScrollView {
  id: methodItem

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "light path light sampler"
    }

    NLightSampler {
      id: lightSampler

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: methodItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      isEyePathSampler: false
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "number of photons"
    }

    NSpinBox {
      id: numOfPhotonsSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: methodItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1024
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "k nearest neighbor"
    }

    NSpinBox {
      id: kNearestNeighborSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: methodItem.width
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
