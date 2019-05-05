/*!
  \file NMethodSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../Items"
import "MethodItems"
import "../definitions.js" as Definitions

NScrollView {
  id: settingView

  property bool isEditMode: true

  GridLayout {
    columns: 4
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "rendering method"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: methodTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 0
          model: [Definitions.pathTracing,
                  Definitions.lightTracing,
                  Definitions.probabilisticPpm]
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "rendering method parameters"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: methodItemLayout

        anchors.fill: parent
        currentIndex: methodTypeComboBox.currentIndex

        NPathTracingMethodItem {
          id: pathTracingMethodItem
        }

        NLightTracingMethodItem {
          id: lightTracingMethodItem
        }

        NProbabilisticPpmMethodItem {
          id: probabilisticPpmMethodItem
        }

        onCurrentIndexChanged: {
          if (settingView.isEditMode) {
            var methodView = methodItemLayout.children[currentIndex];
            methodView.initSceneData();
          }
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < methodItemLayout.children.length; ++i) {
          var methodView = methodItemLayout.children[i];
          methodView.background.color = background.color;
        }
      }
    }

    NGroupBox {
      title: "ray cast epsilon"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NFloatSpinBox {
          id: rayCastEpsilonSpinBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          decimals: 7
          floatFrom: 0.0
          floatTo: 1.0
          floatValue: 0.0000001
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "russian roulette"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: russianRouletteComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
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

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "length"
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

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }
  }

  function getSceneData() {
    var methodView = methodItemLayout.children[methodTypeComboBox.currentIndex];
    var sceneData = methodView.getSceneData();

    sceneData[Definitions.type] = methodTypeComboBox.currentText;
    sceneData[Definitions.rayCastEpsilon] = rayCastEpsilonSpinBox.floatValue;
    sceneData[Definitions.russianRoulette] = russianRouletteComboBox.currentText;
    sceneData[Definitions.pathLength] = roulettePathLengthSpinBox.value;

    return sceneData;
  }

  function setSceneData(sceneData) {
    methodTypeComboBox.currentIndex = methodTypeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.type));
    rayCastEpsilonSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.rayCastEpsilon);
    russianRouletteComboBox.currentIndex = russianRouletteComboBox.find(
        Definitions.getProperty(sceneData, Definitions.russianRoulette));
    roulettePathLengthSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.pathLength);

    var methodView = methodItemLayout.children[methodTypeComboBox.currentIndex];
    methodView.setSceneData(sceneData);
  }
}
