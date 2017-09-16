/*!
  \file NSceneSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "Styles"
import "nanairo.js" as Nanairo

Rectangle {
  id: scene_setting_view

  width: Nanairo.mainWindowWidth
  height: Nanairo.mainWindowHeight

  TabView {
    id: setting_tab_view

    x: 0
    y: 8
    width: scene_setting_view.width
    height: scene_setting_view.height - y

    style: tab_view_style

    Tab {
      title: Nanairo.scene
      active: true
      NSceneTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.system
      active: true
      NSystemTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.color
      active: true
      NColorTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.renderingMethod
      active: true
      NRenderingMethodTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.textureModel
      active: true
      NTextureModelTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.surfaceModel
      active: true
      NSurfaceModelTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.emitterModel
      active: true
      NEmitterModelTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.object
      active: true
      NObjectTabItem {
        color: scene_setting_view.color
      }
    }

    Tab {
      title: Nanairo.bvh
      active: true
      NBvhTabItem {
        color: scene_setting_view.color
      }
    }

    onCountChanged: {
      var textureTabIndex = 4;
      var surfaceTabIndex = 5;
      if ((count - 1) == surfaceTabIndex) {
        var textureTab = getSettingTab(textureTabIndex);
        var surfaceTab = getSettingTab(surfaceTabIndex);
        surfaceTab.textureModel = textureTab.materialModel.model;
      }
      var emitterTabIndex = 6;
      if ((count - 1) == emitterTabIndex) {
        var textureTab = getSettingTab(textureTabIndex);
        var emitterTab = getSettingTab(emitterTabIndex);
        emitterTab.textureModel = textureTab.materialModel.model;
      }
      var objectTabIndex = 7
      if ((count - 1) == objectTabIndex) {
        var surfaceTab = getSettingTab(surfaceTabIndex);
        var emitterTab = getSettingTab(emitterTabIndex);
        var objectTab = getSettingTab(objectTabIndex);
        objectTab.emitterModel = emitterTab.materialModel.model;
        objectTab.surfaceModel = surfaceTab.materialModel.model;
      }
    }
  }

  Component {
    id: tab_view_style

    NTabViewStyle {
      backgroundColor: scene_setting_view.color
    }
  }

  function getSettingTab(tabIndex) {
    return setting_tab_view.getTab(tabIndex).item;
  }

  function getSceneName() {
    var sceneTab = getSettingTab(0);
    return sceneTab.getSceneName();
  }

  function getImageResolution() {
    var systemTab = getSettingTab(1);
    return systemTab.getImageResolution();
  }

  function getSceneData() {
    var sceneData = {};
    // Scene settings
    var sceneTab = getSettingTab(0);
    sceneData[Nanairo.scene] = sceneTab.getSceneData();
    // System settings
    var systemTab = getSettingTab(1);
    sceneData[Nanairo.system] = systemTab.getSceneData();
    // Color settings
    var colorTab = getSettingTab(2);
    sceneData[Nanairo.color] = colorTab.getSceneData();
    // RenderingMethod settings
    var renderingMethodTab = getSettingTab(3);
    sceneData[Nanairo.renderingMethod] = renderingMethodTab.getSceneData();
    // TextureModel settings
    var textureModelTab = getSettingTab(4);
    sceneData[Nanairo.textureModel] = textureModelTab.getSceneData();
    // SurfaceModel settings
    var surfaceModelTab = getSettingTab(5);
    sceneData[Nanairo.surfaceModel] = surfaceModelTab.getSceneData();
    // EmitterModel settings
    var emitterModelTab = getSettingTab(6);
    sceneData[Nanairo.emitterModel] = emitterModelTab.getSceneData();
    // Object settings
    var objectTab = getSettingTab(7);
    sceneData[Nanairo.object] = objectTab.getSceneData();
    // BVH settings
    var bvhTab = getSettingTab(8);
    sceneData[Nanairo.bvh] = bvhTab.getSceneData();
    return sceneData;
  }

  function setSceneData(sceneData) {
    // Scene settings
    var sceneTab = getSettingTab(0);
    sceneTab.setSceneData(sceneData[Nanairo.scene]);
    // System settings
    var systemTab = getSettingTab(1);
    systemTab.setSceneData(sceneData[Nanairo.system]);
    // Color settings
    var colorTab = getSettingTab(2);
    colorTab.setSceneData(sceneData[Nanairo.color]);
    // RenderingMethod settings
    var renderingMethodTab = getSettingTab(3);
    renderingMethodTab.setSceneData(sceneData[Nanairo.renderingMethod]);
    // TextureModel settings
    var textureModelTab = getSettingTab(4);
    textureModelTab.setSceneData(sceneData[Nanairo.textureModel]);
    // SurfaceModel settings
    var surfaceModelTab = getSettingTab(5);
    surfaceModelTab.setSceneData(sceneData[Nanairo.surfaceModel]);
    // EmitterModel settings
    var emitterModelTab = getSettingTab(6);
    emitterModelTab.setSceneData(sceneData[Nanairo.emitterModel]);
    // Object settings
    var objectTab = getSettingTab(7);
    objectTab.setSceneData(sceneData[Nanairo.object]);
    // BVH settings
    var bvhTab = getSettingTab(8);
    bvhTab.setSceneData(sceneData[Nanairo.bvh]);
  }
}
