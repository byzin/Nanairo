/*!
  \file NObjectTreeView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: object_tree_view

  readonly property int buttonAreaOffset: 4
  readonly property int buttonSize: Nanairo.defaultImageButtonSize
  property alias addObjectButton: add_object_button
  property alias addGroupButton: add_group_button
  property alias currentObjectItem: object_tree.currentObjectItem
  property alias objectModel: object_tree.objectModel

  width: Nanairo.objectTreeViewWidth
  height: Nanairo.objectTreeViewHeight

  NObjectTree {
    id: object_tree

    anchors.right: parent.right
    anchors.rightMargin: Nanairo.treeViewMargin
    anchors.left: parent.left
    anchors.leftMargin: Nanairo.treeViewMargin
    anchors.top: parent.top
    anchors.topMargin: Nanairo.treeViewMargin
    anchors.bottom: parent.bottom
    anchors.bottomMargin: buttonSize + buttonAreaOffset * 2
    color: object_tree_view.color
  }

  NImageButton {
    id: add_object_button

    x: object_tree.x
    y: (object_tree.y + object_tree.height) + buttonAreaOffset
    backgroundColor: object_tree_view.color
    enabled: !object_tree.isMoveMode

    iconName: qsTr("Add a object")
    iconSource: enabled ? "../image/add_object.png"
                        : "../image/add_object_disabled.png"
  }

  NImageButton {
    id: add_group_button

    x: add_object_button.x + buttonSize + buttonAreaOffset
    y: add_object_button.y
    backgroundColor: object_tree_view.color
    enabled: !object_tree.isMoveMode

    iconName: qsTr("Add a group")
    iconSource: enabled ? "../image/add_group.png"
                        : "../image/add_group_disabled.png"
  }

  NImageButton {
    id: move_object_button

    x: add_group_button.x + buttonSize + buttonAreaOffset
    y: add_group_button.y
    backgroundColor: object_tree_view.color
    enabled: object_tree.currentIsMovable

    iconName: qsTr("Move a object")
    iconSource: enabled ? "../image/move_object.png"
                        : "../image/move_object_disabled.png"
    onClicked: object_tree.toggleMoveMode()
  }

  NImageButton {
    id: up_object_button

    x: move_object_button.x + buttonSize + buttonAreaOffset
    y: move_object_button.y
    backgroundColor: object_tree_view.color
//    enabled: !object_tree.isMoveMode &&
//             object_tree.currentIsBeginObject
    enabled: false

    iconName: qsTr("Up a object")
    iconSource: enabled ? "../image/up.png"
                        : "../image/up_disabled.png"
  }

  NImageButton {
    id: down_object_button

    x: up_object_button.x + buttonSize + buttonAreaOffset
    y: up_object_button.y
    backgroundColor: object_tree_view.color
//    enabled: !object_tree.isMoveMode &&
//             object_tree.currentIsEndObject
    enabled: false

    iconName: qsTr("Down a object")
    iconSource: enabled ? "../image/down.png"
                        : "../image/down_disabled.png"
  }

  NImageButton {
    id: delete_object_button

    x: down_object_button.x + buttonSize + buttonAreaOffset
    y: down_object_button.y
    backgroundColor: object_tree_view.color
    enabled: !object_tree.isMoveMode && object_tree.currentIsMovable

    iconName: qsTr("Delete a object")
    iconSource: enabled ? "../image/delete_object.png"
                        : "../image/delete_object_disabled.png"
    onClicked: object_tree.deleteObjects(
        object_tree.currentObjectIndex,
        object_tree.numOfCurrentObjectDescendants + 1)
  }
}
