/*!
  \file NTextureSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import "../Items"
import "MaterialItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property var textureModelList: textureModel.model
  property bool isEditMode: true

  NMaterialModel {
    id: textureModel
  }

  NTextureInfoSettingView {
    id: infoSettingView

    anchors.left: parent.left
    anchors.right: textureList.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.rightMargin: Definitions.defaultItemSpace
  }

  NMaterialList {
    id: textureList

    width: textureList.materialListWidth
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: operationButtonView.height + Definitions.defaultItemSpace
    model: textureModel.model

    onCurrentIndexIsChanged: {
      settingView.updateInfoSettingView(index);
      settingView.updateOperationButton(index);
    }
  }

  NMaterialListOperationButton {
    id: operationButtonView

    x: textureList.x
    anchors.bottom: parent.bottom
    addButtonImagePath: "../../image/add_texture_model.png"
    addButtonDisabledImagePath: "../../image/add_texture_model_disabled.png"
    deleteButtonImagePath: "../../image/delete_texture_model.png"
    deleteButtonDisabledImagePath: "../../image/delete_texture_model_disabled.png"

    onAddMaterialButtonClicked: {
      var element = makeTextureMaterial();
      textureModel.addMaterial(element);

      // Update texture list
      settingView.updateOperationButton(textureList.getCurrentIndex());
    }

    onDeleteMaterialButtonClicked: {
      var index = textureList.getCurrentIndex();
      textureModel.deleteMaterial(index);

      // Update texture list
      textureList.setCurrentIndex(-1);
      settingView.updateOperationButton(textureList.getCurrentIndex());
    }
  }

  Component.onCompleted: {
    updateOperationButton(textureList.getCurrentIndex());
  }

  function makeTextureMaterial() {
    var element = textureModel.makeMaterial();
    Definitions.setProperty(element, Definitions.modelNameKey, "TextureModel");
    infoSettingView.initItem(element);
    return element;
  }

  function updateInfoSettingView(index) {
    var isValidIndex = textureModel.isValidIndex(index);
    var item = (isValidIndex) ? textureModel.getMaterial(index) : null;
    infoSettingView.setItem(item);
  }

  function updateOperationButton(index) {
    operationButtonView.isDeletable = (1 < textureModel.model.count) &&
                                      textureModel.isValidIndex(index);
  }

  function getSceneData() {
    var sceneData = [];

    for (var i = 0; i < textureModel.model.count; ++i) {
      var element = textureModel.getMaterial(i);
      var textureData = infoSettingView.getSceneData(element);
      textureData[Definitions.name] =
          Definitions.getProperty(element, Definitions.modelNameKey);
      sceneData.push(textureData);
    }

    return sceneData;
  }

  function setSceneData(sceneData) {
    textureModel.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var textureData = sceneData[i];

      var element = makeTextureMaterial();
      element[Definitions.modelNameKey] =
          Definitions.getProperty(textureData, Definitions.name);

      textureModel.addMaterial(element);
      element = textureModel.getMaterial(i);
      infoSettingView.setSceneData(textureData, element);
    }
  }
}
