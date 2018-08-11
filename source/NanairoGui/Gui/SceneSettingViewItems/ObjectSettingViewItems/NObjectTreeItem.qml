/*!
  \file NObjectTreeItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.Material 2.4
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: objectItem

  property color backgroundColor: Qt.White
  property bool isFocused: false
  property bool isExpansionMode: false
  property bool isMoveMode: false
  // Item properties
  property int itemIndex: -1
  property int itemVisibleIndex: -1
  property bool itemEnabled: false
  property string itemName: ""
  property int itemGroupLevel: 0
  property string itemObjectType: Definitions.singleObject

  height: visible ? Definitions.defaultBlockSize : 0
  enabled: !isMoveMode ||
           ((itemObjectType == Definitions.groupObject) && !isFocused)
  visible: itemVisibleIndex != -1

  NTextField {
    id: nameLabel

    anchors.fill: parent
    activeFocusOnPress: false
    text: objectItem.itemName
    leftPadding: expanderButton.x + expanderButton.width
    readOnly: true

    MouseArea {
      id: mouseArea

      anchors.fill: parent

      onClicked: objectItem.nameLabelClicked()

      onDoubleClicked: {
        if (!objectItem.isMoveMode)
          nameLabel.enableTextInput(true);
        objectItem.nameLabelDoubleClicked();
      }
    }

    onTextChanged: objectItem.itemName = text

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

  RowLayout {
    id: itemLayout

    anchors.fill: parent
    spacing: 0

    NImageButton {
      id: expanderButton

      readonly property int indentSize: 8
      readonly property int maxGroupLevel: 16

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      Layout.leftMargin: (objectItem.itemGroupLevel < maxGroupLevel)
          ? objectItem.itemGroupLevel * indentSize
          : (maxGroupLevel - 1) * indentSize;
      Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
      enabled: objectItem.itemObjectType == Definitions.groupObject
      opacity: (objectItem.itemObjectType == Definitions.groupObject) ? 1 : 0
      imageSource: enabled ? Qt.resolvedUrl("../../image/expander.png")
                           : Qt.resolvedUrl("../../image/expander_disabled.png")

      Component.onCompleted: {
        // Initialize the background
        background.border.width = 0;
      }

      onClicked: objectItem.isExpansionMode = !objectItem.isExpansionMode;
    }

    NImageButton {
      id: visibilityButton

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      Layout.rightMargin: expanderButton.indentSize
      Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
      enabled: !objectItem.isMoveMode
      imageSource: objectItem.itemEnabled
          ? (enabled ? Qt.resolvedUrl("../../image/visibility.png")
                     : Qt.resolvedUrl("../../image/visibility_disabled.png"))
          : (enabled ? Qt.resolvedUrl("../../image/no_visibility.png")
                     : Qt.resolvedUrl("../../image/no_visibility_disabled.png"))

      Component.onCompleted: {
        // Initialize the background
        background.border.width = 0;
      }

      onClicked: objectItem.itemEnabled = !objectItem.itemEnabled
    }
  }

  onIsFocusedChanged: setBackgroundColor()

  onIsExpansionModeChanged: expanderButton.contentItem.rotation = isExpansionMode
      ? 90
      : 0;

  onItemVisibleIndexChanged: setBackgroundColor()

  function setBackgroundColor() {
    background.color = (isFocused)
        ? Material.color(Material.Lime)
        : (itemVisibleIndex % 2 == 0)
            ? backgroundColor
            : Qt.darker(backgroundColor, 1.1);
  }

  signal nameLabelClicked()

  signal nameLabelDoubleClicked()
}
