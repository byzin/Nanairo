/*!
  \file NTransformationItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

Rectangle {
  id: transformation_item

  readonly property int itemOffset: 4
  readonly property int labelHeight: item_enable_checkbox.y + item_enable_checkbox.height
  readonly property alias itemEnabled: item_enable_checkbox.checked
  property var transformationList: null
  property string labelText: ""

  width: Nanairo.transformationItemWidth
  height: Nanairo.transformationItemHeight

  border.color: Qt.darker(color, 1.05)
  border.width: 1

  MouseArea {
    id: item_selectable_area

    anchors.fill: transformation_item

    onClicked: transformationList.currentIndex = model.index
  }

  NCheckBox {
    id: item_enable_checkbox

    anchors.horizontalCenter: undefined
    x: Nanairo.itemOffset
    y: Nanairo.itemOffset
    backgroundColor: transformation_item.color

    text: getOrder(model) + " " + transformation_item.labelText
    checked: model.enabled

    onCheckedChanged: {
      model.enabled = checked;
    }
  }

  function getOrder(item) {
    return transformationList.count - item.index;
  }
}
