/*!
  \file NColorTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import "keyword.js" as Keyword

Rectangle {
  id: color_tab_item
  width: 640
  height: 274
  
  readonly property bool defaultRgbRendering: false
  readonly property real defaultGamma: 2.2

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  Component {
    id: checkbox_style
    NCheckBoxStyle {
    }
  }

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: color_tab_item.color
      fontPixelSize: 10
    }
  }

  Label {
    id: spectra_label
    x: 8
    y: 8
    text: qsTr("Spectra")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  CheckBox {
    id: rgb_rendering_checkbox
    x: 16
    y: 26
    text: qsTr("RGB rendering")
    style: checkbox_style
  }

  Label {
    id: standard_observer_label
    x: 16
    y: 54
    text: qsTr("Standard observer")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  ComboBox {
    id: standard_observer_combobox
    x: 16
    y: 68
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.cie2Deg,
            Keyword.cie10Deg]
  }

  Label {
    id: standard_illumination_label
    x: 16
    y: 99
    text: qsTr("Standard illumination")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  ComboBox {
    id: standard_illumination_combobox
    x: 16
    y: 113
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.cieD65,
            Keyword.cieA]
  }

  Label {
    id: wavelength_sampling_label
    x: 16
    y: 148
    text: qsTr("Wavelength sampling")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  ComboBox {
    id: wavelength_sampling_combobox
    x: 16
    y: 162
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.regularSampling,
            Keyword.randomSampling,
            Keyword.stratifiedSampling,
            Keyword.lightsBasedImportanceSampling]
  }

  Rectangle {
    id: separator1 
    x: 159
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
  }

  Label {
    id: display_label
    x: 168
    y: 8
    text: qsTr("Display")
    verticalAlignment: Text.AlignVCenter
    font.bold: true
    textFormat: Text.PlainText
  }

  Label {
    id: color_space_label
    x: 176
    y: 27
    text: qsTr("Color space")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  ComboBox {
    id: color_space_combobox
    x: 176
    y: 44
    width: 120
    height: 20
    style: combobox_style
    
    model: [Keyword.sRgbD65,
            Keyword.sRgbD50,
            Keyword.adobeRgbD65,
            Keyword.adobeRgbD50]
  }

  SpinBox {
    id: gamma_spinbox 
    x: 176
    y: 70
    width: 130
    height: 20
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 99.9999999
    minimumValue:  0.0000001
    prefix: qsTr("Gamma") + ": "
    decimals: 7
  }

  Rectangle {
    id: separator2 
    x: 319
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
  }

  Label {
    id: tone_mapping_label
    x: 328
    y: 8
    text: qsTr("ToneMapping")
    verticalAlignment: Text.AlignVCenter
    font.bold: true
    textFormat: Text.PlainText
  }

  ComboBox {
    id: tone_mapping_combobox
    x: 336
    y: 27
    width: 130
    height: 20
    style: combobox_style
    model: [Keyword.reinhard,
            Keyword.modifiedReinhard,
            Keyword.filmic,
            Keyword.uncharted2Filmic]
  }

  Rectangle {
    id: separator3
    x: 479
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
  }

  function loadSceneSettings() {
    var prefix = Keyword.color + "/"
    rgb_rendering_checkbox.checked =
        scene_settings.booleanValue(prefix + Keyword.rgbRendering, 
                                    defaultRgbRendering)
    standard_observer_combobox.currentIndex = standard_observer_combobox.find(
        scene_settings.stringValue(prefix + Keyword.standardObserver, 
                                   Keyword.cie2Deg))
    standard_illumination_combobox.currentIndex = standard_illumination_combobox.find(
        scene_settings.stringValue(prefix + Keyword.standardIllumination, 
                                   Keyword.cieD65))
    wavelength_sampling_combobox.currentIndex = wavelength_sampling_combobox.find(
        scene_settings.stringValue(prefix + Keyword.wavelengthSampling, 
                                   Keyword.stratifiedSampling))
    color_space_combobox.currentIndex = color_space_combobox.find(
        scene_settings.stringValue(prefix + Keyword.colorSpace, 
        Keyword.sRgbD65))
    gamma_spinbox.value =
        scene_settings.realValue(prefix + Keyword.gamma, 
                                 defaultGamma)
    tone_mapping_combobox.currentIndex = tone_mapping_combobox.find(
        scene_settings.stringValue(prefix + Keyword.toneMapping, 
                                   Keyword.reinhard))
  }

  function saveSceneSettings() {
    var prefix = Keyword.color + "/"
    scene_settings.setBooleanValue(prefix + Keyword.rgbRendering,
                                   rgb_rendering_checkbox.checked)
    scene_settings.setStringValue(prefix + Keyword.standardObserver,
                                  standard_observer_combobox.currentText)
    scene_settings.setStringValue(prefix + Keyword.standardIllumination,
                                  standard_illumination_combobox.currentText)
    scene_settings.setStringValue(prefix + Keyword.wavelengthSampling,
                                  wavelength_sampling_combobox.currentText)
    scene_settings.setStringValue(prefix + Keyword.colorSpace,
                                  color_space_combobox.currentText)
    scene_settings.setRealValue(prefix + Keyword.gamma,
                                gamma_spinbox.value)
    scene_settings.setStringValue(prefix + Keyword.toneMapping,
                                  tone_mapping_combobox.currentText)
  }
}
