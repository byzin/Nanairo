/*!
  \file NRotationItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NTransformationItem {
  id: rotation_item

  labelText: Nanairo.rotation

  NComboBox {
    id: axis_combobox

    anchors.horizontalCenter: undefined
    x: rotation_item.itemOffset
    y: rotation_item.labelHeight + rotation_item.itemOffset
    enabled: itemEnabled
    height: angle_spinbox.height
    backgroundColor: rotation_item.color

    model: [Nanairo.xAxis,
            Nanairo.yAxis,
            Nanairo.zAxis]
    onCurrentIndexChanged: setAxis(currentIndex)
  }

  NSpinBox {
    id: angle_spinbox

    anchors.horizontalCenter: undefined
    x: (axis_combobox.x + axis_combobox.width) + rotation_item.itemOffset
    y: axis_combobox.y
    enabled: itemEnabled

    maximumValue: Nanairo.transformationMaxValue
    minimumValue: -maximumValue
    onValueChanged: model.angle = value
  }

  NComboBox {
    id: unit_combobox

    anchors.horizontalCenter: undefined
    x: (angle_spinbox.x + angle_spinbox.width) + rotation_item.itemOffset
    y: angle_spinbox.y
    enabled: itemEnabled
    height: angle_spinbox.height
    backgroundColor: rotation_item.color

    model: [Nanairo.degreeUnit,
            Nanairo.radianUnit]
    onCurrentIndexChanged: setUnit(currentIndex)
  }

  Component.onCompleted: {
    axis_combobox.currentIndex = model.axis;
    angle_spinbox.value = model.angle;
    unit_combobox.currentIndex = model.unit;
  }

  function setAxis(axisIndex) {
    if (axisIndex != -1) {
      model.axis = axisIndex;
    }
  }

  function setUnit(unitIndex) {
    if (unitIndex != -1) {
      model.unit = unitIndex;
    }
  }
}
