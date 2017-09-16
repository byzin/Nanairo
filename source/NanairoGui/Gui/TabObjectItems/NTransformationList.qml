/*!
  \file NTransformationList.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "TransformationItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: transformation_list_area

  readonly property alias currentTransformationItem: transformation_list.currentItem
  readonly property bool isLastItem: transformation_list.count <= 1
  readonly property bool isBeginItem: transformation_list.currentIndex == 0
  readonly property bool isEndItem: transformation_list.currentIndex ==
                                        (transformation_list.count - 1)
  property var currentObjectItem: null
  property var transformationModel: (currentObjectItem != null)
                                        ? currentObjectItem[Nanairo.transformation]
                                        : null

  width: Nanairo.transformationListWidth

  border.color: Qt.darker(color, Nanairo.defaultDarkerScale)
  border.width: 1

  ListView  {
    id: transformation_list

    readonly property real marginSize: Nanairo.transformationItemMargin

    anchors.fill: parent
    clip: true

    leftMargin: marginSize
    rightMargin: marginSize
    topMargin: marginSize
    bottomMargin: marginSize
    delegate: transformation_item_delegate
    model: transformation_list_area.transformationModel
  }

  Component {
    id: transformation_item_delegate
    NTransformationListItem {
      backgroundColor: transformation_list_area.color
    }
  }

  function upItem() {
    var index = transformation_list.currentIndex
    transformationModel.move(index, index - 1, 1);
  }

  function downItem() {
    var index = transformation_list.currentIndex
    transformationModel.move(index, index + 1, 1);
  }

  function deleteItem() {
    var index = transformation_list.currentIndex
    transformationModel.remove(index, 1);
  }

  function createTransformationItem() {
    var item = {};
    item.enabled = true;
    
    // Dummy
    item.x = 0.0;
    item.y = 0.0;
    item.z = 0.0;
    item.axis = 0;
    item.angle = 0.0;
    item.unit = 0;

    return item;
  }

  function addTranslationItem() {
    var item = createTransformationItem();
    item.type = Nanairo.translation;
    item.x = 0.0;
    item.y = 0.0;
    item.z = 0.0;
    transformationModel.insert(0, item);
  }

  function addScalingItem() {
    var item = createTransformationItem();
    item.type = Nanairo.scaling;
    item.x = 1.0;
    item.y = 1.0;
    item.z = 1.0;
    transformationModel.insert(0, item);
  }

  function addRotationItem() {
    var item = createTransformationItem();
    item.type = Nanairo.rotation;
    item.axis = 0;
    item.angle = 0.0;
    item.unit = 0;
    transformationModel.insert(0, item);
  }

  function getTranslationItemData(item) {
    var itemData = {};
    itemData[Nanairo.value] = [item.x, item.y, item.z];
    return itemData;
  }

  function makeTranslationItem(itemData) {
    var item = {};
    var value = itemData[Nanairo.value];
    item.x = value[0];
    item.y = value[1];
    item.z = value[2];
    return item;
  }

  function getScalingItemData(item) {
    return getTranslationItemData(item);
  }

  function makeScalingItem(itemData) {
    return makeTranslationItem(itemData);
  }

  function getAxisIndex(axisText) {
    return   (axisText == Nanairo.xAxis) ? 0
           : (axisText == Nanairo.yAxis) ? 1
           :                               2;
  }

  function getAxisText(axisIndex) {
    return   (axisIndex == 0) ? Nanairo.xAxis
           : (axisIndex == 1) ? Nanairo.yAxis
           :                    Nanairo.zAxis;
  }

  function getUnitIndex(unitText) {
    return (unitText == Nanairo.degreeUnit) ? 0
                                            : 1;
  }

  function getUnitText(unitIndex) {
    return (unitIndex == 0) ? Nanairo.degreeUnit
                            : Nanairo.radianUnit;
  }

  function getRotationItemData(item) {
    var itemData = {};
    itemData[Nanairo.axis] = getAxisText(item.axis);
    itemData[Nanairo.angle] = item.angle;
    itemData[Nanairo.unit] = getUnitText(item.unit);
    return itemData;
  }

  function makeRotationItem(itemData) {
    var item = {};
    item.axis = getAxisIndex(itemData[Nanairo.axis]);
    item.angle = itemData[Nanairo.angle];
    item.unit = getUnitIndex(itemData[Nanairo.unit]);
    return item;
  }

  function getItemData(item) {
    var type = item.type;
    var itemData = (type == Nanairo.translation) ? getTranslationItemData(item)
                 : (type == Nanairo.scaling)     ? getScalingItemData(item)
                 :                             getRotationItemData(item);
    itemData[Nanairo.type] = type;
    itemData[Nanairo.enabled] = item.enabled;
    return itemData;
  }

  function makeItem(itemData) {
    var type = itemData[Nanairo.type];
    var item = (type == Nanairo.translation) ? makeTranslationItem(itemData)
             : (type == Nanairo.scaling)     ? makeScalingItem(itemData)
             :                             makeRotationItem(itemData);
    item.type = type;
    item.enabled = itemData[Nanairo.enabled];
    return item;
  }

  function getTransformationItemData(item) {
    var itemData = [];
    for (var i = 0; i < item.count; ++i) {
      var transformationItem = item.get(i);
      itemData.push(getItemData(transformationItem));
    }
    return itemData;
  }

  function makeTransformationItem(itemData) {
    var item = [];
    for (var i = 0; i < itemData.length; ++i) {
      var transformationItemData = itemData[i];
      item.push(makeItem(transformationItemData));
    }
    return item;
  }
}
