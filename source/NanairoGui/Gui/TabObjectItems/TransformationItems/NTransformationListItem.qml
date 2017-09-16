/*!
  \file NTransformationListItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../../nanairo.js" as Nanairo

Rectangle {
  id: transformation_list_item

  property color backgroundColor

  width: Nanairo.transformationItemWidth
  height: Nanairo.transformationItemHeight
  color: ListView.isCurrentItem
      ? Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)
      : (model.index % 2 == 0)
          ? backgroundColor
          : Qt.darker(backgroundColor, 1.05)

  Loader {
    id: transformation_item_loader
    anchors.fill: parent
    sourceComponent: getTransformationComponent(model.type)
  }

  Component {
    id: translation_delegate
    NTranslationItem {
      color: transformation_list_item.color
      transformationList: transformation_list_item.ListView.view
    }
  }

  Component {
    id: scaling_delegate
    NScalingItem {
      color: transformation_list_item.color
      transformationList: transformation_list_item.ListView.view
    }
  }

  Component {
    id: rotation_delegate
    NRotationItem {
      color: transformation_list_item.color
      transformationList: transformation_list_item.ListView.view
    }
  }

  function getTransformationComponent(type) {
    var item = null;
    switch (type) {
      case Nanairo.translation:
        item = translation_delegate;
        break;
      case Nanairo.scaling:
        item = scaling_delegate;
        break;
      case Nanairo.rotation:
        item = rotation_delegate;
        break;
      default:
        console.assert(false, "Unsupported transformation type is specified: ", type);
        break;
    }
    return item;
  }
}
