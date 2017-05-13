/*!
  \file NMaterialList.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: material_list_area

  readonly property bool isLastItem: material_list.count <= 1
  readonly property real marginSize: 2.0
  readonly property alias currentMaterialIndex: material_list.currentIndex
  property alias materialModel: material_model
  property var currentMaterialItem: material_model.isValidIndex(currentMaterialIndex)
      ?  material_model.getMaterialElement(currentMaterialIndex)
      : null

  width: Nanairo.materialListWidth

  border.color: Qt.darker(color, Nanairo.defaultDarkerScale)
  border.width: 1

  ListView {
    id: material_list

    anchors.fill: parent
    clip: true

    leftMargin: material_list_area.marginSize
    rightMargin: material_list_area.marginSize
    topMargin: material_list_area.marginSize
    bottomMargin: material_list_area.marginSize
    delegate: material_list_item_delegate
    model: material_model.model
  }

  Component {
    id: material_list_item_delegate

    NMaterialListItem {
      id: material_list_item
      backgroundColor: material_list_area.color
    }
  }

  NMaterialModel {
    id: material_model
  }

  function deleteItem() {
    var index = material_list.currentIndex;
    material_model.model.remove(index, 1);
  }
}
