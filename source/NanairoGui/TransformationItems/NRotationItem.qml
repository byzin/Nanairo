/*!
  \file NRotationItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import ".."
import "../keyword.js" as Keyword

Rectangle {
  id: transformation_list_item
  width: parent.width
//  width: 400
  height: 35
  radius: 5

  property ListView transformationList: parent.transformationList
  property var transformationItem: parent.transformationItem

  color: (transformationList.currentIndex == model.index)
      ? Qt.darker(parent.backgroundColor, 1.15)
      : parent.backgroundColor
  border.color: Qt.darker(color, 1.4)
  border.width: 2

  MouseArea {
    anchors.fill: parent
    onClicked: transformationList.currentIndex = model.index
  }

  CheckBox {
    id: active_checkbox
    x: 13
    y: 8
    width: 20
    height: 20
    text: qsTr("")
    checked: true
    style: NCheckBoxStyle {}

    onCheckedChanged: transformationItem.active = checked
  }

  Label {
    id: name_label
    x: 34
    y: 8
    width: 20
    height: 20
    text: qsTr("R")
    font.bold: true
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
    textFormat: Text.PlainText
  }

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: transformation_list_item.color
      fontPixelSize: 10
    }
  }

  ComboBox {
    id: axis_combobox
    x: 60
    y: 8
    width: 105
    height: 20
    style: combobox_style

    model: [Keyword.x, Keyword.y, Keyword.z]

    onCurrentIndexChanged: {
      transformationItem.axis = getAxis(currentIndex)
    }
  }

  SpinBox {
    id: angle_spinbox
    x: 171
    y: 8
    width: 105
    height: 20
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 10000
    minimumValue: -10000
    value: 0
    decimals: 7

    onValueChanged: transformationItem.angle = value
  }

  ComboBox {
    id: unit_combobox
    x: 282
    y: 8
    width: 105
    height: 20
    style: combobox_style

    model: [Keyword.degreeUnit, Keyword.radianUnit]

    onCurrentIndexChanged: transformationItem.unit = getUnit(currentIndex)
  }

  Component.onCompleted: {
    axis_combobox.currentIndex = getAxisIndex(transformationItem.axis)
    angle_spinbox.value = transformationItem.angle
    unit_combobox.currentIndex = getUnitIndex(transformationItem.unit)
    active_checkbox.checked = transformationItem.active
  }

  function getAxis(index) {
    return (index == 0) ? Keyword.x : (index == 1) ? Keyword.y : Keyword.z
  }

  function getAxisIndex(axis) {
    return (axis == Keyword.x) ? 0 : (axis == Keyword.y) ? 1 : 2
  }

  function getUnit(index) {
    return (index == 0) ? Keyword.degreeUnit : Keyword.radianUnit
  }

  function getUnitIndex(unit) {
    return (unit == Keyword.degreeUnit) ? 0 : 1
  }
}
