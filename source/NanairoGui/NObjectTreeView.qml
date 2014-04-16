/*!
  \file NObjectTreeView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Layouts 1.1
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: object_tree_view
  border.color: Qt.darker(color, 1.2)
  border.width: 1

  readonly property int marginSize: 2

  property bool isMoveMode: false
  property var currentObjectItem: null

  ListView {
    id: object_tree
    anchors.fill: parent
    clip: true

    delegate: object_delegate
    model: object_model

    leftMargin: object_tree_view.marginSize
    rightMargin: object_tree_view.marginSize
    topMargin: object_tree_view.marginSize
    bottomMargin: object_tree_view.marginSize

    onCurrentIndexChanged: {
      object_tree_view.currentObjectItem = getObjectItem(currentIndex)
    }
  }

  Component {
    id: object_delegate

    Rectangle {
      id: object_list_item
      enabled: !(isMoveMode && (!isGroupObject(model) || ListView.isCurrentItem))
      visible: (parentItem == null) ? true 
                                    : parentItem.isVisible && parentItem.isExpanded
      color: (ListView.isCurrentItem)
          ? Qt.darker(object_tree_view.color, 1.3)
          : (model.index % 2 == 0) 
              ? object_tree_view.color
              : Qt.darker(object_tree_view.color, 1.05)

      readonly property int button_size: 16

      property var parentItem: getObjectItem(model.parentIndex)

      width: object_tree_view.width - (2 * object_tree_view.marginSize)
      height: (visible) ? button_size : 0

      Row {
        id: item_row

        Component {
          id: item_selectable_area
          MouseArea {
            anchors.fill: parent
            onClicked: {
              if (!object_tree_view.isMoveMode)
                setCurrentObjectIndex(model.index)
            }
            onDoubleClicked: {
              var source = object_tree.currentIndex
              var dest = model.index
              var num = getNumOfChildItems(source)
              if (object_tree_view.isMoveMode && 
                  !(source <= dest && dest <= source + num)) {
                moveObjectItem(source, dest, num + 1)
                setParentIndex()
                object_tree_view.isMoveMode = false
              }
            }
          }
        }

        Rectangle {
          id: indent_space
          width: model.treeLevel * object_list_item.button_size
          height: object_list_item.height
          color: object_list_item.color
          
          Loader {
            anchors.fill: parent
            sourceComponent: item_selectable_area
          }
        }

        Component {
          id: item_button_style
          ButtonStyle {
            background: Rectangle {
              color: object_list_item.color
            }
          }
        }

        Button {
          id: expander_button
          width: object_list_item.button_size
          height: object_list_item.height
          style: item_button_style

          enabled: (model.type == Keyword.groupObject)
          opacity: (model.type == Keyword.groupObject) ? 1.0 : 0.0

          text: qsTr("")
          rotation: ((model.type == Keyword.groupObject) && (model.isExpanded)) 
              ? 90.0 
              : 0.0
          iconName: "Expander"
          iconSource: object_list_item.enabled ? "image/expander.png"
                                               : "image/expander_disabled.png"

          onClicked: getObjectItem(index).isExpanded = !model.isExpanded
        }

        Label {
          id: object_name_label
          width: object_list_item.width - 
               ((2 + model.treeLevel) * object_list_item.button_size)
          height: object_list_item.height
          text: model.name
          horizontalAlignment: Text.AlignLeft
          verticalAlignment: Text.AlignVCenter
          textFormat: Text.PlainText

          Loader {
            anchors.fill: parent
            sourceComponent: item_selectable_area
          }
        }

        Button {
          id: visibility_button
          width: object_list_item.button_size
          height: object_list_item.height
          style: item_button_style

          text: qsTr("")
          iconName: "Visibility"
          iconSource: (model.visibility) 
              ? object_list_item.enabled ? "image/visibility.png" 
                                         : "image/visibility_disabled.png"
              : object_list_item.enabled ? "image/no_visibility.png" 
                                         : "image/no_visibility_disabled.png"

          onClicked: {
            var item = getObjectItem(model.index)
            item.visibility = !item.visibility
          }
        }
      }

      function setCurrentObjectIndex(index) {
        object_tree.currentIndex = index
      }

      onVisibleChanged: getObjectItem(index).isVisible = visible
    }
  }

  ListModel {
    id: object_model
  }

  Component {
    id: transformation_model
    ListModel {}
  }

  ListModel {
    id: default_model_list
    ListElement {name: "Camera"}
    ListElement {name: "Object"}
    ListElement {name: "Group"}
  }

  Component.onCompleted: {
    // General
    for (var i = 0; i < default_model_list.count; ++i) {
      var defaultItem = default_model_list.get(i)
      defaultItem.treeLevel = 0
      defaultItem.parentIndex = -1
      defaultItem.visibility = true
      defaultItem.isVisible = true
      defaultItem.isExpanded = true
    }
    // Camera
    var cameraItem = default_model_list.get(0)
    cameraItem.type = Keyword.cameraObject
    cameraItem.jittering = true
    cameraItem.cameraType = Keyword.pinholeCamera
    cameraItem.angleOfView = 45.0
    // Object
    var objectItem = default_model_list.get(1)
    objectItem.type = Keyword.singleObject
    objectItem.geometryType = Keyword.planeObject
    objectItem.objectFilePath = Qt.resolvedUrl("")
    objectItem.smoothing = false
    objectItem.surfaceIndex = 0
    objectItem.isEmissiveObject = false
    objectItem.emitterIndex = 0
    // Group
    var groupItem = default_model_list.get(2)
    groupItem.type = Keyword.groupObject
  }

  function addCamera() {
    object_model.append(default_model_list.get(0))
    getObjectItem(object_model.count - 1).transformationModel = 
        transformation_model.createObject(parent)
  }

  function addObject() {
    object_model.append(default_model_list.get(1))
    getObjectItem(object_model.count - 1).transformationModel = 
        transformation_model.createObject(parent)
  }

  function addGroup() {
    object_model.append(default_model_list.get(2))
    getObjectItem(object_model.count - 1).transformationModel = 
        transformation_model.createObject(parent)
  }

  function isCameraObject(item) {
    return item.type == Keyword.cameraObject
  }

  function isSingleObject(item) {
    return item.type == Keyword.singleObject
  }

  function isGroupObject(item) {
    return item.type == Keyword.groupObject
  }

  function clearObjects() {
    if (object_model.count > 1)
      object_model.remove(1, object_model.count - 1)
    getObjectItem(0).transformationModel.clear()
    object_tree.currentIndex = -1
    object_tree.currentIndex = 0
  }

  function count() {
    return object_model.count
  }

  function currentIndex() {
    return object_tree.currentIndex
  }

  function getNumOfChildItems(index)
  {
    var count = 0
    var treeLevel = getObjectItem(index).treeLevel
    for (var i = index + 1; i < object_model.count; ++i) {
      if (getObjectItem(i).treeLevel <= treeLevel)
        break
      ++count
    }
    return count
  }

  function getObjectItem(index) {
    return (index < 0 || object_model.count <= index)
        ? null
        : object_model.get(index)
  }

  function getObjectModel() {
    return object_model
  }

  function moveObjectItem(source, dest, num) {
    var diffLevel = getObjectItem(dest).treeLevel -
                    getObjectItem(source).treeLevel
    for (var i = source; i < source + num; ++i)
      getObjectItem(i).treeLevel += (diffLevel + 1)
    if (source < dest)
      object_model.move(source, dest + 1 - num, num)
    else
      object_model.move(source, dest + 1, num)
  }

  function resetItemIndex() {
    object_tree.currentIndex = -1
    object_tree.currentIndex = 0
  }

  function setMoveMode() {
    isMoveMode = true
  }

  function setParentIndex() {
    var previousItem = getObjectItem(0)
    previousItem.parentIndex = -1
    var count = object_model.count
    for (var i = 1; i < count; ++i) {
      var item = getObjectItem(i)
      if (item.treeLevel == previousItem.treeLevel) {
        item.parentIndex = previousItem.parentIndex
      }
      else if (item.treeLevel > previousItem.treeLevel) {
        item.parentIndex = i - 1
      }
      else {
        var objectItem = getObjectItem(previousItem.parentIndex) 
        while (item.treeLevel != objectItem.treeLevel)
          objectItem = getObjectItem(objectItem.parentIndex)
        item.parentIndex = objectItem.parentIndex
      }
      previousItem = item
    }
  }

  function getMaterialIndex(index, removedIndex) {
    var i = index
    if (index == removedIndex)
      i = 0
    else if (index > removedIndex)
      i = index - 1
    return i
  }

  function setRemovedSurfaceIndex(index) {
    var count = object_model.count
    for (var i = 0; i < count; ++i) {
      var item = getObjectItem(i)
      if (item.type == Keyword.singleObject)
        item.surfaceIndex = getMaterialIndex(item.surfaceIndex, index)
    }
  }

  function setRemovedEmitterIndex(index) {
    var count = object_model.count
    for (var i = 0; i < count; ++i) {
      var item = object_model.get(i)
      if (item.type == Keyword.singleObject)
        item.emitterIndex = getMaterialIndex(item.emitterIndex, index)
    }
  }

  function removeObject(index) {
    var numOfObjects = getNumOfChildItems(index) + 1
    object_model.remove(index, numOfObjects)
    object_tree.currentIndex = 0
    setParentIndex()
  }
}
