/*!
  \file NEmitterInfoSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../Items"
import "EmitterModelItems"
import "../../definitions.js" as Definitions

NPane {
  id: infoSettingView

  property var textureModelList: null
  property var materialItem: null
  property string emitterType: ""

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "type"
    }

    NComboBox {
      id: typeComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: find(infoSettingView.emitterType)
      model: [Definitions.nonDirectionalEmitter]

      onCurrentTextChanged: infoSettingView.emitterType = currentText
    }
  }

  StackLayout {
    id: emitterItemLayout

    anchors.fill: parent
    anchors.topMargin: (column1.y + column1.height) + Definitions.defaultBlockSize
    currentIndex: 0

    NNonDirectionalEmitterItem {
      id: nonDirectionalEmitterItem
      textureModelList: infoSettingView.textureModelList
      onColorIndexChanged: infoSettingView.setProperty(Definitions.emissiveColorIndex, colorIndex)
      onRadiantExitanceChanged: infoSettingView.setProperty(Definitions.radiantExitance, radiantExitance)
    }
  }

  onEmitterTypeChanged: {
    var viewIndex = typeComboBox.find(emitterType);
    if ((materialItem != null) && (viewIndex != -1)) {
      // Set base emitter properties
      setProperty(Definitions.type, emitterType);
      // Set other emitter properties
      var settingView = emitterItemLayout.children[viewIndex];
      settingView.initItem(materialItem);
      // Change the emitter setting view
      emitterItemLayout.currentIndex = viewIndex;
      // Set the emitter properties to the view
      settingView.setValue(materialItem);
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null");
    // Set the properties of non-directional emitter item
    item[Definitions.type] = Definitions.nonDirectionalEmitter;
    nonDirectionalEmitterItem.initItem(item);
  }

  function setProperty(propertyName, value) {
    if (materialItem != null)
      Definitions.setProperty(materialItem, propertyName, value);
  }

  function setItem(item) {
    materialItem = null;
    if (item != null) {
      // Set emitter type to the view
      emitterType = Definitions.getProperty(item, Definitions.type);
      // Change the emitter setting view
      var viewIndex = typeComboBox.find(emitterType);
      console.assert(viewIndex != -1, "The item has invalid emitter type.");
      emitterItemLayout.currentIndex = viewIndex;
      // Set the item properties to the view
      var settingView = emitterItemLayout.children[viewIndex];
      settingView.setValue(item);
    }
    materialItem = item;
  }

  function getSceneData(item) {
    var type = Definitions.getProperty(item, Definitions.type);

    var viewIndex = typeComboBox.find(type);
    var emitterView = emitterItemLayout.children[viewIndex];

    var sceneData = emitterView.getSceneData(item);
    sceneData[Definitions.type] = type;

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var type = Definitions.getProperty(sceneData, Definitions.type);

    var viewIndex = typeComboBox.find(type);
    var emitterView = emitterItemLayout.children[viewIndex];

    item[Definitions.type] = type;
    emitterView.setSceneData(sceneData, item);
  }
}
