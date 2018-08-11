/*!
  \file NBvhSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../Items"
import "BvhItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property bool isEditMode: true

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
      currentIndex: 0
      model: [Definitions.binaryRadixTreeBvh,
              Definitions.agglomerativeTreeletRestructuringBvh]

      onCurrentIndexChanged: {
        if (settingView.isEditMode) {
          var bvhView = bvhItemLayout.children[currentIndex];
          bvhView.initSceneData();
        }
      }
    }
  }

  StackLayout {
    id: bvhItemLayout

    anchors.fill: parent
    anchors.topMargin: (column1.y + column1.height) + Definitions.defaultBlockSize
    currentIndex: typeComboBox.currentIndex

    NBinaryRadixTreeBvhItem {
      id: binaryRadixTreeBvhItem
    }

    NAgglomerativeTreeletRestructuringBvhItem {
      id: agglomerativeTreeletRestructuringBvh
    }
  }

  function getSceneData() {
    var bvhView = bvhItemLayout.children[typeComboBox.currentIndex];
    var sceneData = bvhView.getSceneData();

    sceneData[Definitions.type] = typeComboBox.currentText;

    return sceneData;
  }

  function setSceneData(sceneData) {
    typeComboBox.currentIndex = typeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.type));

    var bvhView = bvhItemLayout.children[typeComboBox.currentIndex];
    bvhView.setSceneData(sceneData);
  }
}
