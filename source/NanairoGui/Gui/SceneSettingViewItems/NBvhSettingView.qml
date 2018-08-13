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

NScrollView {
  id: settingView

  property bool isEditMode: true

  GridLayout {
    columns: 4
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "bvh type"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: bvhTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
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

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "bvh parameters"
      color: settingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: bvhItemLayout

        anchors.fill: parent
        currentIndex: bvhTypeComboBox.currentIndex

        NBinaryRadixTreeBvhItem {
          id: binaryRadixTreeBvhItem
        }

        NAgglomerativeTreeletRestructuringBvhItem {
          id: agglomerativeTreeletRestructuringBvh
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < bvhItemLayout.children.length; ++i) {
          var bvhView = bvhItemLayout.children[i];
          bvhView.background.color = background.color;
        }
      }
    }
  }

  function getSceneData() {
    var bvhView = bvhItemLayout.children[bvhTypeComboBox.currentIndex];
    var sceneData = bvhView.getSceneData();

    sceneData[Definitions.type] = bvhTypeComboBox.currentText;

    return sceneData;
  }

  function setSceneData(sceneData) {
    bvhTypeComboBox.currentIndex = bvhTypeComboBox.find(
        Definitions.getProperty(sceneData, Definitions.type));

    var bvhView = bvhItemLayout.children[bvhTypeComboBox.currentIndex];
    bvhView.setSceneData(sceneData);
  }
}
