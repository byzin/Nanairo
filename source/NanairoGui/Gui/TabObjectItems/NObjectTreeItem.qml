/*!
  \file NObjectTreeItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "ObjectTreeItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: object_tree_item

  property color backgroundColor

  width: Nanairo.objectTreeItemWidth
  height: visible ? Nanairo.objectTreeItemHeight : 0
  color: (ListView.isCurrentItem)
      ? Qt.darker(backgroundColor, Nanairo.defaultDarkerScale) // Selected item
      : (model.index % 2 == 0)
          ? backgroundColor
          : Qt.darker(backgroundColor, 1.05)
  visible: model.visible

  Rectangle {
    id: indent_space
    x: 0
    y: 0
    width: Nanairo.defaultIndentWidth * model.groupLevel
    height: object_tree_item.height
    color: Qt.darker(object_tree_item.color, Nanairo.defaultDarkerScale)
  }

  NExpanderButton {
    id: expander_button
    x: indent_space.x + indent_space.width
    y: 0
    height: object_tree_item.height
    objectTree: object_tree_item.ListView.view
    backgroundColor: Qt.darker(object_tree_item.color, Nanairo.defaultDarkerScale)
  }

  NNameLabel {
    id: name_label
    x: expander_button.x + expander_button.width
    y: 0
    width: Nanairo.objectTreeItemWidth -
           (indent_space.width + expander_button.width + visibility_button.width)
    height: object_tree_item.height
    objectTree: object_tree_item.ListView.view
  }

  NVisibilityButton {
    id: visibility_button
    x: name_label.x + name_label.width
    y: 0
    height: object_tree_item.height
    objectTree: object_tree_item.ListView.view
    backgroundColor: object_tree_item.color
  }
}
