/*!
  \file NEmitterInfoSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../Items"
import "EmitterModelItems"
import "../../definitions.js" as Definitions

NScrollView {
  id: infoSettingView

  property var textureModelList: null
  property var materialItem: null
  property string emitterType: ""

  GridLayout {
    columns: 3
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "emitter type"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: emitterTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: find(infoSettingView.emitterType)
          model: [Definitions.nonDirectionalEmitter]

          onCurrentTextChanged: infoSettingView.emitterType = currentText
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "emitter type"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: emitterItemLayout

        anchors.fill: parent
        currentIndex: 0

        NNonDirectionalEmitterItem {
          id: nonDirectionalEmitterItem
          textureModelList: infoSettingView.textureModelList
          onColorIndexChanged: infoSettingView.setProperty(Definitions.emissiveColorIndex, colorIndex)
          onRadiantExitanceChanged: infoSettingView.setProperty(Definitions.radiantExitance, radiantExitance)
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < emitterItemLayout.children.length; ++i) {
          var emitterView = emitterItemLayout.children[i];
          emitterView.background.color = background.color;
        }
      }
    }
  }

  onEmitterTypeChanged: {
    var viewIndex = emitterTypeComboBox.find(emitterType);
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
      var viewIndex = emitterTypeComboBox.find(emitterType);
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

    var viewIndex = emitterTypeComboBox.find(type);
    var emitterView = emitterItemLayout.children[viewIndex];

    var sceneData = emitterView.getSceneData(item);
    sceneData[Definitions.type] = type;

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var type = Definitions.getProperty(sceneData, Definitions.type);

    var viewIndex = emitterTypeComboBox.find(type);
    var emitterView = emitterItemLayout.children[viewIndex];

    item[Definitions.type] = type;
    emitterView.setSceneData(sceneData, item);
  }
}
