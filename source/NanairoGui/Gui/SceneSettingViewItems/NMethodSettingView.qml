/*!
  \file NMethodSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../Items"
import "MethodItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property bool isEditMode: true

  RowLayout {
    id: row1

    spacing: Definitions.defaultSettingItemColumnInterval

    ColumnLayout {
      id: column1

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: Definitions.defaultItemSpace

      NLabel {
        text: "type"
      }

      NComboBox {
        id: typeComboBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        currentIndex: 0
        model: [Definitions.pathTracing,
                Definitions.lightTracing,
                Definitions.probabilisticPpm]

        onCurrentIndexChanged: {
          if (settingView.isEditMode) {
            var methodView = methodItemLayout.children[currentIndex];
            methodView.initSceneData();
          }
        }
      }
    }

    ColumnLayout {
      id: column2

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: column1.spacing

      NLabel {
        text: "ray cast epsilon"
      }

      NFloatSpinBox {
        id: rayCastEpsilonSpinBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        decimals: 7
        floatFrom: 0.0
        floatTo: 1.0
        floatValue: 0.0000001
      }
    }

    ColumnLayout {
      id: column3

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: column1.spacing

      NLabel {
        text: "russian roulette"
      }

      NComboBox {
        id: russianRouletteComboBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        currentIndex: 0
        model: [Definitions.rouletteMaxReflectance,
                Definitions.rouletteAverageReflectance,
                Definitions.roulettePathLength]

        onCurrentIndexChanged: {
          if (settingView.isEditMode)
            roulettePathLengthSpinBox.initItem();
        }
      }
    }

    ColumnLayout {
      id: column4

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: column1.spacing

      NLabel {
        enabled: roulettePathLengthSpinBox.enabled
        text: "roulette path length"
      }

      NSpinBox {
        id: roulettePathLengthSpinBox

        enabled: russianRouletteComboBox.currentText == Definitions.roulettePathLength
        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        from: 1
        to: Definitions.intMax
        value: 3

        function initItem() {
          value = 3;
        }
      }
    }
  }

  StackLayout {
    id: methodItemLayout

    anchors.fill: parent
    anchors.topMargin: (row1.y + row1.height) + Definitions.defaultBlockSize
    currentIndex: typeComboBox.currentIndex

    NPathTracingMethodItem {
      id: pathTracingMethodItem
    }

    NLightTracingMethodItem {
      id: lightTracingMethodItem
    }

    NProbabilisticPpmMethodItem {
      id: probabilisticPpmMethodItem
    }
  }

  function getSceneData() {
    var methodView = methodItemLayout.children[typeComboBox.currentIndex];
    var sceneData = methodView.getSceneData();

    sceneData[Definitions.type] = typeComboBox.currentText;
    sceneData[Definitions.rayCastEpsilon] = rayCastEpsilonSpinBox.floatValue;
    sceneData[Definitions.russianRoulette] = russianRouletteComboBox.currentText;
    sceneData[Definitions.pathLength] = roulettePathLengthSpinBox.value;

    return sceneData;
  }

  function setSceneData(sceneData) {
    typeComboBox.currentIndex = typeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.type));
    rayCastEpsilonSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.rayCastEpsilon);
    russianRouletteComboBox.currentIndex = russianRouletteComboBox.find(
        Definitions.getProperty(sceneData, Definitions.russianRoulette));
    roulettePathLengthSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.pathLength);

    var methodView = methodItemLayout.children[typeComboBox.currentIndex];
    methodView.setSceneData(sceneData);
  }
}
