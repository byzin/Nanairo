/*!
  \file NObjectTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: object_tab_item
  width: 422
  height: 214

  property url objectFilePath: Qt.resolvedUrl("")
  property string objectFileName: ""
  property var currentObjectItem: null

  Component {
    id: checkbox_style
    NCheckBoxStyle {}
  }

  TextField {
    id: object_name_editor
    x: 8
    y: 8
    width: 141
    height: 20
    placeholderText: qsTr("Object name")
    style: NTextFieldStyle {}

    onEditingFinished: {
      if (currentObjectItem != null)
        currentObjectItem.name = displayText
    }
  }

  Label {
    id: geometry_type_label
    x: 8
    y: 39
    text: qsTr("Geometry type")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  Button {
    id: open_object_button
    x: 16
    y: 89
    width: 125
    height: 20
    enabled: geometry_type_combobox.currentText == Keyword.meshObject

    style: NButtonStyle {
      backgroundColor: object_tab_item.color
    }
    text: (objectFileName == "") ? qsTr("Open object file") : objectFileName

    FileDialog {
      id: open_object_dialog
      title: qsTr("Open object file")
      nameFilters: ["Wavefront file (*.obj)"]
      onAccepted: objectFilePath = fileUrl
    }

    onClicked: open_object_dialog.open()
  }

  CheckBox {
    id: smoothing_checkbox
    x: 16
    y: 115
    width: 125
    height: 20
    enabled: geometry_type_combobox.currentText == Keyword.meshObject
    text: qsTr("Smoothing")
    style: checkbox_style

    onCheckedChanged: {
      if (currentObjectItem != null)
        currentObjectItem.smoothing = checked
    }
  }

  ComboBox {
    id: geometry_type_combobox
    x: 16
    y: 61
    width: 125
    height: 20
    style: NComboBoxStyle {
      backgroundColor: object_tab_item.color
    }

    model: [Keyword.planeObject,
            Keyword.meshObject]
    currentIndex: 0

    onCurrentIndexChanged: {
      if (currentObjectItem != null)
        currentObjectItem.geometryType = geometry_type_combobox.currentText
      if (currentText != Keyword.meshObject) {
        objectFilePath = Qt.resolvedUrl("")
        smoothing_checkbox.checked = false
      }
    }
  }

  Label {
    id: surface_model_label
    x: 168
    y: 39
    text: qsTr("Surface")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  ComboBox {
    id: surface_model_combobox
    x: 176
    y: 61
    width: 125
    height: 20
    currentIndex: 0
    style: NComboBoxStyle {
      backgroundColor: object_tab_item.color
    }
    model: null

    onCurrentIndexChanged: {
      if (currentObjectItem != null)
        currentObjectItem.surfaceIndex = currentIndex
    }
  }

  Label {
    id: emitter_model_label
    x: 170
    y: 89
    text: qsTr("Emitter")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  CheckBox {
    id: emissive_object_checkbox
    x: 176
    y: 108
    width: 125
    height: 20
    text: qsTr("Emissive object")
    style: checkbox_style

    onCheckedChanged: {
      if (currentObjectItem != null)
        currentObjectItem.isEmissiveObject = checked
    }
  }

  ComboBox {
    id: emitter_model_combobox
    x: 176
    y: 134
    width: 125
    height: 20
    enabled: emissive_object_checkbox.checked
    currentIndex: 0
    style: NComboBoxStyle {
      backgroundColor: object_tab_item.color
    }
    model: null

    onCurrentIndexChanged: {
        if (currentObjectItem != null)
            currentObjectItem.emitterIndex = currentIndex
    }
  }

  onObjectFilePathChanged: {
      objectFileName = manager.getFileName(objectFilePath)
    if (currentObjectItem != null)
      currentObjectItem.objectFilePath = objectFilePath
  }

  onCurrentObjectItemChanged: {
    if (currentObjectItem != null) {
      object_name_editor.text = currentObjectItem.name
      geometry_type_combobox.currentIndex = getIndex(currentObjectItem.geometryType)
      objectFilePath = currentObjectItem.objectFilePath
      smoothing_checkbox.checked = currentObjectItem.smoothing
      surface_model_combobox.currentIndex = currentObjectItem.surfaceIndex
      emissive_object_checkbox.checked = currentObjectItem.isEmissiveObject
      emitter_model_combobox.currentIndex = currentObjectItem.emitterIndex
    }
  }

  function getIndex(type) {
    return geometry_type_combobox.find(type)
  }

  function isMeshObject(item) {
    return item.geometryType == Keyword.meshObject
  }

  function setMaterialModel(surfaceModel, emitterModel) {
    surface_model_combobox.model = surfaceModel
    emitter_model_combobox.model = emitterModel
  }

  function loadSceneSettings(item, prefix) {
    item.geometryType = scene_settings.stringValue(prefix + Keyword.geometryType, 
                                                   Keyword.planeObject)
    if (isMeshObject(item)) {
      item.objectFilePath = scene_settings.urlValue(prefix + Keyword.objectFilePath,
                                                    Qt.resolvedUrl(""))
      item.smoothing = scene_settings.booleanValue(prefix + Keyword.smoothing, false)
    }
    item.surfaceIndex = scene_settings.intValue(prefix + Keyword.surfaceIndex, 0)
    item.isEmissiveObject = 
        scene_settings.booleanValue(prefix + Keyword.isEmissiveObject, false)
    if (item.isEmissiveObject)
      item.emitterIndex = scene_settings.intValue(prefix + Keyword.emitterIndex, 0)
  }

  function saveSceneSettings(item, prefix) {
    scene_settings.setStringValue(prefix + Keyword.geometryType, item.geometryType)
    if (isMeshObject(item)) {
      scene_settings.setUrlValue(prefix + Keyword.objectFilePath, item.objectFilePath)
      scene_settings.setBooleanValue(prefix + Keyword.smoothing, item.smoothing)
    }
    scene_settings.setIntValue(prefix + Keyword.surfaceIndex, item.surfaceIndex)
    scene_settings.setBooleanValue(prefix + Keyword.isEmissiveObject, 
                                    item.isEmissiveObject)
    if (item.isEmissiveObject)
      scene_settings.setIntValue(prefix + Keyword.emitterIndex, item.emitterIndex)
  }
}
