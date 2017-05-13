/*!
  \file NTransformationTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "TransformationItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: transformation_tab_item

  readonly property int buttonAreaOffset: 4
  readonly property int buttonSize: Nanairo.defaultImageButtonSize
  readonly property bool isCameraItem: (currentItem != null)
      ? (currentItem["type"] == Nanairo.cameraObject)
      : false
  property var currentItem: null
  property bool isItemChangeMode: false

  width: Nanairo.objectSettingTabWidth
  height: Nanairo.objectSettingTabHeight

  NTransformationList {
    id: transformation_list

    anchors.horizontalCenter: parent.horizontalCenter
    color: transformation_tab_item.color
    anchors.top: parent.top
    anchors.topMargin: Nanairo.transformationListMargin
    anchors.bottom: parent.bottom
    anchors.bottomMargin: buttonSize + buttonAreaOffset * 2

    currentObjectItem: transformation_tab_item.currentItem
  }
 
  NImageButton {
    id: add_translation_button

    x: transformation_list.x
    y: (transformation_list.y + transformation_list.height) + buttonAreaOffset
    backgroundColor: transformation_tab_item.color

    iconName: qsTr("Add a translation")
    iconSource: enabled ? "../image/translation.png"
                        : "../image/translation_disabled.png"
    onClicked: transformation_list.addTranslationItem()
  }

  NImageButton {
    id: add_scaling_button

    x: (add_translation_button.x + add_translation_button.width) + buttonAreaOffset
    y: add_translation_button.y
    backgroundColor: transformation_tab_item.color
    enabled: !isCameraItem

    iconName: qsTr("Add a scaling")
    iconSource: enabled ? "../image/scaling.png"
                        : "../image/scaling_disabled.png"
    onClicked: transformation_list.addScalingItem()
  }

  NImageButton {
    id: add_rotation_button

    x: (add_scaling_button.x + add_scaling_button.width) + buttonAreaOffset
    y: add_scaling_button.y
    backgroundColor: transformation_tab_item.color

    iconName: qsTr("Add a rotation")
    iconSource: enabled ? "../image/rotation.png"
                        : "../image/rotation_disabled.png"
    onClicked: transformation_list.addRotationItem()
  }

  NImageButton {
    id: up_transformation_button

    x: (add_rotation_button.x + add_rotation_button.width) + buttonAreaOffset
    y: add_rotation_button.y
    backgroundColor: transformation_tab_item.color
    enabled: (transformation_list.currentTransformationItem != null) &&
             !(transformation_list.isLastItem || transformation_list.isBeginItem)

    iconName: qsTr("Up a transformation")
    iconSource: enabled ? "../image/up.png"
                        : "../image/up_disabled.png"
    onClicked: transformation_list.upItem()
  }

  NImageButton {
    id: down_transformation_button

    x: (up_transformation_button.x + up_transformation_button.width) + buttonAreaOffset
    y: up_transformation_button.y
    backgroundColor: transformation_tab_item.color
    enabled: (transformation_list.currentTransformationItem != null) &&
             !(transformation_list.isLastItem || transformation_list.isEndItem)

    iconName: qsTr("Down a transformation")
    iconSource: enabled ? "../image/down.png"
                        : "../image/down_disabled.png"
    onClicked: transformation_list.downItem()
  }

  NImageButton {
    id: delete_transformation_button

    x: (down_transformation_button.x + down_transformation_button.width) + buttonAreaOffset
    y: down_transformation_button.y
    backgroundColor: transformation_tab_item.color
    enabled: (transformation_list.currentTransformationItem != null) &&
             (0 < transformation_list.transformationModel.count)

    iconName: qsTr("Delete a transformation")
    iconSource: enabled ? "../image/delete.png"
                        : "../image/delete_disabled.png"
    onClicked: transformation_list.deleteItem()
  }

  Component {
    id: transformation_model
    ListModel {
    }
  }

  function initializeItem(item) {
    item[Nanairo.transformation] = 
        transformation_model.createObject(transformation_tab_item);
  }

  function reflectItem() {
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function getTransformationData(item) {
    return transformation_list.getTransformationItemData(item);
  }

  function makeTransformationItem(itemData) {
    var item = transformation_model.createObject(transformation_tab_item);
    var itemList = transformation_list.makeTransformationItem(itemData);
    for (var i = 0; i < itemList.length; ++i) {
      item.append(itemList[i]);
    }
    return item;
  }
}
