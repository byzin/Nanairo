/*!
  \file NValueTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../../Items"
import "../../../definitions.js" as Definitions

NScrollView {
  id: textureItem

  // Properties
  property real textureValue

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "value"
    }

    NFloatSpinBox {
      id: valueSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: textureItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      floatFrom: 0.0
      floatTo: realMax
      floatValue: textureItem.textureValue

      onFloatValueChanged: textureItem.textureValue = floatValue
    }
  }

  onTextureValueChanged: valueSpinBox.floatValue = textureValue

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.value] = 0.8;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    textureValue = Definitions.getProperty(item, Definitions.value);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.value] =
        Definitions.getProperty(item, Definitions.value);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.value] =
        Definitions.getProperty(sceneData, Definitions.value);
  }
}
