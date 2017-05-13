/*!
  \file NVisibilityButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../../nanairo.js" as Nanairo

Button {
  id: visibility_button

  readonly property int buttonSize: Nanairo.objectTreeItemButtonSize
  property var objectTree: null
  property color backgroundColor

  width: buttonSize
  height: buttonSize
  style: button_style
  enabled: !objectTree.isMoveMode && (model.index != 0)
  visible: (model.index != 0)

  iconName: "Visibility"
  iconSource: (model.enabled) 
      ? enabled ? "../../image/visibility.png"
                : "../../image/visibility_disabled.png"
      : enabled ? "../../image/no_visibility.png"
                : "../../image/no_visibility_disabled.png"

  onClicked: {
    model.enabled = !model.enabled;
  }

  Component {
    id: button_style
    ButtonStyle {
      background: Rectangle {
        color: visibility_button.backgroundColor
      }
    }
  }
}
