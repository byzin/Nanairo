/*!
  \file NTransformationList.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: transformationList

  readonly property int viewBorderWidth: 2
  readonly property int viewMarginSize: viewBorderWidth + 1
  readonly property int transformationItemWidth: 240
  property var model: null

  ListView {
    id: transformationListView

    anchors.fill: parent
    leftMargin: transformationList.viewMarginSize
    rightMargin: transformationList.viewMarginSize
    topMargin: transformationList.viewMarginSize
    bottomMargin: transformationList.viewMarginSize
    clip: true
    model: transformationList.model
    verticalLayoutDirection: ListView.BottomToTop

    delegate: NTransformationListItem {
      width: transformationList.width - 2 * transformationList.viewMarginSize
      backgroundColor: transformationList.background.color
      isFocused: ListView.isCurrentItem
      itemIndex: model.index
      itemEnabled: model[Definitions.modelEnabledKey]
      itemTransformationType: model[Definitions.modelTypeKey]
      itemX: model[Definitions.modelXAxisKey]
      itemY: isRotationTransformation ? model[Definitions.modelAngleKey] : model[Definitions.modelYAxisKey]
      itemZ: model[Definitions.modelZAxisKey]
      itemAxis: model[Definitions.modelAxisKey]
      itemUnit: model[Definitions.modelUnitKey]

      onItemEnabledChanged: setProperty(Definitions.modelEnabledKey, itemEnabled)

      onItemXChanged: {
        if (!isRotationTransformation)
          setProperty(Definitions.modelXAxisKey, itemX);
      }

      onItemYChanged: {
        if (!isRotationTransformation)
          setProperty(Definitions.modelYAxisKey, itemY);
        else
          setProperty(Definitions.modelAngleKey, itemY);
      }

      onItemZChanged: {
        if (!isRotationTransformation)
          setProperty(Definitions.modelZAxisKey, itemZ);
      }

      onItemAxisChanged: {
        if (isRotationTransformation && (itemAxis != ""))
          setProperty(Definitions.modelAxisKey, itemAxis);
      }

      onItemUnitChanged: {
        if (isRotationTransformation && (itemUnit != ""))
          setProperty(Definitions.modelUnitKey, itemUnit);
      }

      onItemClicked: transformationListView.currentIndex = itemIndex

      function setProperty(propertyName, value) {
        var tModel = transformationList.model;
        if (tModel != null) {
          if (Definitions.isInBounds(itemIndex, 0, tModel.count)) {
            var item = tModel.get(itemIndex);
            Definitions.setProperty(item, propertyName, value);
          }
        }
      }
    }

    onCurrentIndexChanged: transformationList.currentIndexIsChanged(currentIndex)
  }

  Component.onCompleted: {
    // Initialize the background
    background.border.width = viewBorderWidth;
    background.border.color = Material.color(Material.BlueGrey);
  }

  function getCurrentIndex() {
    return transformationListView.currentIndex;
  }

  function getSceneData(item) {
    var sceneData = [];

    for (var i = 0; i < item.count; ++i) {
      var tItem = item.get(i);
      var tData = {};

      var tType = Definitions.getProperty(tItem, Definitions.modelTypeKey);
      tData[Definitions.type] = tType;
      tData[Definitions.enabled] =
          Definitions.getProperty(tItem, Definitions.modelEnabledKey);

      if (tType == Definitions.rotation) {
        tData[Definitions.axis] =
            Definitions.getProperty(tItem, Definitions.modelAxisKey);
        tData[Definitions.angle] =
            Definitions.getProperty(tItem, Definitions.modelAngleKey);
        tData[Definitions.unit] =
            Definitions.getProperty(tItem, Definitions.modelUnitKey);
      }
      else {
        var value = [Definitions.getProperty(tItem, Definitions.modelXAxisKey),
                     Definitions.getProperty(tItem, Definitions.modelYAxisKey),
                     Definitions.getProperty(tItem, Definitions.modelZAxisKey)];
        tData[Definitions.value] = value;
      }

      sceneData.push(tData);
    }

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var tList = [];
    for (var i = 0; i < sceneData.length; ++i) {
      var tItem = {};
      var tData = sceneData[i];

      var tType = Definitions.getProperty(tData, Definitions.type);
      tItem[Definitions.modelTypeKey] = tType;
      tItem[Definitions.modelEnabledKey] =
          Definitions.getProperty(tData, Definitions.enabled);

      if (tType == Definitions.rotation) {
        tItem[Definitions.modelXAxisKey] = 0.0;
        tItem[Definitions.modelYAxisKey] = 0.0;
        tItem[Definitions.modelZAxisKey] = 0.0;
        tItem[Definitions.modelAxisKey] =
            Definitions.getProperty(tData, Definitions.axis);
        tItem[Definitions.modelAngleKey] =
            Definitions.getProperty(tData, Definitions.angle);
        tItem[Definitions.modelUnitKey] =
            Definitions.getProperty(tData, Definitions.unit);
      }
      else {
        var value = Definitions.getProperty(tData, Definitions.value);
        tItem[Definitions.modelXAxisKey] = value[0];
        tItem[Definitions.modelYAxisKey] = value[1];
        tItem[Definitions.modelZAxisKey] = value[2];
        tItem[Definitions.modelAxisKey] = Definitions.xAxis;
        tItem[Definitions.modelAngleKey] = 0.0;
        tItem[Definitions.modelUnitKey] = Definitions.degreeUnit;
      }

      tList.push(tItem);
    }
    item[Definitions.modelTransformationKey] = tList;
  }

  signal currentIndexIsChanged(int index)
}
