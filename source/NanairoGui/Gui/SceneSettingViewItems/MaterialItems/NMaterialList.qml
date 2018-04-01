/*!
  \file NMaterialList.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: materialList

  readonly property int viewBorderWidth: 2
  readonly property int viewMarginSize: viewBorderWidth + 1
  readonly property int materialListWidth: 200
  property var model: null

  ListView {
    id: materialListView

    anchors.fill: parent
    leftMargin: materialList.viewMarginSize
    rightMargin: materialList.viewMarginSize
    topMargin: materialList.viewMarginSize
    bottomMargin: materialList.viewMarginSize
    clip: true
    model: materialList.model

    delegate: NMaterialListItem {
      width: materialList.width - 2 * materialList.viewMarginSize
      height: Definitions.defaultBlockSize
      backgroundColor: materialList.background.color
      isFocused: ListView.isCurrentItem
      itemIndex: model.index
      itemName: Definitions.getProperty(model, Definitions.modelNameKey)

      onItemNameChanged: setProperty(Definitions.modelNameKey, itemName)

      onNameLabelClicked: materialListView.currentIndex = itemIndex

      function setProperty(propertyName, value) {
        if (materialList.model.isValidIndex(itemIndex)) {
          var item = materialList.model.get(itemIndex);
          Definitions.setProperty(item, propertyName, value);
        }
      }
    }

    onCurrentIndexChanged: materialList.currentIndexIsChanged(currentIndex)
  }

  Component.onCompleted: {
    // Initialize the background
    background.border.width = viewBorderWidth;
    background.border.color = Material.color(Material.BlueGrey);
  }

  function getCurrentIndex() {
    return materialListView.currentIndex;
  }

  function setCurrentIndex(index) {
    materialListView.currentIndex = (model.isValidIndex(index)) ? index : -1;
  }

  signal currentIndexIsChanged(int index)
}
