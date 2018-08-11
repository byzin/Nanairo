/*!
  \file NColorSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
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
      text: "color mode"
    }

    NButton {
      id: colorModeButton

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      text: Definitions.rgb

      onClicked: text = (text == Definitions.rgb) ? Definitions.spectra
                                                  : Definitions.rgb
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      enabled: wavelengthSamplerComboBox.enabled
      text: "wavelength sampler"
    }

    NComboBox {
      id: wavelengthSamplerComboBox 

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      enabled: colorModeButton.text == Definitions.spectra
      currentIndex: 0
      model: [Definitions.regularSampling,
              Definitions.randomSampling,
              Definitions.stratifiedSampling]
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "color space"
    }

    NComboBox {
      id: colorSpaceComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: 0
      model: [Definitions.sRgbD65,
              Definitions.sRgbD50,
              Definitions.adobeRgbD65,
              Definitions.adobeRgbD50]
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "gamma"
    }

    NFloatSpinBox {
      id: gammaSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      floatFrom: 0.0001
      floatTo: realMax
      floatValue: 1.0
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
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

    NLabel {
      text: "tonemapping"
    }

    NComboBox {
      id: tonemappingComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: 0
      model: [Definitions.reinhard,
              Definitions.filmic,
              Definitions.uncharted2Filmic]
    }
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.colorMode] = colorModeButton.text;
    sceneData[Definitions.wavelengthSampling] = wavelengthSamplerComboBox.currentText;
    sceneData[Definitions.colorSpace] = colorSpaceComboBox.currentText;
    sceneData[Definitions.gamma] = gammaSpinBox.floatValue;
    sceneData[Definitions.exposure] = exposureSpinBox.floatValue;
    sceneData[Definitions.toneMapping] = tonemappingComboBox.currentText;

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
  }
}
