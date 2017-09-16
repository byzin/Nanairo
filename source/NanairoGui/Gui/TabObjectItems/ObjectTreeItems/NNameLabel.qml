/*!
  \file NNameLabel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

Label {
  id: name_label

  property var objectTree: null

  height: Nanairo.objectTreeItemHeight
  enabled: (objectTree != null)
      ? (objectTree.isMoveMode && objectTree.currentIndexIsValid)
          ? (model.type== Nanairo.groupObject) &&
            !objectTree.isSameOrDescendantOfCurrentObject(model.index)
          : true
      : false

  text: model.name
  horizontalAlignment: Text.AlignLeft
  verticalAlignment: Text.AlignVCenter
  textFormat: Text.PlainText

  MouseArea {
    id: item_selectable_area
    anchors.fill: parent

    onClicked: {
      if (!objectTree.isMoveMode) {
        setCurrentIndex(name_label.objectTree, model);
      }
    }

    onDoubleClicked: {
      if (objectTree.isMoveMode) {
        objectTree.moveObjects(objectTree.currentIndex,
                               model.index,
                               objectTree.numOfCurrentObjectDescendants + 1);
        objectTree.isMoveMode = false;
        objectTree.currentIndex = -1;
        objectTree.currentIndex = 0;
      }
    }

    function setCurrentIndex(objectTree, model) {
      var flag = objectTree.currentIndex != model.index;
      if (flag) {
        objectTree.currentIndex = model.index;
      }
    }
  }
}
