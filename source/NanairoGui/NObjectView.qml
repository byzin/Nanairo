/*!
  \file NObjectView.qml
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
  id: object_view
  width: 640
  height: 240

  Connections {
    target: manager
    onOutputCameraMatrix: {
      var camera = object_tree_view.getObjectItem(0)
      object_setting_view.addMatrix(camera, matrix)
    }
  }

  NObjectSettingView {
    id: object_setting_view
    width: 422
    height: 240
    enabled: !object_tree_view.isMoveMode
    color: object_view.color
    anchors.left: parent.left
    anchors.leftMargin: 0
    anchors.top: parent.top
    anchors.topMargin: 0
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    anchors.right: parent.right
    anchors.rightMargin: 218
  }

  NObjectTreeView {
    id: object_tree_view
    x: 428
    width: 204
    color: object_view.color
    anchors.top: parent.top
    anchors.topMargin: 8
    anchors.right: parent.right
    anchors.rightMargin: 8
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 37

    onCurrentObjectItemChanged: {
      object_setting_view.setCurrentObjectItem(currentObjectItem)
    }
  }

  Component {
    id: object_button_style
    NImageButtonStyle {
      backgroundColor: object_view.color
    }
  }

  Button {
    id: add_object_buttion
    x: 428
    y: 209
    width: 24
    height: 24
    style: object_button_style
    enabled: !object_tree_view.isMoveMode

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 188
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Add object")
    iconSource: enabled ? "image/add_object.png"
                        : "image/add_object_disabled.png"

    onClicked: object_tree_view.addObject()
  }

  Button {
    id: add_group_button
    x: 458
    y: 209
    width: 24
    height: 24
    style: object_button_style
    enabled: !object_tree_view.isMoveMode

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 158
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Add group")
    iconSource: enabled ? "image/add_group.png"
                        : "image/add_group_disabled.png"

    onClicked: object_tree_view.addGroup()
  }

  Button {
    id: move_object_button
    x: 488
    y: 209
    width: 24
    height: 24
    style: object_button_style
    enabled: object_tree_view.currentObjectItem != null &&
             object_tree_view.currentObjectItem.type != Keyword.cameraObject

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 128
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Move object")
    iconSource: enabled ? "image/move_object.png"
                        : "image/move_object_disabled.png"

    onClicked: object_tree_view.isMoveMode = !object_tree_view.isMoveMode
  }

  Button {
    id: moveout_button
    x: 518
    y: 209
    width: 24
    height: 24
    style: object_button_style
    enabled: false

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 98
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Moveout")
    iconSource: enabled ? "image/moveout_object.png"
                        : "image/moveout_object_disabled.png"
  }

  Button {
    id: up_object_button
    x: 548
    y: 209
    width: 24
    height: 24
    style: object_button_style
//    enabled: object_tree_view.currentObjectItem != null &&
//             !object_tree_view.isMoveMode &&
//             (1 < object_tree_view.currentIndex())
    enabled: false


    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 68
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Up object")
    iconSource: enabled ? "image/up.png"
                        : "image/up_disabled.png"
  }

  Button {
    id: down_object_button
    x: 578
    y: 209
    width: 24
    height: 24
    style: object_button_style
//    enabled: object_tree_view.currentObjectItem != null &&
//             !object_tree_view.isMoveMode &&
//             (object_tree_view.currentIndex() != 0 &&
//              object_tree_view.currentIndex() < object_tree_view.count() - 1)
    enabled: false

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 38
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: qsTr("Down object")
    iconSource: enabled ? "image/down.png"
                        : "image/down_disabled.png"
  }

  Button {
    id: delete_object_button
    x: 608
    y: 209
    width: 24
    height: 24
    style: object_button_style
    enabled: object_tree_view.currentObjectItem != null &&
             !object_tree_view.isMoveMode &&
             (object_tree_view.currentIndex() != 0)

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 8
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 7
    iconName: "Delete object"
    iconSource: enabled ? "image/delete_object.png"
                        : "image/delete_object_disabled.png"
    onClicked: object_tree_view.removeObject(object_tree_view.currentIndex())
  }

  function initialize() {
    object_tree_view.addCamera()
  }

  function setMaterialModel(surfaceModel, emitterModel) {
    object_setting_view.setMaterialModel(surfaceModel, emitterModel)
  }

  function setRemovedSurfaceIndex(index) {
    object_tree_view.setRemovedSurfaceIndex(index)
  }

  function setRemovedEmitterIndex(index) {
    object_tree_view.setRemovedEmitterIndex(index)
  }

  function addObject(type) {
    switch (type) {
     case Keyword.cameraObject:
      object_tree_view.addCamera()
      break
     case Keyword.singleObject:
      object_tree_view.addObject()
      break
     case Keyword.groupObject:
      object_tree_view.addGroup()
      break
     default:
      break
    }
  }

  function validateData(messageList) {
    var objectModel = object_tree_view.getObjectModel()
    var sceneHasLightSource = false
    var count = objectModel.count
    for (var i = 0; i < count; ++i) {
      var item = objectModel.get(i)
      if (item.type == Keyword.singleObject) {
        sceneHasLightSource = sceneHasLightSource || item.isEmissiveObject
        if (item.geometryType == Keyword.meshObject &&
            item.objectFilePath.toString() == "") {
          var text = "[Object: " + item.name + "] Object file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
      }
    }

    if (!sceneHasLightSource) {
      var text = "[Object] This scene doesn't have any light source."
      var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
      messageList.append(message)
    }
  }

  function loadSceneSettings() {
    var prefix = Keyword.object + "/"
    var objectModel = object_tree_view.getObjectModel()
    objectModel.clear()
    var count = scene_settings.intValue(prefix + Keyword.count, 1)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var type = scene_settings.stringValue(p + Keyword.type, Keyword.singleObject)
      addObject(type)
      var item = objectModel.get(i)
      item.name = scene_settings.stringValue(p + Keyword.name, "Object")
      item.treeLevel = scene_settings.intValue(p + Keyword.treeLevel, 0)
      item.visibility = scene_settings.booleanValue(p + Keyword.visibility, true)
      object_setting_view.loadSceneSettings(item, p)
    }
    object_tree_view.setParentIndex()
    object_tree_view.resetItemIndex()
  }

  function saveSceneSettings() {
    var prefix = Keyword.object + "/"
    var objectModel = object_tree_view.getObjectModel()
    var count = objectModel.count
    scene_settings.setIntValue(prefix + Keyword.count, count)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var item = objectModel.get(i)
      scene_settings.setStringValue(p + Keyword.name, item.name)
      scene_settings.setStringValue(p + Keyword.type, item.type)
      scene_settings.setIntValue(p + Keyword.treeLevel, item.treeLevel)
      scene_settings.setBooleanValue(p + Keyword.visibility, item.visibility)
      object_setting_view.saveSceneSettings(item, p)
    }
  }
}
