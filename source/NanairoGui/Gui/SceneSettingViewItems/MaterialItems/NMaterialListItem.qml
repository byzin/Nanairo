/*!
  \file NMaterialListItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: materialItem

  property color backgroundColor: Qt.White
  property bool isFocused: false
  // Item properties
  property int itemIndex: -1
  property string itemName: ""

  NTextField {
    id: nameLabel

    anchors.fill: parent
    activeFocusOnPress: false
    text: materialItem.itemName
    leftPadding: Definitions.defaultItemSpace
    readOnly: true

    MouseArea {
      id: mouseArea

      anchors.fill: parent

      onClicked: materialItem.nameLabelClicked()

      onDoubleClicked: {
        nameLabel.enableTextInput(true);
        materialItem.nameLabelDoubleClicked();
      }
    }

    onTextChanged: materialItem.itemName = text

    onEditingFinished: enableTextInput(false)

    function enableTextInput(isEnabled) {
      mouseArea.enabled = !isEnabled;
      nameLabel.readOnly = !isEnabled;
      if (isEnabled)
        nameLabel.forceActiveFocus();
      else
        nameLabel.focus = false;
    }
  }

  onIsFocusedChanged: setBackgroundColor()

  onItemIndexChanged: setBackgroundColor()

  function setBackgroundColor() {
    background.color = (isFocused)
        ? Material.color(Material.Lime)
        : (itemIndex % 2 == 0)
            ? backgroundColor
            : Qt.darker(backgroundColor, 1.1);
  }

  signal nameLabelClicked()

  signal nameLabelDoubleClicked()
}
