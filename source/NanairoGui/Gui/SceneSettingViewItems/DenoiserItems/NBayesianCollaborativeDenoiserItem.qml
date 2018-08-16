/*!
  \file NBayesianCollaborativeDenoiserItem.qml
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

NScrollView {
  id: denoiserItem

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "histogram bins"
    }

    NSpinBox {
      id: histogramBinsSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: denoiserItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 3
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "histogram distance threshold"
    }

    NFloatSpinBox {
      id: histogramDistanceThresholdSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: denoiserItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      floatFrom: 0.0001
      floatTo: realMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "patch radius"
    }

    NSpinBox {
      id: patchRadiusSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: denoiserItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 0
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "search window radius"
    }

    NSpinBox {
      id: searchWindowRadiusSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: denoiserItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: patchRadiusSpinBox.value
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "number of scales"
    }

    NSpinBox {
      id:numberOfScalesSpinBox 

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: denoiserItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1
      to: Definitions.intMax
    }
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.histogramBins] = histogramBinsSpinBox.value;
    sceneData[Definitions.histogramDistanceThreshold] = histogramDistanceThresholdSpinBox.floatValue;
    sceneData[Definitions.patchRadius] = patchRadiusSpinBox.value;
    sceneData[Definitions.searchWindowRadius] = searchWindowRadiusSpinBox.value;
    sceneData[Definitions.numberOfScales] = numberOfScalesSpinBox.value;

    return sceneData;
  }

  function initSceneData() {
    histogramBinsSpinBox.value = 16;
    histogramDistanceThresholdSpinBox.floatValue = 0.75;
    patchRadiusSpinBox.value = 1;
    searchWindowRadiusSpinBox.value = 6;
    numberOfScalesSpinBox.value = 2;
  }

  function setSceneData(sceneData) {
    histogramBinsSpinBox.value = 
        Definitions.getProperty(sceneData, Definitions.histogramBins);
    histogramDistanceThresholdSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.histogramDistanceThreshold);
    patchRadiusSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.patchRadius);
    searchWindowRadiusSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.searchWindowRadius);
    numberOfScalesSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.numberOfScales);
  }
}
