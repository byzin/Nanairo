/*!
  \file NSurfaceSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import "../Items"
import "MaterialItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property var textureModelList: null
  property var surfaceModelList: surfaceModel.model
  property bool isEditMode: true

  NMaterialModel {
    id: surfaceModel
  }

  NSurfaceInfoSettingView {
    id: infoSettingView

    textureModelList: settingView.textureModelList

    anchors.left: parent.left
    anchors.right: surfaceList.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.rightMargin: Definitions.defaultItemSpace
  }

  NMaterialList {
    id: surfaceList

    width: surfaceList.materialListWidth
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: operationButtonView.height + Definitions.defaultItemSpace
    model: surfaceModel.model

    onCurrentIndexIsChanged: {
      settingView.updateInfoSettingView(index);
      settingView.updateOperationButton(index);
    }
  }

  NMaterialListOperationButton {
    id: operationButtonView

    x: surfaceList.x
    anchors.bottom: parent.bottom
    addButtonImagePath: "../../image/add_surface_model.png"
    addButtonDisabledImagePath: "../../image/add_surface_model_disabled.png"
    deleteButtonImagePath: "../../image/delete_surface_model.png"
    deleteButtonDisabledImagePath: "../../image/delete_surface_model_disabled.png"

    onAddMaterialButtonClicked: {
      var element = makeSurfaceMaterial();
      surfaceModel.addMaterial(element);

      // Update surface list
      settingView.updateOperationButton(surfaceList.getCurrentIndex());
    }

    onDeleteMaterialButtonClicked: {
      var index = surfaceList.getCurrentIndex();
      surfaceModel.deleteMaterial(index);

      // Update surface list
      surfaceList.setCurrentIndex(-1);
      settingView.updateOperationButton(surfaceList.getCurrentIndex());
    }
  }

  Component.onCompleted: {
    updateOperationButton(surfaceList.getCurrentIndex());
  }

  function makeSurfaceMaterial() {
    var element = surfaceModel.makeMaterial();
    Definitions.setProperty(element, Definitions.modelNameKey, "SurfaceModel");
    infoSettingView.initItem(element);
    return element;
  }

  function updateInfoSettingView(index) {
    var isValidIndex = surfaceModel.isValidIndex(index);
    var item = (isValidIndex) ? surfaceModel.getMaterial(index) : null;
    infoSettingView.setItem(item);
  }

  function updateOperationButton(index) {
    operationButtonView.isDeletable = (1 < surfaceModel.model.count) &&
                                      surfaceModel.isValidIndex(index);
  }

  function getSceneData() {
    var sceneData = [];

    for (var i = 0; i < surfaceModel.model.count; ++i) {
      var element = surfaceModel.getMaterial(i);
      var surfaceData = infoSettingView.getSceneData(element);
      surfaceData[Definitions.name] =
          Definitions.getProperty(element, Definitions.modelNameKey);
      sceneData.push(surfaceData);
    }

    return sceneData;
  }

  function setSceneData(sceneData) {
    surfaceModel.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var surfaceData = sceneData[i];

      var element = makeSurfaceMaterial();
      element[Definitions.modelNameKey] =
          Definitions.getProperty(surfaceData, Definitions.name);

      surfaceModel.addMaterial(element);
      element = surfaceModel.getMaterial(i);
      infoSettingView.setSceneData(surfaceData, element);
    }
  }
}
