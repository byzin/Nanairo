/*!
  \file NSceneSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.11
import "Items"
import "SceneSettingViewItems"
import "definitions.js" as Definitions

NPane {
  id: settingView

  property int currentIndex: 0
  property bool isEditMode: true

  StackLayout {
    id: settingLayout

    anchors.fill: parent
    anchors.margins: Definitions.defaultItemSpace
    currentIndex: settingView.currentIndex

    NTagSettingView {
      id: tagSettingView
      isEditMode: settingView.isEditMode
    }

    NSystemSettingView {
      id: systemSettingView
      isEditMode: settingView.isEditMode
    }

    NColorSettingView {
      id: colorSettingView
      isEditMode: settingView.isEditMode
    }

    NMethodSettingView {
      id: methodSettingView
      isEditMode: settingView.isEditMode
    }

    NTextureSettingView {
      id: textureSettingView
      isEditMode: settingView.isEditMode
    }

    NSurfaceSettingView {
      id: surfaceSettingView
      isEditMode: settingView.isEditMode
      textureModelList: textureSettingView.textureModelList
    }

    NEmitterSettingView {
      id: emitterSettingView
      isEditMode: settingView.isEditMode
      textureModelList: textureSettingView.textureModelList
    }

    NObjectSettingView {
      id: objectSettingView
      isEditMode: settingView.isEditMode
      surfaceModelList: surfaceSettingView.surfaceModelList
      emitterModelList: emitterSettingView.emitterModelList
    }

    NBvhSettingView {
      id: bvhSettingView
      isEditMode: settingView.isEditMode
    }
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.scene] = tagSettingView.getSceneData();
    sceneData[Definitions.system] = systemSettingView.getSceneData();
    sceneData[Definitions.color] = colorSettingView.getSceneData();
    sceneData[Definitions.renderingMethod] = methodSettingView.getSceneData();
    sceneData[Definitions.textureModel] = textureSettingView.getSceneData();
    sceneData[Definitions.surfaceModel] = surfaceSettingView.getSceneData();
    sceneData[Definitions.emitterModel] = emitterSettingView.getSceneData();
    sceneData[Definitions.object] = objectSettingView.getSceneData();
    sceneData[Definitions.bvh] = bvhSettingView.getSceneData();

    return sceneData;
  }

  function getSceneName() {
    return tagSettingView.getSceneName();
  }

  function getImageResolution() {
    return systemSettingView.getImageResolution();
  }

  function setSceneData(sceneData) {
    isEditMode = false;

    var tagData = Definitions.getProperty(sceneData, Definitions.scene);
    tagSettingView.setSceneData(tagData);

    var systemData = Definitions.getProperty(sceneData, Definitions.system);
    systemSettingView.setSceneData(systemData);

    var colorData = Definitions.getProperty(sceneData, Definitions.color);
    colorSettingView.setSceneData(colorData);

    var methodData = Definitions.getProperty(sceneData, Definitions.renderingMethod);
    methodSettingView.setSceneData(methodData);

    var textureData = Definitions.getProperty(sceneData, Definitions.textureModel);
    textureSettingView.setSceneData(textureData);

    var surfaceData = Definitions.getProperty(sceneData, Definitions.surfaceModel);
    surfaceSettingView.setSceneData(surfaceData);

    var emitterData = Definitions.getProperty(sceneData, Definitions.emitterModel);
    emitterSettingView.setSceneData(emitterData);

    var objectData = Definitions.getProperty(sceneData, Definitions.object);
    objectSettingView.setSceneData(objectData);

    var bvhData = Definitions.getProperty(sceneData, Definitions.bvh);
    bvhSettingView.setSceneData(bvhData);

    isEditMode = true;
  }
}
