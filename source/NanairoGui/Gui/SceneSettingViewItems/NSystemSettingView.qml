/*!
  \file NSystemSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../Items"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property bool isEditMode: true

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "num of threads"
    }

    NSpinBox {
      id: numOfThreadsSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1
      to: 128
      value: 1
    }

    NButton {
      Layout.preferredHeight: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      text: "auto"

      onClicked: numOfThreadsSpinBox.value = nanairoManager.getIdealThreadCount()
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "random seed"
    }

    NSpinBox {
      id: randomSeedSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 0
      to: Definitions.intMax
      value: 0
    }

    NButton {
      Layout.preferredHeight: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      text: "random"

      onClicked: randomSeedSpinBox.value = nanairoManager.generateSeedRandomly()
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "image resolution"
    }

    RowLayout {

      Layout.fillWidth: true

      NLabel {
        Layout.preferredWidth: Definitions.defaultBlockSize
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

      Layout.fillWidth: true

      NLabel {
        Layout.preferredWidth: Definitions.defaultBlockSize
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

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "terminationCycle"
    }

    NSpinBox {
      id: terminationCycleSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 0
      to: Definitions.intMax
      value: 0
    }

    NLabel {
      text: "terminationTime"
    }

    RowLayout {

      Layout.fillWidth: true

      NSpinBox {
        id: terminationTimeSpinBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        from: 0
        to: Definitions.intMax
        value: 0
      }

      NLabel {
        text: "ms"
      }
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "image saving interval"
    }

    RowLayout {

      Layout.fillWidth: true

      NSpinBox {
        id: savingIntervalTimeSpinBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        from: 0
        to: Definitions.intMax
        value: 0
      }

      NLabel {
        text: "ms"
      }
    }

    RowLayout {

      Layout.fillWidth: true

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
        text: "cycle"
      }
    }

    NCheckBox {
      id: power2CycleSavingCheckBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      checked: false
      text: "2^n cycle saving"
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
    sceneData[Definitions.randomSeed] = randomSeedSpinBox.value;
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
    randomSeedSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.randomSeed);
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
