/*!
  \file NObjectSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: object_setting_view
  width: 422
  height: 240

  Component {
    id: tab_view_style
    NTabViewStyle {
      backgroundColor: object_setting_view.color
    }
  }

  TabView {
    id: object_tab_view
    x: 0
    y: 8
    width: parent.width
    height: parent.height - y
    style: tab_view_style
  
    Tab {
      title: "Camera"
      active: true
      NCameraTabItem {
        color: object_setting_view.color
      }
    }
 
    Tab {
      title: "Object"
      active: true
      NObjectTabItem {
        color: object_setting_view.color
      }
    }
  
    Tab {
      title: "Group"
      active: true
      NGroupObjectTabItem {
        color: object_setting_view.color
      }
    }
  
    Tab {
      title: "Transformation"
      active: true
      NTransformationTabItem {
        color: object_setting_view.color
      }
    }

    function getObjectTabItem(type) {
      var tabItem = null
      switch (type) {
       case Keyword.cameraObject:
        tabItem = object_tab_view.getTab(0).item
        break
       case Keyword.singleObject:
        tabItem = object_tab_view.getTab(1).item
        break
       case Keyword.groupObject:
        tabItem = object_tab_view.getTab(2).item
        break
       default:
        break
      }
      return tabItem
    }
  
    function setCurrentObjectItem(item) {
      for (var i = 0; i < count; ++i) {
        var tabItem = getTab(i)
        tabItem.enabled = false 
        tabItem.item.currentObjectItem = null
      }

      if (item == null) {
        currentIndex = count - 1
        return
      }

      var itemType = item.type
      var tabIndex = (itemType == Keyword.cameraObject)
          ? 0
          : (itemType == Keyword.singleObject)
              ? 1
              : 2

      var transformationTab = getTab(count - 1)
      transformationTab.enabled = true
      transformationTab.item.currentObjectItem = item
      transformationTab.item.scalingButtonEnabled = 
          (itemType != Keyword.cameraObject)

      var tabItem = getTab(tabIndex)
      tabItem.enabled = true
      tabItem.item.currentObjectItem = item
      currentIndex = tabIndex
    }

    Component.onCompleted: currentIndex = count - 1
  }

  function addMatrix(item, matrix) {
    var transformationTab = object_tab_view.getTab(object_tab_view.count - 1)
    transformationTab.item.addMatrix(item, matrix)
  }

  function setCurrentObjectItem(item) {
    object_tab_view.setCurrentObjectItem(item)
  }

  function setMaterialModel(surfaceModel, emitterModel) {
    var tabItem = object_tab_view.getTab(1).item
    tabItem.setMaterialModel(surfaceModel, emitterModel)
  }

  function loadSceneSettings(item, prefix) {
    var tabItem = object_tab_view.getObjectTabItem(item.type)
    tabItem.loadSceneSettings(item, prefix)
    var transformationTabItem = object_tab_view.getTab(3).item
    transformationTabItem.loadSceneSettings(item, prefix)
  }

  function saveSceneSettings(item, prefix) {
    var tabItem = object_tab_view.getObjectTabItem(item.type)
    tabItem.saveSceneSettings(item, prefix)
    var transformationTabItem = object_tab_view.getTab(3).item
    transformationTabItem.saveSceneSettings(item, prefix)
  }
}
