/*!
  \file NTagSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../Items"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  readonly property string defaultSceneName: "SceneName"
  property bool isEditMode: true

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "scene name"
    }

    NTextField {
      id: sceneNameEditor

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      horizontalAlignment: TextInput.AlignRight
      verticalAlignment: TextInput.AlignBottom
      text: settingView.defaultSceneName
      placeholderText: qsTr("Scene Name")
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
