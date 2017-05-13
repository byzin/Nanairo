/*!
  \file NMaterialListView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: material_list_view

  readonly property int buttonAreaOffset: 4
  readonly property int buttonSize: Nanairo.defaultImageButtonSize
  property var materialList: material_list
  property alias materialModel: material_list.materialModel
  property alias currentMaterialItem: material_list.currentMaterialItem
  property alias addMaterialButton: add_material_button
  property alias deleteMaterialButton: delete_material_button

  width: Nanairo.materialListViewWidth
  height: Nanairo.tabItemHeight

  NMaterialList {
    id: material_list

    anchors.top: parent.top
    anchors.topMargin: Nanairo.materialViewMargin
    anchors.right: parent.right
    anchors.rightMargin: Nanairo.materialViewMargin
    anchors.bottom: parent.bottom
    anchors.bottomMargin: buttonSize + buttonAreaOffset * 2
    color: material_list_view.color
  }

  NImageButton {
    id: add_material_button

    x: material_list.x
    y: (material_list.y + material_list.height) + buttonAreaOffset
    backgroundColor: material_list_view.color

    iconName: qsTr("Add a material")
  }

  NImageButton {
    id: delete_material_button

    x: add_material_button.x + buttonSize + buttonAreaOffset
    y: add_material_button.y
    backgroundColor: material_list_view.color
    enabled: !material_list.isLastItem

    iconName: qsTr("Delete a material")

    onClicked: material_list.deleteItem()
  }
}
