/*!
  \file NScalingItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import ".."

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
    text: qsTr("S")
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

  SpinBox {
    id: x_spinbox
    x: 60
    y: 8
    width: 105
    height: 20
    prefix: "x: "
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 10000
    minimumValue: 0.0000001
    value: 1.0
    decimals: 7

    onValueChanged: transformationItem.x = value
  }

  SpinBox {
    id: y_spinbox
    x: 171
    y: 8
    width: 105
    height: 20
    prefix: "y: "
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 10000
    minimumValue: 0.0000001
    value: 1.0
    decimals: 7

    onValueChanged: transformationItem.y = value
  }

  SpinBox {
    id: z_spinbox
    x: 282
    y: 8
    width: 105
    height: 20
    prefix: "z: "
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 10000
    minimumValue: 0.0000001
    value: 1.0
    decimals: 7

    onValueChanged: transformationItem.z = value
  }

  Component.onCompleted: {
    x_spinbox.value = transformationItem.x
    y_spinbox.value = transformationItem.y
    z_spinbox.value = transformationItem.z
    active_checkbox.checked = transformationItem.active
  }
}
