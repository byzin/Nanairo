/*!
  \file NColorSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../Items"
import "DenoiserItems"
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
      title: "color mode"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NButton {
          id: colorModeButton

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          text: Definitions.rgb

          onClicked: text = (text == Definitions.rgb) ? Definitions.spectra
                                                      : Definitions.rgb
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "wavelength sampler"
      color: settingView.background.color
      enabled: colorModeButton.text == Definitions.spectra
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: wavelengthSamplerComboBox 

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 0
          model: [Definitions.regularSampling,
                  Definitions.randomSampling,
                  Definitions.stratifiedSampling]
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "color space"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: colorSpaceComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 0
          model: [Definitions.sRgbD65,
                  Definitions.sRgbD50,
                  Definitions.adobeRgbD65,
                  Definitions.adobeRgbD50]
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "tonemapping"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: tonemappingComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 0
          model: [Definitions.reinhard,
                  Definitions.filmic,
                  Definitions.uncharted2Filmic]
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "gamma   "
          }

          NFloatSpinBox {
            id: gammaSpinBox

            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            floatFrom: 0.0001
            floatTo: realMax
            floatValue: 1.0
          }
        }

        RowLayout {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop

          NLabel {
            font.family: nanairoManager.getDefaultFixedFontFamily()
            text: "exposure"
          }

          NFloatSpinBox {
            id: exposureSpinBox

            Layout.fillWidth: true
            Layout.preferredHeight: Definitions.defaultSettingItemHeight
            floatFrom: 0.0001
            floatTo: realMax
            floatValue: 1.0
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
      title: "denoising"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NCheckBox {
          id: enableDenoisingCheckBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          checked: false
          text: "Enable denoising"
        }

        NComboBox {
          id: denoiserTypeComboBox

          enabled: enableDenoisingCheckBox.checked
          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: 0
          model: [Definitions.bayesianCollaborativeDenoiser]
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      enabled: enableDenoisingCheckBox.checked
      title: "denoising parameters"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: denoiserItemLayout

        anchors.fill: parent
        currentIndex: denoiserTypeComboBox.currentIndex

        NBayesianCollaborativeDenoiserItem {
          id: bayesianCollaborativeDenoiserItem
        }

        onCurrentIndexChanged: {
          if (settingView.isEditMode) {
            var denoiserView = children[currentIndex];
            denoiserView.initSceneData();
          }
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < denoiserItemLayout.children.length; ++i) {
          var denoiserView = denoiserItemLayout.children[i];
          denoiserView.background.color = background.color;
        }
      }
    }
  }

  function getSceneData() {
    var denoiserView = denoiserItemLayout.children[denoiserTypeComboBox.currentIndex];
    var sceneData = denoiserView.getSceneData();

    sceneData[Definitions.colorMode] = colorModeButton.text;
    sceneData[Definitions.wavelengthSampling] = wavelengthSamplerComboBox.currentText;
    sceneData[Definitions.colorSpace] = colorSpaceComboBox.currentText;
    sceneData[Definitions.gamma] = gammaSpinBox.floatValue;
    sceneData[Definitions.exposure] = exposureSpinBox.floatValue;
    sceneData[Definitions.toneMapping] = tonemappingComboBox.currentText;
    sceneData[Definitions.enableDenoising] = enableDenoisingCheckBox.checked;
    sceneData[Definitions.denoiserType] = denoiserTypeComboBox.currentText;

    return sceneData;
  }

  function setSceneData(sceneData) {
    colorModeButton.text =
        Definitions.getProperty(sceneData, Definitions.colorMode);
    wavelengthSamplerComboBox.currentIndex = wavelengthSamplerComboBox.find(
        Definitions.getProperty(sceneData, Definitions.wavelengthSampling));
    colorSpaceComboBox.currentIndex = colorSpaceComboBox.find(
        Definitions.getProperty(sceneData, Definitions.colorSpace));
    gammaSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.gamma);
    exposureSpinBox.floatValue =
        Definitions.getProperty(sceneData, Definitions.exposure);
    tonemappingComboBox.currentIndex = tonemappingComboBox.find(
        Definitions.getProperty(sceneData, Definitions.toneMapping));
    enableDenoisingCheckBox.checked =
        Definitions.getProperty(sceneData, Definitions.enableDenoising);
    denoiserTypeComboBox.currentIndex = denoiserTypeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.denoiserType));

    var denoiserView = denoiserItemLayout.children[denoiserTypeComboBox.currentIndex];
    denoiserView.setSceneData(sceneData);
  }
}
