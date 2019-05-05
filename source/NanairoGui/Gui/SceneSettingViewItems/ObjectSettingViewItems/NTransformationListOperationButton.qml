/*!
  \file NTransformationListOperationButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: operationButtonView

  property bool isTranslationEnabled: false
  property bool isScalingEnabled: false
  property bool isRotationEnabled: false

  property bool isMovableUp: false
  property bool isMovableDown: false
  property bool isDeletable: false

  RowLayout {
    id: buttonLayout

    anchors.fill: parent
    spacing: Definitions.defaultItemSpace

    NImageButton {
      id: addTranslationButton

      enabled: operationButtonView.isTranslationEnabled
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/translation.png")
                           : Qt.resolvedUrl("../../image/translation_disabled.png")

      onClicked: addTranslationButtonClicked()
    }

    NImageButton {
      id: addScalingButton

      enabled: operationButtonView.isScalingEnabled
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/scaling.png")
                           : Qt.resolvedUrl("../../image/scaling_disabled.png")

      onClicked: addScalingButtonClicked()
    }

    NImageButton {
      id: addRotationButton

      enabled: operationButtonView.isRotationEnabled
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/rotation.png")
                           : Qt.resolvedUrl("../../image/rotation_disabled.png")

      onClicked: addRotationButtonClicked()
    }

    NImageButton {
      id: moveUpTransformationButton

      enabled: operationButtonView.isMovableUp
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/up.png")
                           : Qt.resolvedUrl("../../image/up_disabled.png")

      onClicked: moveUpTransformationButtonClicked()
    }

    NImageButton {
      id: moveDownTransformationButton

      enabled: operationButtonView.isMovableDown
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/down.png")
                           : Qt.resolvedUrl("../../image/down_disabled.png")

      onClicked: moveDownTransformationButtonClicked()
    }

    NImageButton {
      id: deleteTransformationButton

      enabled: operationButtonView.isDeletable
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled ? Qt.resolvedUrl("../../image/delete.png")
                           : Qt.resolvedUrl("../../image/delete_disabled.png")

      onClicked: deleteTransformationButtonClicked()
    }
  }

  signal addTranslationButtonClicked()

  signal addScalingButtonClicked()

  signal addRotationButtonClicked()

  signal moveUpTransformationButtonClicked()

  signal moveDownTransformationButtonClicked()

  signal deleteTransformationButtonClicked()
}
