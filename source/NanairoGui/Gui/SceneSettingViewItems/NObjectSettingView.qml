/*!
  \file NObjectSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import "../Items"
import "ObjectSettingViewItems"
import "../definitions.js" as Definitions

NPane {
  id: settingView

  readonly property int objectTreeWidth: Definitions.objectTreeWidth
  property var surfaceModelList: null
  property var emitterModelList: null
  property bool isEditMode: true

  NObjectModel {
    id: objectModel
  }

  NTransformationListModel {
    id: transformationModel
  }

  NObjectInfoSettingView {
    id: infoSettingView

    surfaceModelList: settingView.surfaceModelList
    emitterModelList: settingView.emitterModelList

    anchors.left: parent.left
    anchors.right: transformationList.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.rightMargin: Definitions.defaultItemSpace
    currentIndex: -1
  }

  NTransformationList {
    id: transformationList

    width: transformationItemWidth + 2 * viewMarginSize
    anchors.right: objectTree.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.rightMargin: Definitions.defaultItemSpace
    anchors.bottomMargin: transformationButtonView.height + Definitions.defaultItemSpace
    enabled: (model != null) && !objectTree.isMoveMode

    onCurrentIndexIsChanged: {
      settingView.updateTransformationButtons(objectTree.getCurrentIndex());
    }
  }

  NTransformationListOperationButton {
    id: transformationButtonView

    x: transformationList.x
    anchors.bottom: parent.bottom
    enabled: transformationList.enabled

    onAddTranslationButtonClicked: addTransformationItem(Definitions.translation)
    onAddScalingButtonClicked: addTransformationItem(Definitions.scaling)
    onAddRotationButtonClicked: addTransformationItem(Definitions.rotation)
    onMoveUpTransformationButtonClicked: {
      var tModel = getTransformationModel();
      if (tModel != null) {
        var source = transformationList.getCurrentIndex();
        var dest = source + 1;
        tModel.move(source, dest, 1);
      }
    }
    onMoveDownTransformationButtonClicked: {
      var tModel = getTransformationModel();
      if (tModel != null) {
        var dest = transformationList.getCurrentIndex();
        var source = dest - 1;
        tModel.move(source, dest, 1);
      }
    }
    onDeleteTransformationButtonClicked: {
      var tModel = getTransformationModel();
      if (tModel != null)
        tModel.remove(transformationList.getCurrentIndex(), 1);
    }

    function addTransformationItem(transformationType) {
      var tModel = getTransformationModel();
      if (tModel != null) {
        var transformationItem = (transformationType == Definitions.translation)
            ? transformationModel.makeTranslationItem()
            : (transformationType == Definitions.scaling)
                ? transformationModel.makeScalingItem()
                : transformationModel.makeRotationItem();
        tModel.append(transformationItem);
      }
    }

    function getTransformationModel() {
      var tModel = null;
      var itemIndex = objectTree.getCurrentIndex();
      if (objectModel.isValidIndex(itemIndex)) {
        var item = objectModel.getObject(itemIndex);
        tModel = Definitions.getProperty(item, Definitions.modelTransformationKey);
      }
      return tModel;
    }
  }

  NObjectTree {
    id: objectTree

    width: objectTreeWidth
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: operationButtionView.height + Definitions.defaultItemSpace
    model: objectModel.model

    onItemVisibleChanged: {
      // Update object tree
      objectModel.updateItemVisible();
      objectTree.syncData();
    }

    onMoveDestinationSelected: {
      var source = getCurrentIndex();
      var dest = index;
      objectModel.toChild(source, dest);

      // Update object tree
      objectModel.updateItemVisible();
      objectTree.setCurrentIndex(0);
      settingView.updateOperationButtons(getCurrentIndex());
      objectTree.syncData();

      toggleMoveMode();
    }

    onCurrentIndexIsChanged: {
      settingView.updateInfoSettingView(index);
      settingView.updateOperationButtons(index);
      settingView.updateTransformationList(index);
      settingView.updateTransformationButtons(index);
    }
  }

  NObjectTreeOperationButton {
    id: operationButtionView

    x: objectTree.x
    anchors.bottom: parent.bottom
    isMoveMode: objectTree.isMoveMode

    onAddSingleObjectButtonClicked: {
      var item = settingView.makeObject(Definitions.singleObject);
      addObject(item);
    }

    onAddGroupObjectButtonClicked: {
      var item = settingView.makeObject(Definitions.groupObject);
      addObject(item);
    }

    onMoveObjectButtonClicked: {
      var sourceItem = objectModel.getObject(objectTree.getCurrentIndex());
      Definitions.setProperty(sourceItem, Definitions.modelExpandedKey, false);

      // Update object tree
      objectTree.syncData();

      objectTree.toggleMoveMode();
    }

    onMoveUpObjectButtonClicked: {
      objectTree.toggleMoveMode();
      var dest = objectTree.getCurrentIndex();
      var source = objectModel.getPrevSiblingIndex(dest);
      objectModel.moveObject(source, dest);
      objectTree.toggleMoveMode();

      // Update object tree
      objectModel.updateItemVisible();
      objectTree.setCurrentIndex(0);
      objectTree.syncData();
      settingView.updateOperationButtons(objectTree.getCurrentIndex());
    }

    onMoveDownObjectButtonClicked: {
      objectTree.toggleMoveMode();
      var source = objectTree.getCurrentIndex();
      var dest = objectModel.getNextSiblingIndex(source);
      objectModel.moveObject(source, dest);
      objectTree.toggleMoveMode();

      // Update object tree
      objectModel.updateItemVisible();
      objectTree.setCurrentIndex(0);
      objectTree.syncData();
      settingView.updateOperationButtons(objectTree.getCurrentIndex());
    }

    onDeleteObjectButtonClicked: {
      var index = objectTree.getCurrentIndex();
      objectModel.deleteObject(index);

      // Update object tree
      objectModel.updateItemVisible();
      objectTree.setCurrentIndex(0);
      objectTree.syncData();
      settingView.updateOperationButtons(objectTree.getCurrentIndex());
    }

    function addObject(element) {
      // Init the object
      Definitions.setProperty(element, Definitions.modelGroupLevelKey, 1);
      infoSettingView.initItem(element);

      objectModel.addObject(element);

      // Update object tree
      objectModel.updateItemVisible();
      objectTree.syncData();
      settingView.updateOperationButtons(objectTree.getCurrentIndex());
    }
  }

  Component.onCompleted: updateOperationButtons(objectTree.getCurrentIndex())

  function makeObject(objectType) {
    switch (objectType) {
      case Definitions.cameraObject:
        return objectModel.makeCameraObject();
      case Definitions.groupObject:
        return objectModel.makeGroupObject();
      case Definitions.singleObject:
        return objectModel.makeSingleObject();
    }
  }

  function updateInfoSettingView(index) {
    var isValidIndex = objectModel.isValidIndex(index);
    var item = (isValidIndex) ? objectModel.getObject(index) : null;
    infoSettingView.setItem(item);
  }

  function updateOperationButtons(index) {
    var isValidIndex = objectModel.isValidIndex(index);
    var itemIsChildOfWorld = false;
    if (isValidIndex) {
      var item = objectModel.getObject(index);
      var groupLevel = Definitions.getProperty(item, Definitions.modelGroupLevelKey);
      itemIsChildOfWorld = 0 < groupLevel;
      if (itemIsChildOfWorld) {
        operationButtionView.isMovable = true;
        operationButtionView.isDeletable = true;
        // Check the object is movable up
        var prevSiblingIndex = objectModel.getPrevSiblingIndex(index);
        operationButtionView.isMovableUp = prevSiblingIndex != -1;
        // Check the object is movable down
        var nextSiblingIndex = objectModel.getNextSiblingIndex(index);
        operationButtionView.isMovableDown = nextSiblingIndex != -1;
        // Check the object is deletable
      }
    }
    if (!isValidIndex || !itemIsChildOfWorld) {
      operationButtionView.isMovable = false;
      operationButtionView.isMovableUp = false;
      operationButtionView.isMovableDown = false;
      operationButtionView.isDeletable = false;
    }
  }

  function updateTransformationList(index) {
    var isValidIndex = objectModel.isValidIndex(index);
    if (isValidIndex) {
      var item = objectModel.getObject(index);
      var tModel = Definitions.getProperty(item, Definitions.modelTransformationKey);
      transformationList.model = tModel;
    }
    else {
      transformationList.model = null;
    }
  }

  function updateTransformationButtons(index) {
    var isValidIndex = objectModel.isValidIndex(index);
    if (isValidIndex) {
      var item = objectModel.getObject(index);
      transformationButtonView.isTranslationEnabled = true;
      transformationButtonView.isScalingEnabled = Definitions.getProperty(item, Definitions.modelTypeKey) != Definitions.cameraObject;
      transformationButtonView.isRotationEnabled = true;
      var tModel = Definitions.getProperty(item, Definitions.modelTransformationKey);
      transformationButtonView.isMovableUp =
          (0 <= transformationList.getCurrentIndex()) &&
          (transformationList.getCurrentIndex() < tModel.count - 1);
      transformationButtonView.isMovableDown =
          (0 < transformationList.getCurrentIndex()) &&
          (transformationList.getCurrentIndex() < tModel.count);
      transformationButtonView.isDeletable =
          Definitions.isInBounds(transformationList.getCurrentIndex(), 0, tModel.count);
    }
    else {
      transformationButtonView.isTranslationEnabled = false;
      transformationButtonView.isScalingEnabled = false;
      transformationButtonView.isRotationEnabled = false;
      transformationButtonView.isMovableUp = false;
      transformationButtonView.isMovableDown = false;
      transformationButtonView.isDeletable = false;
    }
  }

  function getSceneData() {
    var sceneData = [];

    for (var i = 0; i < objectModel.model.count; ++i) {
      var item = objectModel.getObject(i);

      var objectData = infoSettingView.getSceneData(item);
      objectData[Definitions.name] =
          Definitions.getProperty(item, Definitions.modelNameKey);
      objectData[Definitions.enabled] =
          Definitions.getProperty(item, Definitions.modelEnabledKey);
      objectData[Definitions.groupLevel] =
          Definitions.getProperty(item, Definitions.modelGroupLevelKey);

      // Transformation
      var transformationItem =
          Definitions.getProperty(item, Definitions.modelTransformationKey);
      objectData[Definitions.transformation] =
          transformationList.getSceneData(transformationItem);

      sceneData.push(objectData);
    }

    return sceneData;
  }

  function setSceneData(sceneData) {
    objectModel.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var objectData = sceneData[i];

      var type = Definitions.getProperty(objectData, Definitions.type);
      var item = makeObject(type);

      Definitions.setProperty(item, Definitions.modelNameKey,
          Definitions.getProperty(objectData, Definitions.name));
      Definitions.setProperty(item, Definitions.modelEnabledKey,
          Definitions.getProperty(objectData, Definitions.enabled));
      Definitions.setProperty(item, Definitions.modelGroupLevelKey,
          Definitions.getProperty(objectData, Definitions.groupLevel));
      infoSettingView.setSceneData(objectData, item);

      // Transformation
      var transformationData =
          Definitions.getProperty(objectData, Definitions.transformation);
      transformationList.setSceneData(transformationData, item);

      objectModel.addObject(item);
    }

    // Update object tree
    objectTree.setCurrentIndex(1);
    objectModel.updateItemVisible();
    objectTree.syncData();
    settingView.updateOperationButtons(objectTree.getCurrentIndex());
    objectTree.setCurrentIndex(0);
  }
}
