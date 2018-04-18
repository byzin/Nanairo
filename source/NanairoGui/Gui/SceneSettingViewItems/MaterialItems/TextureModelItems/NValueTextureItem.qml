/*!
  \file NValueTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: textureItem

  // Properties
  property real textureValue

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "value"
    }

    NFloatSpinBox {
      id: valueSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      decimals: 5
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
