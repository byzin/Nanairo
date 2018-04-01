/*!
  \file NProbabilisticPpmMethodItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
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
      text: "photon search radius"
    }

    NFloatSpinBox {
      id: photonSearchRadiusSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      decimals: 6
      floatFrom: 0.000001
      floatTo: realMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "radius reduction rate"
    }

    NFloatSpinBox {
      id: radiusReductionRateSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      decimals: 7
      floatFrom: 0.0000001
      floatTo: 1.0
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

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "kernel"
    }

    NComboBox {
      id: kernelComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: 0
      model: [Definitions.noKernel,
              Definitions.coneKernel,
              Definitions.perlinKernel]
    }
  }

  function initSceneData() {
    lightSampler.initSceneData();
    numOfPhotonsSpinBox.value = 131072;
    photonSearchRadiusSpinBox.floatValue = 0.02;
    radiusReductionRateSpinBox.floatValue = 0.6666666;
    kNearestNeighborSpinBox.value = 8;
    kernelComboBox.currentIndex = kernelComboBox.find(Definitions.noKernel);
  }

  function getSceneData() {
    var sceneData = lightSampler.getSceneData();
    sceneData[Definitions.numOfPhotons] = numOfPhotonsSpinBox.value;
    sceneData[Definitions.photonSearchRadius] = photonSearchRadiusSpinBox.floatValue;
    sceneData[Definitions.radiusReductionRate] = radiusReductionRateSpinBox.floatValue;
    sceneData[Definitions.kNearestNeighbor] = kNearestNeighborSpinBox.value;
    sceneData[Definitions.kernelType] = kernelComboBox.currentText;

    return sceneData;
  }

  function setSceneData(sceneData) {
    numOfPhotonsSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.numOfPhotons);
    photonSearchRadiusSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.photonSearchRadius);
    radiusReductionRateSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.radiusReductionRate);
    kNearestNeighborSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.kNearestNeighbor);
    kernelComboBox.currentIndex = kernelComboBox.find(
        Definitions.getProperty(sceneData, Definitions.kernelType));

    lightSampler.setSceneData(sceneData);
  }
}
