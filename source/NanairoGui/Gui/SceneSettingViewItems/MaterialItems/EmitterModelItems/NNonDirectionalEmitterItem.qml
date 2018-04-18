/*!
  \file NNonDirectionalEmitterItem.qml
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
  id: emitterItem

  property var textureModelList: null
  // Properties
  property int colorIndex
  property real radiantExitance

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "color"
    }

    NComboBox {
      id: colorIndexComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: emitterItem.colorIndex
      model: emitterItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: emitterItem.colorIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "radiant exitance"
    }

    NFloatSpinBox {
      id: radiantExitanceSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      decimals: 4
      floatFrom: 0.0
      floatTo: realMax
      floatValue: emitterItem.radiantExitance

      onFloatValueChanged: emitterItem.radiantExitance = floatValue
    }
  }

  onRadiantExitanceChanged: radiantExitanceSpinBox.floatValue = radiantExitance

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.emissiveColorIndex] = 0;
    item[Definitions.radiantExitance] = 32.0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    colorIndex = Definitions.getProperty(item, Definitions.emissiveColorIndex);
    radiantExitance = Definitions.getProperty(item, Definitions.radiantExitance);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.emissiveColorIndex] =
        Definitions.getProperty(item, Definitions.emissiveColorIndex);
    sceneData[Definitions.radiantExitance] =
        Definitions.getProperty(item, Definitions.radiantExitance);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.emissiveColorIndex] =
        Definitions.getProperty(sceneData, Definitions.emissiveColorIndex);
    item[Definitions.radiantExitance] =
        Definitions.getProperty(sceneData, Definitions.radiantExitance);
  }
}
