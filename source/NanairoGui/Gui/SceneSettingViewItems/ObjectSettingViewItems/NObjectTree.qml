/*!
  \file NObjectTree.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: objectTree

  readonly property int viewBorderWidth: 2
  readonly property int viewMarginSize: viewBorderWidth + 1
  readonly property string delegateItemName: "ObjectTreeItem"
  property var model: null
  property bool isMoveMode: false

  ListView {
    id: objectTreeView

    anchors.fill: parent
    leftMargin: objectTree.viewMarginSize
    rightMargin: objectTree.viewMarginSize
    topMargin: objectTree.viewMarginSize
    bottomMargin: objectTree.viewMarginSize
    clip: true
    model: objectTree.model

    delegate: NObjectTreeItem {
      width: objectTree.width - 2 * objectTree.viewMarginSize
      objectName: objectTree.delegateItemName
      backgroundColor: objectTree.background.color
      isFocused: ListView.isCurrentItem
      isMoveMode: objectTree.isMoveMode
      itemIndex: model.index
      itemObjectType: model[Definitions.modelTypeKey]

      Component.onCompleted: syncProperties()

      onIsExpansionModeChanged: {
        setProperty(Definitions.modelExpandedKey, isExpansionMode);
        objectTree.itemVisibleChanged();
      }

      onItemEnabledChanged: setProperty(Definitions.modelEnabledKey, itemEnabled)

      onItemNameChanged: setProperty(Definitions.modelNameKey, itemName);

      onNameLabelClicked: {
        if (!isMoveMode)
          objectTreeView.currentIndex = itemIndex;
      }

      onNameLabelDoubleClicked: {
        if (isMoveMode)
          objectTree.moveDestinationSelected(itemIndex);
      }

      function setProperty(propertyName, value) {
        if (objectTree.model.isValidIndex(itemIndex)) {
          var item = objectTree.model.get(itemIndex);
          Definitions.setProperty(item, propertyName, value);
        }
      }

      function syncProperties() {
        if (objectTree.model.isValidIndex(itemIndex)) {
          isExpansionMode = model[Definitions.modelExpandedKey];
          itemVisibleIndex = model[Definitions.modelVisibleIndexKey];
          itemEnabled = model[Definitions.modelEnabledKey];
          itemName = model[Definitions.modelNameKey];
          itemGroupLevel = model[Definitions.modelGroupLevelKey];
        }
      }
    }

    onCurrentIndexChanged: objectTree.currentIndexIsChanged(currentIndex)
  }

  Component.onCompleted: {
    // Initialize the background
    background.border.width = viewBorderWidth;
    background.border.color = Material.color(Material.BlueGrey);
  }

  function getCurrentIndex() {
    return objectTreeView.currentIndex;
  }

  function setCurrentIndex(index) {
    if (model.isValidIndex(index))
      objectTreeView.currentIndex = index;
  }

  function syncData() {
    var itemViewList = objectTreeView.contentItem.children;
    for (var i = 0; i < itemViewList.length; ++i) {
      var itemView = itemViewList[i];
      if (itemView.objectName == objectTree.delegateItemName)
        itemView.syncProperties();
    }
  }

  function toggleMoveMode() {
    isMoveMode = !isMoveMode;
  }

  signal currentIndexIsChanged(int index)

  signal itemVisibleChanged()

  signal moveDestinationSelected(int index)
}
