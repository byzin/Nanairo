/*!
  \file NSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3

Rectangle {
  id: setting_view
  width: 640
  height: 300

  property int removedSurfaceIndex: -1
  property int removedEmitterIndex: -1

  Component {
    id: tab_view_style
    NTabViewStyle {
      backgroundColor: setting_view.color
    }
  }

  TabView {
    id: setting_tab_view
    x: 0
    y: 8
    width: parent.width
    height: parent.height - y
    style: tab_view_style

    Tab {
      title: qsTr("System")
      active: true
      NSystemTabItem {
        color: setting_view.color
      }
    }

    Tab {
      title: qsTr("Color")
      active: true
      NColorTabItem {
        color: setting_view.color
      }
    }

    Tab {
      title: qsTr("Rendering")
      active: true
      NRenderingMethodTabItem {
        color: setting_view.color
      }
    }

    Tab {
      title: qsTr("BVH")
      active: true
      NBvhTabItem {
        color: setting_view.color
      }
    }

    Tab {
      title: qsTr("Emitter model")
      active: true
      NEmitterModelTabItem {
        color: setting_view.color

        onRemovedEmitterIndexChanged: {
          setting_view.removedEmitterIndex = removedEmitterIndex
        }
      }
    }

    Tab {
      title: qsTr("Surface model")
      active: true
      NSurfaceModelTabItem {
        color: setting_view.color

        onRemovedSurfaceIndexChanged: {
          setting_view.removedSurfaceIndex = removedSurfaceIndex
        }
      }
    }

    Tab {
      title: qsTr("Texture")
      active: true
      NTextureTabItem {
        color: setting_view.color
        onRemovedTextureIndexChanged: {
          var index = removedTextureIndex
          if (index != -1) {
            var surfaceModelTab = getSettingTabItem(setting_tab_view.count - 2)
            surfaceModelTab.setRemovedTextureIndex(index)
          }
        }
      }
    }
  }

  function getEmitterModel() {
    var emitterModelTab = getSettingTabItem(setting_tab_view.count - 3)
    return emitterModelTab.getEmitterModel()
  }

  function getImageWidth() {
    var systemTab = getSettingTabItem(0)
    return systemTab.getImageWidth()
  }

  function getImageHeight() {
    var systemTab = getSettingTabItem(0)
    return systemTab.getImageHeight()
  }

  function getSceneName() {
    var systemTab = getSettingTabItem(0)
    return systemTab.getSceneName()
  }

  function getSettingTabItem(index) {
    return setting_tab_view.getTab(index).item
  }

  function getSurfaceModel() {
    var surfaceModelTab = getSettingTabItem(setting_tab_view.count - 2)
    return surfaceModelTab.getSurfaceModel()
  }

  function initialize() {
    var textureTab = getSettingTabItem(setting_tab_view.count - 1)
    textureTab.initialize()
    var surfaceModelTab = getSettingTabItem(setting_tab_view.count - 2)
    surfaceModelTab.initialize()
    surfaceModelTab.textureModel = textureTab.textureModel
    var emitterModelTab = getSettingTabItem(setting_tab_view.count - 3)
    emitterModelTab.initialize()
  }

  function loadSceneSettings() {
    var systemTab = getSettingTabItem(0)
    systemTab.loadSceneSettings()
    var colorTab = getSettingTabItem(1)
    colorTab.loadSceneSettings()
    var renderingTab = getSettingTabItem(2)
    renderingTab.loadSceneSettings()
    var bvhTab = getSettingTabItem(3)
    bvhTab.loadSceneSettings()
    var textureTab = getSettingTabItem(6)
    textureTab.loadSceneSettings()
    var surfaceModelTab = getSettingTabItem(5)
    surfaceModelTab.loadSceneSettings()
    var emitterModelTab = getSettingTabItem(4)
    emitterModelTab.loadSceneSettings()
  }

  function saveSceneSettings() {
    var systemTab = getSettingTabItem(0)
    systemTab.saveSceneSettings()
    var colorTab = getSettingTabItem(1)
    colorTab.saveSceneSettings()
    var renderingTab = getSettingTabItem(2)
    renderingTab.saveSceneSettings()
    var bvhTab = getSettingTabItem(3)
    bvhTab.saveSceneSettings()
    var textureTab = getSettingTabItem(6)
    textureTab.saveSceneSettings()
    var surfaceModelTab = getSettingTabItem(5)
    surfaceModelTab.saveSceneSettings()
    var emitterModelTab = getSettingTabItem(4)
    emitterModelTab.saveSceneSettings()
  }

  function validateData(messageList) {
    var emitterModelTab = getSettingTabItem(4)
    emitterModelTab.validateData(messageList)
    var surfaceModelTab = getSettingTabItem(5)
    surfaceModelTab.validateData(messageList)
    var textureTab = getSettingTabItem(6)
    textureTab.validateData(messageList)
  }
}
