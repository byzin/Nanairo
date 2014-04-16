/*!
  \file NCameraTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "CameraItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: camera_tab_item
  width: 422
  height: 214

  property var currentObjectItem: null

  ComboBox {
    id: camera_type_combobox
    x: 8
    y: 8
    width: 125
    height: 20
    style: NComboBoxStyle {
      backgroundColor: camera_tab_item.color
    }

    model: [Keyword.pinholeCamera]

    onCurrentIndexChanged: {
      if (currentObjectItem != null)
        currentObjectItem.cameraType = camera_type_combobox.currentText
    }
  }

  TextField {
    id: camera_name_editor
    x: 146
    y: 8
    width: 120
    height: 20
    placeholderText: qsTr("Camera name")
    style: NTextFieldStyle {}

    onEditingFinished: {
      if (currentObjectItem != null)
        currentObjectItem.name = displayText
    } }

  CheckBox {
    id: jittering_checkbox
    x: 341
    y: 8
    text: qsTr("Jittering")
    style: NCheckBoxStyle {}
    checked: true

    onCheckedChanged: {
      if (currentObjectItem != null)
        currentObjectItem.jittering = checked
    }
  }

  TabView {
    id: camera_tab_view
    x: 0
    y: 36
    width: 422
    height: 178
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: qsTr(Keyword.pinholeCamera)
      active: true
      NPinholeCameraItem {
        color: camera_tab_item.color
      }
    }

    currentIndex: camera_type_combobox.currentIndex

    onCurrentIndexChanged: {
      if (currentIndex != -1 && currentObjectItem != null) {
        var tabItem = getCameraTabItem(currentIndex)
        tabItem.setDefault(currentObjectItem)
        tabItem.currentObjectItem = null
        tabItem.currentObjectItem = currentObjectItem
      }
    }
  }

  onCurrentObjectItemChanged: {
    if (currentObjectItem != null) {
      camera_name_editor.text = currentObjectItem.name
      jittering_checkbox.checked = currentObjectItem.jittering
      var index = getIndex(currentObjectItem.cameraType)
      camera_type_combobox.currentIndex = index
      var tabItem = getCameraTabItem(index)
      tabItem.currentObjectItem = null
      tabItem.currentObjectItem = currentObjectItem
    }
  }

  function getIndex(type) {
    return camera_type_combobox.find(type)
  }

  function getCameraTabItem(index) {
    return camera_tab_view.getTab(index).item
  }

  function loadSceneSettings(item, prefix) {
    item.cameraType = scene_settings.stringValue(prefix + Keyword.cameraType, 
                                                 Keyword.pinholeCamera)
    item.jittering = scene_settings.booleanValue(prefix + Keyword.jittering, false)
    var tabItem = getCameraTabItem(getIndex(item.cameraType))
    tabItem.loadSceneSettings(item, prefix)
  }

  function saveSceneSettings(item, prefix) {
    scene_settings.setStringValue(prefix + Keyword.cameraType, item.cameraType)
    scene_settings.setBooleanValue(prefix + Keyword.jittering, item.jittering)
    var tabItem = getCameraTabItem(getIndex(item.cameraType))
    tabItem.saveSceneSettings(item, prefix)
  }
}
