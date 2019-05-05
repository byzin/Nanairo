/*!
  \file NSystemSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../Items"
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
      title: "threads"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NSpinBox {
          id: numOfThreadsSpinBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          from: 1
          to: 128
          value: 1
        }

        NButton {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.preferredHeight: Definitions.defaultBlockSize
          text: "auto"

          onClicked: numOfThreadsSpinBox.value = nanairoManager.getIdealThreadCount()
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "sampler"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: samplerTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 2
          model: [Definitions.pcgSampler,
                  Definitions.xoshiroSampler,
                  Definitions.cmjSampler]
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "seed"
          }

          NSpinBox {
            id: samplerSeedSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 0
            to: Definitions.intMax
            value: 0
          }
        }

        NButton {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.preferredHeight: Definitions.defaultBlockSize
          text: "random"

          onClicked: samplerSeedSpinBox.value = nanairoManager.generateSeedRandomly()
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "image resolution"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "w"
          }

          NSpinBox {
            id: widthResolutionSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 256
            to: Definitions.intMax
            value: from
          }
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "h"
          }

          NSpinBox {
            id: heightResolutionSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 256
            to: Definitions.intMax
            value: from
          }
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "image saving interval"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NSpinBox {
            id: savingIntervalCycleSpinBox

            enabled: false // \todo Implement image saving cycle
            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 0
            to: Definitions.intMax
            value: 0
          }

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "cycle"
          }
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NSpinBox {
            id: savingIntervalTimeSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 0
            to: Definitions.intMax
            value: 0
          }

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "ms   "
          }
        }

        NCheckBox {
          id: power2CycleSavingCheckBox

          Layout.alignment: Qt.AlignLeft | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          checked: false
          text: "2^n cycle"
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "termination condition"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NSpinBox {
            id: terminationCycleSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 0
            to: Definitions.intMax
            value: 0
          }

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "cycle"
          }
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NSpinBox {
            id: terminationTimeSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            from: 0
            to: Definitions.intMax
            value: 0
          }

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "ms   "
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

  function getImageResolution() {
    var imageResolution = [widthResolutionSpinBox.value,
                           heightResolutionSpinBox.value];
    return imageResolution;
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.numOfThreads] = numOfThreadsSpinBox.value;
    sceneData[Definitions.samplerType] = samplerTypeComboBox.currentText;
    sceneData[Definitions.samplerSeed] = samplerSeedSpinBox.value;
    var imageResolution = getImageResolution();
    sceneData[Definitions.imageResolution] = imageResolution;
    sceneData[Definitions.terminationCycle] = terminationCycleSpinBox.value;
    sceneData[Definitions.terminationTime] = terminationTimeSpinBox.value;
    sceneData[Definitions.savingIntervalTime] = savingIntervalTimeSpinBox.value;
    sceneData[Definitions.savingIntervalCycle] = savingIntervalCycleSpinBox.value;
    sceneData[Definitions.power2CycleSaving] = power2CycleSavingCheckBox.checked;

    return sceneData;
  }

  function setSceneData(sceneData) {
    numOfThreadsSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.numOfThreads);
    samplerTypeComboBox.currentIndex = samplerTypeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.samplerType));
    samplerSeedSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.samplerSeed);
    var imageResolution =
        Definitions.getProperty(sceneData, Definitions.imageResolution);
    widthResolutionSpinBox.value = imageResolution[0];
    heightResolutionSpinBox.value = imageResolution[1];
    terminationCycleSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.terminationCycle);
    terminationTimeSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.terminationTime);
    savingIntervalTimeSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.savingIntervalTime);
    savingIntervalCycleSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.savingIntervalCycle);
    power2CycleSavingCheckBox.checked =
        Definitions.getProperty(sceneData, Definitions.power2CycleSaving);
  }
}
