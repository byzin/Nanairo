/*!
  \file NTransformationTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: transformation_tab_item
  width: 422
  height: 214

  property bool scalingButtonEnabled: true
  property var currentObjectItem: null
  property ListModel transformationModel: null

  readonly property int marginSize: 3

  Component {
    id: transformation_delegate
    Loader {
      property color backgroundColor: Qt.darker(transformation_tab_item.color, 1.05)
      property ListView transformationList: transformation_list
      property var transformationItem: transformationModel.get(index)

      source: getTransformationItem(model.type)
      width: transformation_list_frame.width - 2 * marginSize
    }
  }

  Rectangle {
    id: transformation_list_frame
    x: 8
    width: 406
    color: transformation_tab_item.color
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 35
    anchors.top: parent.top
    anchors.topMargin: 8
    border.color: Qt.darker(color, 1.2)
    border.width: 1

    ListView {
      id: transformation_list
      anchors.fill: parent
      clip: true
      delegate: transformation_delegate
      model: transformationModel

      leftMargin: marginSize 
      rightMargin: marginSize
      topMargin: marginSize
      bottomMargin: marginSize
      spacing: 1.0
      verticalLayoutDirection: ListView.BottomToTop
    }
  }

  Component {
    id: transformation_button_style
    NImageButtonStyle {
      backgroundColor: transformation_tab_item.color
    }
  }

  Button {
    id: add_tanslation_button
    x: 125
    y: 185
    width: 24
    height: 24
    style: transformation_button_style

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Translation")
    iconSource: enabled ? "image/translation.png"
                        : "image/translation_disabled.png"

    onClicked: addTranslation()
  }

  Button {
    id: add_scaling_button
    x: 155
    y: 185
    width: 24
    height: 24
    style: transformation_button_style
    enabled: scalingButtonEnabled

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Scaling")
    iconSource: enabled ? "image/scaling.png"
                        : "image/scaling_disabled.png"

    onClicked: addScaling()
  }

  Button {
    id: add_rotation_button
    x: 185
    y: 185
    width: 24
    height: 24
    style: transformation_button_style

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Rotation")
    iconSource: enabled ? "image/rotation.png"
                        : "image/rotation_disabled.png"

    onClicked: addRotation()
  }

  Button {
    id: up_button
    x: 215
    y: 185
    width: 24
    height: 24
    style: transformation_button_style
    enabled: (transformationModel != null) &&
             (transformation_list.currentIndex != -1) &&
             (transformation_list.currentIndex < transformation_list.count - 1)

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Up")
    iconSource: enabled ? "image/up.png"
                        : "image/up_disabled.png"

    onClicked: upItem()
  }

  Button {
    id: down_button
    x: 245
    y: 185
    width: 24
    height: 24
    style: transformation_button_style
    enabled: (transformationModel != null) &&
             (0 < transformation_list.currentIndex)

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Down")
    iconSource: enabled ? "image/down.png"
                        : "image/down_disabled.png"

    onClicked: downItem()
  }

  Button {
    id: delete_button
    x: 275
    y: 185
    width: 24
    height: 24
    style: transformation_button_style
    enabled: (transformationModel != null) &&
             (transformation_list.currentIndex != -1)

    text: qsTr("")
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 5
    iconName: qsTr("Delete")
    iconSource: enabled ? "image/delete.png"
                        : "image/delete_disabled.png"

    onClicked: deleteItem()
  }

  ListModel {
    id: default_transformation_model_list

    // Translation
    ListElement {
      active: true
      x: 0.0
      y: 0.0
      z: 0.0
    }

    // Scaling
    ListElement {
      active: true
      x: 1.0
      y: 1.0
      z: 1.0
    }

    // Rotation
    ListElement {
      active: true
      angle: 0.0
    }
  }

  Component.onCompleted: {
    // Translation
    default_transformation_model_list.get(0).type = Keyword.translation
    // Scaling
    default_transformation_model_list.get(1).type = Keyword.scaling
    // Rotation
    default_transformation_model_list.get(2).type = Keyword.rotation
    default_transformation_model_list.get(2).axis = Keyword.x
    default_transformation_model_list.get(2).unit = Keyword.degreeUnit
  }

//  function isIdentityMatrix(m) {
//    return ((m.m11 == 1.0) && (m.m12 == 0.0) && (m.m13 == 0.0) && (m.m14 == 0.0) &&
//            (m.m21 == 0.0) && (m.m22 == 1.0) && (m.m23 == 0.0) && (m.m24 == 0.0) &&
//            (m.m31 == 0.0) && (m.m32 == 0.0) && (m.m33 == 1.0) && (m.m34 == 0.0) &&
//            (m.m41 == 0.0) && (m.m42 == 0.0) && (m.m43 == 0.0) && (m.m44 == 1.0))
//  }

  function addMatrix(item, matrix) {
    var transformationModel = item.transformationModel
    // Decomposite the transformation matrix
    // Translation
    var translation = [matrix.m14, matrix.m24, matrix.m34]
    // Rotation
    var rotationX = Math.asin(matrix.m32)
    var rotationY = 0.0
    var rotationZ = 0.0
    if (rotationX < Math.PI * 0.5) {
      if (Math.PI * 0.5 < rotationX) {
        rotationY = Math.atan2(-matrix.m12, matrix.m22)
        rotationZ = Math.atan2(-matrix.m31, matrix.m33)
      }
      else {
        rotationY = -Math.atan2(-matrix.m13, matrix.m11)
      }
    }
    else {
      rotationY = Math.atan2(matrix.m13, matrix.m11)
    }
//    var rotationX = Math.atan2(matrix.m32, matrix.m33)
//    var t = Math.sqrt(matrix.m32 * matrix.m32 + matrix.m33 * matrix.m33)
//    var rotationY = Math.atan2(-matrix.m31, t)
//    var rotationZ = Math.atan2(matrix.m21, matrix.m11)

    // Add the transformations
    // Translation
    if (translation[0] != 0.0 && translation[1] != 0.0 && translation[2] != 0.0) {
      transformationModel.append(default_transformation_model_list.get(0))
      var item = transformationModel.get(transformationModel.count - 1)
      item.x = translation[0]
      item.y = translation[1]
      item.z = translation[2]
    }
    // Rotation Z
    if (rotationZ != 0.0) {
      transformationModel.append(default_transformation_model_list.get(2))
      var item = transformationModel.get(transformationModel.count - 1)
      item.axis = Keyword.z
      item.angle = rotationZ
      item.unit = Keyword.radianUnit
    }
    // Rotation y
    if (rotationY != 0.0) {
      transformationModel.append(default_transformation_model_list.get(2))
      var item = transformationModel.get(transformationModel.count - 1)
      item.axis = Keyword.y
      item.angle = rotationY
      item.unit = Keyword.radianUnit
    }
    // Rotation x
    if (rotationX != 0.0) {
      transformationModel.append(default_transformation_model_list.get(2))
      var item = transformationModel.get(transformationModel.count - 1)
      item.axis = Keyword.x
      item.angle = rotationX
      item.unit = Keyword.radianUnit
    }
  }

  function addTransformation(transformationModel, type) {
    switch (type) {
     case Keyword.translation:
      transformationModel.append(default_transformation_model_list.get(0))
      break
     case Keyword.scaling:
      transformationModel.append(default_transformation_model_list.get(1))
      break
     case Keyword.rotation:
      transformationModel.append(default_transformation_model_list.get(2))
      break
     default:
      break
    }
  }

  function addTranslation() {
    transformationModel.append(default_transformation_model_list.get(0))
  }

  function addScaling() {
    transformationModel.append(default_transformation_model_list.get(1))
  }

  function addRotation() {
    transformationModel.append(default_transformation_model_list.get(2))
  }

  function getTransformationItem(type) {
    var transformation_item = "TransformationItems/"
    switch (type) {
     case Keyword.translation:
      transformation_item += "NTranslationItem.qml"
      break
     case Keyword.scaling:
      transformation_item += "NScalingItem.qml"
      break
     case Keyword.rotation:
      transformation_item += "NRotationItem.qml"
      break
     default:
      transformation_item = ""
      break
    }
    return transformation_item
  }

  function upItem() {
    var index = transformation_list.currentIndex
    transformationModel.move(index, index + 1, 1)
  }

  function downItem() {
    var index = transformation_list.currentIndex
    transformationModel.move(index, index - 1, 1)
  }

  function deleteItem() {
    var index = transformation_list.currentIndex
    transformationModel.remove(index, 1)
  }

  onCurrentObjectItemChanged: {
    if (currentObjectItem != null)
      transformationModel = currentObjectItem.transformationModel
    else
      transformationModel = null
  }

  function loadTransformationSettings(item, prefix) {
    switch (item.type) {
     case Keyword.translation:
      item.x = scene_settings.realValue(prefix + Keyword.x, 1.0)
      item.y = scene_settings.realValue(prefix + Keyword.y, 1.0)
      item.z = scene_settings.realValue(prefix + Keyword.z, 1.0)
      break
     case Keyword.scaling:
      item.x = scene_settings.realValue(prefix + Keyword.x, 0.0)
      item.y = scene_settings.realValue(prefix + Keyword.y, 0.0)
      item.z = scene_settings.realValue(prefix + Keyword.z, 0.0)
      break
     case Keyword.rotation:
      item.axis = scene_settings.stringValue(prefix + Keyword.axis, Keyword.x)
      item.angle = scene_settings.realValue(prefix + Keyword.angle, 0.0)
      item.unit = scene_settings.stringValue(prefix + Keyword.unit, 
                                             Keyword.degreeUnit)
      break
     default:
      break
    }
  }

  function saveTransformationSettings(item, prefix) {
    switch (item.type) {
     case Keyword.translation:
     case Keyword.scaling:
      scene_settings.setRealValue(prefix + Keyword.x, item.x)
      scene_settings.setRealValue(prefix + Keyword.y, item.y)
      scene_settings.setRealValue(prefix + Keyword.z, item.z)
      break
     case Keyword.rotation:
      scene_settings.setStringValue(prefix + Keyword.axis, item.axis)
      scene_settings.setRealValue(prefix + Keyword.angle, item.angle)
      scene_settings.setStringValue(prefix + Keyword.unit, item.unit)
      break
     default:
      break
    }
  }

  function loadSceneSettings(item, p) {
    var prefix = p + Keyword.transformation + "/"
    var transformationModel = item.transformationModel
    var count = scene_settings.intValue(prefix + Keyword.count, 1)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var type = scene_settings.stringValue(p + Keyword.type, Keyword.translation)
      addTransformation(transformationModel, type)
      var item = transformationModel.get(i)
      item.active = scene_settings.booleanValue(p + Keyword.active, true)
      loadTransformationSettings(item, p)
    }
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.transformation + "/"
    var transformationModel = item.transformationModel
    var count = transformationModel.count
    scene_settings.setIntValue(prefix + Keyword.count, count)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var item = transformationModel.get(i)
      scene_settings.setStringValue(p + Keyword.type, item.type)
      scene_settings.setBooleanValue(p + Keyword.active, item.active)
      saveTransformationSettings(item, p)
    }
  }
}
