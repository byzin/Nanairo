/*!
  \file NSystemTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: system_tab_item
  width: 640
  height: 274

  readonly property string defaultSceneName: "DefaultScene"
  readonly property int defaultNumOfThreads: 1
  readonly property int defaultRandomSeed: manager.defaultRandomSeed()
  readonly property int defaultTerminationPass: 0
  readonly property int defaultImageWidth: 640
  readonly property int defaultImageHeight: 480
  readonly property bool defaultSavingSpectraImage: false
  readonly property bool defaultPower2Saving: true
  readonly property int defaultSavingInterval: 10 * 60 * 1000
  readonly property bool defaultOpenCl: false

  Component {
    id: spinbox_button_style
    NSpinBoxButtonStyle {
    }
  }

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: system_tab_item.color
      fontPixelSize: 10
    }
  }

  Component {
    id: checkbox_style
    NCheckBoxStyle {}
  }

  Component {
    id: button_style
    NButtonStyle {
      id: style
      backgroundColor: system_tab_item.color
      fontPixelSize: 10
    }
  }

  TextField {
    id: scene_name_editor
    x: 8
    y: 8
    width: 200
    height: 20
    placeholderText: qsTr("Scene name")
    style: NTextFieldStyle {}
  }

  Label {
    id: thread_label
    x: 8
    y: 34
    text: qsTr("Thread")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: thread_spinbox
    x: 16
    y: 53
    width: 100
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: 1024
    minimumValue: 1
    decimals: 0
  }

  Button {
    id: thread_auto_button
    x: 47
    y: 79
    width: 40
    height: 15
    text: qsTr("Auto")
    style: button_style
    onClicked: thread_spinbox.value = manager.idealThreadCount()
  }

  Label {
    id: random_seed_label
    x: 8
    y: 108
    text: qsTr("Random seed")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: random_seed_spinbox
    x: 16
    y: 127
    width: 100
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: Nanairo.int32Max()
    minimumValue: 0
    decimals: 0
  }

  Button {
    id: random_seed_random_set_button
    x: 31
    y: 153
    width: 70
    height: 15
    text: qsTr("Random set")
    style: button_style
    onClicked: random_seed_spinbox.value = manager.random()
  }

  Label {
    id: termination_pass_label
    x: 8
    y: 181
    text: qsTr("Termination pass")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    font.bold: true
  }

  SpinBox {
    id: termination_pass_spinbox
    x: 16
    y: 200
    width: 100
    height: 20
    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: Nanairo.int32Max()
    minimumValue: 0
    decimals: 0
  }

  Rectangle {
    id: separator1 
    x: 159
    y: 52
    width: 1
    height: 190
    color: Qt.darker(parent.color, 1.1)
    border.width: 0
  }

  Label {
    id: image_resolution_label
    x: 168
    y: 34
    text: qsTr("Image resolution")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    font.bold: true
  }

  SpinBox {
    id: width_resolution_spinbox
    x: 176
    y: 53
    width: 60
    height: 20
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 16
    decimals: 0
  }

  Label {
    id: cross_label
    x: width_resolution_spinbox.x + width_resolution_spinbox.width + 5
    y: 57
    text: qsTr("x")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
  }

  SpinBox {
    id: height_resolution_spinbox
    x: cross_label.x + cross_label.width + 5
    y: 53
    width: 60
    height: 20
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 16
    decimals: 0
  }

  ComboBox {
    id: image_resolution_template
    x: 176
    y: 79
    width: 100
    height: 20
    style: NComboBoxStyle {
      backgroundColor: system_tab_item.color
      label: Label {
        text: qsTr("Template size")
        font.pixelSize: 10
        horizontalAlignment: Text.AlignHLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.PlainText
      }
    }

    model: ["---",
            "QVGA: 320x240",
            "VGA: 640x480",
            "SVGA: 800x600",
            "Quad VGA: 1280x960",
            "HQVGA+: 427x240",
            "HVGAW: 640x360",
            "Quarter HD: 960x540",
            "FWXGA: 1280x720",
            "Full HD: 1920x1080",
            "Wide Quad HD: 2560x1440"]
    currentIndex: 0

    onCurrentIndexChanged: {
      if (image_resolution_template.currentIndex == 0)
        return
      var template_size = image_resolution_template.currentText
      var regex = /.+:\s?(\d+)x(\d+)/
      template_size.match(regex)
      width_resolution_spinbox.value = Number(RegExp.$1)
      height_resolution_spinbox.value = Number(RegExp.$2)
      image_resolution_template.currentIndex = 0
    }
  }

  Label {
    id: spectra_image_label
    x: 168
    y: 108
    text: qsTr("Spectra image")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    font.bold: true
  }

  CheckBox {
    id: saving_spectra_image_checkbox
    x: 176
    y: 127
    height: 20
    text: qsTr("Save spectra image")
    style: checkbox_style
  }

  Label {
    id: ldr_image_label
    x: 168
    y: 159
    text: qsTr("LDR image")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    font.bold: true
  }

  ComboBox {
    id: ldr_image_format_combobox
    x: 176
    y: 178
    width: 100
    height: 20
    style: combobox_style

    model: [Keyword.png,
            Keyword.bmp,
            Keyword.ppm]
  }

  Rectangle {
    id: separator2
    x: 319
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
    border.width: 0
  }

  Label {
    id: image_saving_label
    x: 328
    y: 34
    text: qsTr("Image saving")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    font.bold: true
  }

  CheckBox {
    id: power2_saving_checkbox
    x: 336
    y: 53
    height: 20
    text: qsTr("Power of 2 pass")
    style: checkbox_style
  }

  SpinBox {
    id: saving_interval_spinbox
    x: 336
    y: 79
    width: 140
    height: 20
    suffix: "ms"
    prefix: qsTr("Interval") + ": "

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: Nanairo.int32Max()
    minimumValue: 1
    decimals: 0
  }

  Rectangle {
    id: separator3
    x: 479
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
    border.width: 0
  }

  Label {
    id: open_cl_label
    x: 488
    y: 34
    text: qsTr("OpenCL")
    verticalAlignment: Text.AlignVCenter
    font.bold: true
    textFormat: Text.PlainText
  }

  CheckBox {
    id: open_cl_checkbox
    enabled: false
    x: 496
    y: 53
    height: 20
    text: qsTr("OpenCL")
    style: checkbox_style
  }

  ComboBox {
    id: open_cl_combobox
    enabled: false
    x: 496
    y: 79
    width: 120
    height: 20

    style: combobox_style

    model: [Keyword.openClSingleGpu,
            Keyword.openClMultipleGpus,
            Keyword.openClCpu]
  }

  function getImageWidth() {
    return width_resolution_spinbox.value
  }

  function getImageHeight() {
    return height_resolution_spinbox.value
  }

  function getSceneName() {
    return scene_name_editor.text
  }

  function loadSceneSettings() {
    var prefix = Keyword.system + "/"
    scene_name_editor.text = 
        scene_settings.stringValue(prefix + Keyword.sceneName, defaultSceneName)
    thread_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.numOfThreads, defaultNumOfThreads)
    random_seed_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.randomSeed, defaultRandomSeed)
    termination_pass_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.terminationPass, 
                                defaultTerminationPass)
    width_resolution_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.imageWidthResolution, 
                                defaultImageWidth)
    height_resolution_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.imageHeightResolution, 
                                defaultImageHeight)
    saving_spectra_image_checkbox.checked = 
        scene_settings.booleanValue(prefix + Keyword.enableToSaveSpectraImage, 
                                    defaultSavingSpectraImage)
    ldr_image_format_combobox.currentIndex = ldr_image_format_combobox.find(
        scene_settings.stringValue(prefix + Keyword.ldrImageFormat, Keyword.png))
    power2_saving_checkbox.checked = 
        scene_settings.booleanValue(prefix + Keyword.power2Saving, 
                                    defaultPower2Saving)
    saving_interval_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.savingInterval, 
                                defaultSavingInterval)
    open_cl_checkbox.checked = 
        scene_settings.booleanValue(prefix + Keyword.openClRendering, defaultOpenCl)
    open_cl_combobox.currentIndex = open_cl_combobox.find(
        scene_settings.stringValue(prefix + Keyword.openClType, 
                                   Keyword.openClSingleGpu))
  }

  function saveSceneSettings() {
    var prefix = Keyword.system + "/"
    scene_settings.setStringValue(prefix + Keyword.sceneName, 
                                  scene_name_editor.text)
    scene_settings.setIntValue(prefix + Keyword.numOfThreads, 
                               thread_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.randomSeed, 
                               random_seed_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.terminationPass, 
                               termination_pass_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.imageWidthResolution, 
                               width_resolution_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.imageHeightResolution, 
                               height_resolution_spinbox.value)
    scene_settings.setBooleanValue(prefix + Keyword.enableToSaveSpectraImage, 
                                   saving_spectra_image_checkbox.checked)
    scene_settings.setStringValue(prefix + Keyword.ldrImageFormat, 
                                  ldr_image_format_combobox.currentText)
    scene_settings.setBooleanValue(prefix + Keyword.power2Saving, 
                                   power2_saving_checkbox.checked)
    scene_settings.setIntValue(prefix + Keyword.savingInterval, 
                               saving_interval_spinbox.value)
    scene_settings.setBooleanValue(prefix + Keyword.openClRendering, 
                                   open_cl_checkbox.checked)
    scene_settings.setStringValue(prefix + Keyword.openClType, 
                                  open_cl_combobox.currentText)
  }
}
