/*!
  \file NObjectTree.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../nanairo.js" as Nanairo

Rectangle {
  id: object_tree_area

  border.color: Qt.darker(color, Nanairo.defaultSarkerScale)
  border.width: 1

  readonly property alias isMoveMode: object_tree.isMoveMode
  readonly property alias currentObjectIndex: object_tree.currentIndex
  readonly property alias numOfCurrentObjectDescendants: object_tree.numOfCurrentObjectDescendants
  readonly property bool currentIsMovable: 1 < currentObjectIndex
  readonly property bool currentIsNestedObject: (currentObjectItem != null)
      ? (0 < currentObjectItem.groupLevel)
      : false
  readonly property bool currentIsBeginObject: false
  readonly property bool currentIsEndObject: false
  readonly property real marginSize: Nanairo.objectTreeItemMargin
  property alias currentObjectItem: object_tree.currentObjectItem
  property alias objectModel: object_model

  ListView {
    id: object_tree

    readonly property bool currentIndexIsValid: isValidIndex(currentIndex)
    readonly property int numOfCurrentObjectDescendants: currentIndexIsValid
        ? getNumOfDescendants(currentIndex)
        : 0
    property bool isMoveMode: false
    property var currentObjectItem: currentIndexIsValid
        ? getObject(currentIndex)
        : null

    anchors.fill: parent
    clip: true

    leftMargin: object_tree_area.marginSize
    rightMargin: object_tree_area.marginSize
    topMargin: object_tree_area.marginSize
    bottomMargin: object_tree_area.marginSize
    delegate: object_tree_item_delegate
    model: object_model.model

    function isValidIndex(index) {
      return object_model.isValidIndex(index);
    }

    function getObject(index) {
      return object_model.getObject(index);
    }

    function getNumOfDescendants(index) {
      return object_model.getNumOfDescendants(index);
    }

    function moveObjects(source, dest, num) {
      object_model.moveObjects(source, dest, num);
      updateItemVisible();
      currentIndex = 0;
    }

    function deleteObjects(source, num) {
      object_model.model.remove(source, num);
      updateItemVisible();
      currentIndex = 0;
    }

    function updateItemVisible() {
      object_model.updateItemVisible();
    }

    function isSameOrDescendantOfCurrentObject(index) {
      var flag = false;
      if (isValidIndex(index)) {
        flag = (currentIndex <= index) &&
               (index <= (currentIndex + numOfCurrentObjectDescendants));
      }
      return flag;
    }
  }

  Component {
    id: object_tree_item_delegate

    NObjectTreeItem {
      backgroundColor: object_tree_area.color
    }
  }

  NObjectModel {
    id: object_model
  }

  function toggleMoveMode() {
    object_tree.isMoveMode = !object_tree.isMoveMode;
  }

  function deleteObjects(source, num) {
    object_tree.deleteObjects(source, num);
  }
}
