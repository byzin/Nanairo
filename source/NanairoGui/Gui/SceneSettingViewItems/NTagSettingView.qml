/*!
  \file NTagSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../Items"
import "../definitions.js" as Definitions

NScrollView {
  id: settingView

  readonly property string defaultSceneName: "SceneName"
  property bool isEditMode: true

  GridLayout {
    columns: 4
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "scene name"
      color: settingView.background.color

      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent
        spacing: Definitions.defaultItemSpace

        NTextField {
          id: sceneNameEditor

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          horizontalAlignment: TextInput.AlignRight
          verticalAlignment: TextInput.AlignBottom
          text: settingView.defaultSceneName
          placeholderText: qsTr("Scene Name")
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.sceneName] = getSceneName();

    return sceneData;
  }

  function getSceneName() {
    var name = sceneNameEditor.text;
    name = (name == "") ? defaultSceneName : name;
    return name;
  }

  function setSceneData(sceneData) {
    sceneNameEditor.text = Definitions.getProperty(sceneData, Definitions.sceneName);
  }
}
