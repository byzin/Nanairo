/*!
  \file NSurfaceInfoSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../Items"
import "SurfaceModelItems"
import "../../definitions.js" as Definitions

NPane {
  id: infoSettingView

  property var textureModelList: null
  property var materialItem: null
  property string surfaceType: ""

  GridLayout {
    columns: 3
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "surface type"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: surfaceTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: find(infoSettingView.surfaceType)
          model: [Definitions.smoothDiffuseSurface,
                  Definitions.smoothDielectricSurface,
                  Definitions.smoothConductorSurface,
                  Definitions.roughDielectricSurface,
                  Definitions.roughConductorSurface,
                  Definitions.layeredDiffuseSurface]

          onCurrentTextChanged: infoSettingView.surfaceType = currentText
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "surface parameters"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: surfaceItemLayout

        anchors.fill: parent
        currentIndex: 0

        NSmoothDiffuseSurfaceItem {
          id: smoothDiffuseSurfaceItem
          textureModelList: infoSettingView.textureModelList
          onReflectanceIndexChanged: infoSettingView.setProperty(Definitions.reflectanceIndex, reflectanceIndex)
        }

        NSmoothDielectricSurfaceItem {
          id: smoothDielectricSurfaceItem
          textureModelList: infoSettingView.textureModelList
          onOuterRefractiveIndexChanged: infoSettingView.setProperty(Definitions.outerRefractiveIndex, outerRefractiveIndex)
          onInnerRefractiveIndexChanged: infoSettingView.setProperty(Definitions.innerRefractiveIndex, innerRefractiveIndex)
        }

        NSmoothConductorSurfaceItem {
          id: smoothConductorSurfaceItem
          textureModelList: infoSettingView.textureModelList
          onOuterRefractiveIndexChanged: infoSettingView.setProperty(Definitions.outerRefractiveIndex, outerRefractiveIndex)
          onInnerRefractiveIndexChanged: infoSettingView.setProperty(Definitions.innerRefractiveIndex, innerRefractiveIndex)
          onInnerExtinctionChanged: infoSettingView.setProperty(Definitions.innerExtinction, innerExtinction)
        }

        NRoughDielectricSurfaceItem {
          id: roughDielectricSurfaceItem 
          textureModelList: infoSettingView.textureModelList
          onAnisotropicChanged: infoSettingView.setProperty(Definitions.anisotropic, anisotropic);
          onRoughnessXIndexChanged: infoSettingView.setProperty(Definitions.roughnessXIndex, roughnessXIndex);
          onRoughnessYIndexChanged: infoSettingView.setProperty(Definitions.roughnessYIndex, roughnessYIndex);
          onOuterRefractiveIndexChanged: infoSettingView.setProperty(Definitions.outerRefractiveIndex, outerRefractiveIndex)
          onInnerRefractiveIndexChanged: infoSettingView.setProperty(Definitions.innerRefractiveIndex, innerRefractiveIndex)
        }

        NRoughConductorSurfaceItem {
          id: roughConductorSurfaceItem 
          textureModelList: infoSettingView.textureModelList
          onAnisotropicChanged: infoSettingView.setProperty(Definitions.anisotropic, anisotropic);
          onRoughnessXIndexChanged: infoSettingView.setProperty(Definitions.roughnessXIndex, roughnessXIndex);
          onRoughnessYIndexChanged: infoSettingView.setProperty(Definitions.roughnessYIndex, roughnessYIndex);
          onOuterRefractiveIndexChanged: infoSettingView.setProperty(Definitions.outerRefractiveIndex, outerRefractiveIndex)
          onInnerRefractiveIndexChanged: infoSettingView.setProperty(Definitions.innerRefractiveIndex, innerRefractiveIndex)
          onInnerExtinctionChanged: infoSettingView.setProperty(Definitions.innerExtinction, innerExtinction)
        }

        NLayeredDiffuseSurfaceItem {
          id: layeredDiffuseSurfaceItem 
          textureModelList: infoSettingView.textureModelList
          onReflectanceIndexChanged: infoSettingView.setProperty(Definitions.reflectanceIndex, reflectanceIndex)
          onAnisotropicChanged: infoSettingView.setProperty(Definitions.anisotropic, anisotropic);
          onRoughnessXIndexChanged: infoSettingView.setProperty(Definitions.roughnessXIndex, roughnessXIndex);
          onRoughnessYIndexChanged: infoSettingView.setProperty(Definitions.roughnessYIndex, roughnessYIndex);
          onOuterRefractiveIndexChanged: infoSettingView.setProperty(Definitions.outerRefractiveIndex, outerRefractiveIndex)
          onInnerRefractiveIndexChanged: infoSettingView.setProperty(Definitions.innerRefractiveIndex, innerRefractiveIndex)
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < surfaceItemLayout.children.length; ++i) {
          var surfaceView = surfaceItemLayout.children[i];
          surfaceView.background.color = background.color;
        }
      }
    }
  }

  onSurfaceTypeChanged: {
    var viewIndex = surfaceTypeComboBox.find(surfaceType);
    if ((materialItem != null) && (viewIndex != -1)) {
      // Set base surface properties
      setProperty(Definitions.type, surfaceType);
      // Set other emitter properties
      var settingView = surfaceItemLayout.children[viewIndex];
      settingView.initItem(materialItem);
      // Change the surface setting view
      surfaceItemLayout.currentIndex = viewIndex;
      // Set the surface properties to the view
      settingView.setValue(materialItem);
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    // Set the properties of smooth diffuse surface item
    item[Definitions.type] = Definitions.smoothDiffuseSurface;
    smoothDiffuseSurfaceItem.initItem(item);
  }

  function setProperty(propertyName, value) {
    if (materialItem != null)
      Definitions.setProperty(materialItem, propertyName, value);
  }

  function setItem(item) {
    materialItem = null;
    if (item != null) {
      // Set surface type to the view
      surfaceType = Definitions.getProperty(item, Definitions.type);
      // Change the emitter setting view
      var viewIndex = surfaceTypeComboBox.find(surfaceType);
      console.assert(viewIndex != -1, "The item has invalid surface type.");
      surfaceItemLayout.currentIndex = viewIndex;
      // Set the item properties to thew 
      var settingView = surfaceItemLayout.children[viewIndex];
      settingView.setValue(item);
    }
    materialItem = item;
  }

  function getSceneData(item) {
    var type = Definitions.getProperty(item, Definitions.type);

    var viewIndex = surfaceTypeComboBox.find(type);
    var surfaceView = surfaceItemLayout.children[viewIndex];

    var sceneData = surfaceView.getSceneData(item);
    sceneData[Definitions.type] = type;

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var type = Definitions.getProperty(sceneData, Definitions.type);

    var viewIndex = surfaceTypeComboBox.find(type);
    var surfaceView = surfaceItemLayout.children[viewIndex];

    item[Definitions.type] = type;
    surfaceView.setSceneData(sceneData, item);
  }
}
