/*!
  \file NMaterialListOperationButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: operationButtonView

  property string addButtonImagePath: ""
  property string addButtonDisabledImagePath: ""
  property string deleteButtonImagePath: ""
  property string deleteButtonDisabledImagePath: ""
  property bool isDeletable: false

  RowLayout {
    id: buttonLayout

    anchors.fill: parent
    spacing: Definitions.defaultItemSpace

    NImageButton {
      id: addMaterialButton

      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled
          ? Qt.resolvedUrl(operationButtonView.addButtonImagePath)
          : Qt.resolvedUrl(operationButtonView.addButtonDisabledImagePath)
      onClicked: addMaterialButtonClicked()
    }

    NImageButton {
      id: deleteMaterialButton

      enabled: operationButtonView.isDeletable
      Layout.preferredWidth: Definitions.defaultImageButtonSize
      Layout.preferredHeight: Definitions.defaultImageButtonSize
      imageSource: enabled
          ? Qt.resolvedUrl(operationButtonView.deleteButtonImagePath)
          : Qt.resolvedUrl(operationButtonView.deleteButtonDisabledImagePath)
      onClicked: deleteMaterialButtonClicked()
    }
  }

  signal addMaterialButtonClicked()

  signal deleteMaterialButtonClicked()
}
