/*!
  \file NObjectTreeOperationButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: operationButtonView

  property bool isMoveMode: false
  property bool isMovable: false
  property bool isMovableUp: false
  property bool isMovableDown: false
  property bool isDeletable: false

  RowLayout {
    id: buttonLayout

    anchors.fill: parent
    spacing: Definitions.defaultItemSpace

    NImageButton {
      id: addObjectButton

      enabled: !operationButtonView.isMoveMode
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/add_object.png")
                           : Qt.resolvedUrl("../../image/add_object_disabled.png")

      onClicked: addSingleObjectButtonClicked()
    }

    NImageButton {
      id: addGroupButton

      enabled: !operationButtonView.isMoveMode
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/add_group.png")
                           : Qt.resolvedUrl("../../image/add_group_disabled.png")

      onClicked: addGroupObjectButtonClicked()
    }

    NImageButton {
      id: moveObjectButton

      enabled: operationButtonView.isMovable
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/move_object.png")
                           : Qt.resolvedUrl("../../image/move_object_disabled.png")

      onClicked: moveObjectButtonClicked()
    }

    NImageButton {
      id: moveUpObjectButton

      enabled: !operationButtonView.isMoveMode && operationButtonView.isMovableUp
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/up.png")
                           : Qt.resolvedUrl("../../image/up_disabled.png")

      onClicked: moveUpObjectButtonClicked()
    }

    NImageButton {
      id: moveDownObjectButton

      enabled: !operationButtonView.isMoveMode && operationButtonView.isMovableDown
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/down.png")
                           : Qt.resolvedUrl("../../image/down_disabled.png")

      onClicked: moveDownObjectButtonClicked()
    }

    NImageButton {
      id: deleteObjectButton

      enabled: !operationButtonView.isMoveMode && operationButtonView.isDeletable
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/delete_object.png")
                           : Qt.resolvedUrl("../../image/delete_object_disabled.png")

      onClicked: deleteObjectButtonClicked()
    }
  }

  signal addSingleObjectButtonClicked()

  signal addGroupObjectButtonClicked()

  signal moveObjectButtonClicked()

  signal moveUpObjectButtonClicked()

  signal moveDownObjectButtonClicked()

  signal deleteObjectButtonClicked()
}
