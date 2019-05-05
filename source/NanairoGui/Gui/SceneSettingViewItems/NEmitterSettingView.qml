/*!
  \file NEmitterSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import "../Items"
import "MaterialItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  property var textureModelList: null
  property var emitterModelList: emitterModel.model
  property bool isEditMode: true

  NMaterialModel {
    id: emitterModel
  }

  NEmitterInfoSettingView {
    id: infoSettingView

    textureModelList: settingView.textureModelList

    anchors.left: parent.left
    anchors.right: emitterList.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.rightMargin: Definitions.defaultItemSpace
  }

  NMaterialList {
    id: emitterList

    width: emitterList.materialListWidth
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: operationButtonView.height + Definitions.defaultItemSpace
    model: emitterModel.model

    onCurrentIndexIsChanged: {
      settingView.updateInfoSettingView(index);
      settingView.updateOperationButton(index);
    }
  }

  NMaterialListOperationButton {
    id: operationButtonView

    x: emitterList.x
    anchors.bottom: parent.bottom
    addButtonImagePath: "../../image/add_emitter_model.png"
    addButtonDisabledImagePath: "../../image/add_emitter_model_disabled.png"
    deleteButtonImagePath: "../../image/delete_emitter_model.png"
    deleteButtonDisabledImagePath: "../../image/delete_emitter_model_disabled.png"

    onAddMaterialButtonClicked: {
      var element = makeEmitterMaterial();
      emitterModel.addMaterial(element);

      // Update emitter list
      settingView.updateOperationButton(emitterList.getCurrentIndex());
    }

    onDeleteMaterialButtonClicked: {
      var index = emitterList.getCurrentIndex();
      emitterModel.deleteMaterial(index);

      // Update emitter list
      emitterList.setCurrentIndex(-1);
      settingView.updateOperationButton(emitterList.getCurrentIndex());
    }
  }

  Component.onCompleted: {
    updateOperationButton(emitterList.getCurrentIndex());
  }

  function makeEmitterMaterial() {
    var element = emitterModel.makeMaterial();
    Definitions.setProperty(element, Definitions.modelNameKey, "EmitterModel");
    infoSettingView.initItem(element);
    return element;
  }

  function updateInfoSettingView(index) {
    var isValidIndex = emitterModel.isValidIndex(index);
    var item = (isValidIndex) ? emitterModel.getMaterial(index) : null;
    infoSettingView.setItem(item);
  }

  function updateOperationButton(index) {
    operationButtonView.isDeletable = (1 < emitterModel.model.count) &&
                                      emitterModel.isValidIndex(index);
  }

  function getSceneData() {
    var sceneData = [];

    for (var i = 0; i < emitterModel.model.count; ++i) {
      var element = emitterModel.getMaterial(i);
      var emitterData = infoSettingView.getSceneData(element);
      emitterData[Definitions.name] =
          Definitions.getProperty(element, Definitions.modelNameKey);
      sceneData.push(emitterData);
    }

    return sceneData;
  }

  function setSceneData(sceneData) {
    emitterModel.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var emitterData = sceneData[i];

      var element = makeEmitterMaterial();
      element[Definitions.modelNameKey] =
          Definitions.getProperty(emitterData, Definitions.name);

      emitterModel.addMaterial(element);
      element = emitterModel.getMaterial(i);
      infoSettingView.setSceneData(emitterData, element);
    }
  }
}
