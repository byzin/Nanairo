/*!
  \file NObjectInfoSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

StackLayout {
  id: infoSettingView

  property var surfaceModelList: null
  property var emitterModelList: null

  NCameraSettingView {
    id: cameraSettingView
  }

  NSingleObjectSettingView {
    id: objectSettingView
    surfaceModelList: infoSettingView.surfaceModelList
    emitterModelList: infoSettingView.emitterModelList
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    var objectType = Definitions.getProperty(item, Definitions.modelTypeKey);
    if (objectType == Definitions.cameraObject)
      cameraSettingView.initItem(item);
    if (objectType == Definitions.singleObject)
      objectSettingView.initItem(item);
  }

  function setItem(item) {
    if (item != null) {
      var objectType = Definitions.getProperty(item, Definitions.modelTypeKey);
      cameraSettingView.setItem((objectType == Definitions.cameraObject) ? item : null);
      objectSettingView.setItem((objectType == Definitions.singleObject) ? item : null);
      currentIndex = (objectType == Definitions.cameraObject) ? 0 :
                     (objectType == Definitions.singleObject) ? 1
                                                              : -1;
    }
    else{
      cameraSettingView.setItem(null);
      objectSettingView.setItem(null);
      currentIndex = -1;
    }
  }

  function getSceneData(item) {
    var sceneData = {};

    var type = Definitions.getProperty(item, Definitions.modelTypeKey);
    switch (type) {
      case Definitions.cameraObject: {
        sceneData = cameraSettingView.getSceneData(item);
        break;
      }
      case Definitions.singleObject: {
        sceneData = objectSettingView.getSceneData(item);
        break;
      }
    }
    sceneData[Definitions.type] = type;

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var type = Definitions.getProperty(sceneData, Definitions.type);
    switch (type) {
      case Definitions.cameraObject: {
        cameraSettingView.setSceneData(sceneData, item);
        break;
      }
      case Definitions.singleObject: {
        objectSettingView.setSceneData(sceneData, item);
        break;
      }
    }
  }
}
