/*!
  \file NGroupObjectTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import "nanairo_definitions.js" as Nanairo

Rectangle {
  id: group_tab_item
  width: 422
  height: 214

  property var currentObjectItem: null

  TextField {
    id: group_name_editor
    x: 8
    y: 8
    width: 141
    height: 20
    placeholderText: qsTr("Group name")
    style: NTextFieldStyle {}

    onEditingFinished: {
      if (currentObjectItem != null)
        currentObjectItem.name = displayText
    }
  }

  onCurrentObjectItemChanged: {
    if (currentObjectItem != null) {
      group_name_editor.text = currentObjectItem.name
    }
  }

  function loadSceneSettings(item, prefix) {
  }

  function saveSceneSettings(item, prefix) {
  }
}
