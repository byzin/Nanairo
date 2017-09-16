/*!
  \file NExpanderButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../../nanairo.js" as Nanairo

Button {
  id: expander_button

  readonly property bool isGroupObject: (model.type == Nanairo.groupObject)
  readonly property int buttonSize: Nanairo.objectTreeItemHeight
  property var objectTree: null
  property color backgroundColor

  width: buttonSize
  height: buttonSize
  style: button_style

  enabled: isGroupObject

  rotation: model.expanded ? 90.0
                           : 0.0
  iconName: qsTr("Expander")
  iconSource: isGroupObject ?  enabled ? "../../image/expander.png"
                                       : "../../image/expander_disabled.png"
                            : ""

  onClicked: {
    model.expanded = !model.expanded;
    objectTree.updateItemVisible();
  }

  Component {
    id: button_style
    ButtonStyle {
      background: Rectangle {
        color: expander_button.backgroundColor
      }
    }
  }
}
