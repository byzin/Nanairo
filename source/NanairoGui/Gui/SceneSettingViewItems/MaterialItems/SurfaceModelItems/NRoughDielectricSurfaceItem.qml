/*!
  \file NRoughDielectricSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: surfaceItem

  property var textureModelList: null
  // Properties
  property bool anisotropic
  property int roughnessXIndex 
  property int roughnessYIndex 
  property int outerRefractiveIndex
  property int innerRefractiveIndex

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "roughness"
    }

    NCheckBox {
      id: anisotropicCheckBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      text: "anisotropic"
      checked: surfaceItem.anisotropic

      onCheckedChanged: surfaceItem.anisotropic = checked
    }

    NComboBox {
      id: roughnessXComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.roughnessXIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.roughnessXIndex = currentIndex
    }

    NComboBox {
      id: roughnessYComboBox

      enabled: anisotropicCheckBox.checked
      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.roughnessYIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.roughnessYIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "outer refractive index"
    }

    NComboBox {
      id: outerRefractiveIndexComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.outerRefractiveIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.outerRefractiveIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "inner refractive index"
    }

    NComboBox {
      id: innerRefractiveIndexComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.innerRefractiveIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.innerRefractiveIndex = currentIndex
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.anisotropic] = false;
    item[Definitions.roughnessXIndex] = 0;
    item[Definitions.roughnessYIndex] = 0;
    item[Definitions.outerRefractiveIndex] = 0;
    item[Definitions.innerRefractiveIndex] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    anisotropic = Definitions.getProperty(item, Definitions.anisotropic);
    roughnessXIndex = Definitions.getProperty(item, Definitions.roughnessXIndex);
    roughnessYIndex = Definitions.getProperty(item, Definitions.roughnessYIndex);
    outerRefractiveIndex = Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    innerRefractiveIndex = Definitions.getProperty(item, Definitions.innerRefractiveIndex);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.anisotropic] =
        Definitions.getProperty(item, Definitions.anisotropic);
    sceneData[Definitions.roughnessXIndex] =
        Definitions.getProperty(item, Definitions.roughnessXIndex);
    sceneData[Definitions.roughnessYIndex] =
        Definitions.getProperty(item, Definitions.roughnessYIndex);
    sceneData[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    sceneData[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.innerRefractiveIndex);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.anisotropic] =
        Definitions.getProperty(sceneData, Definitions.anisotropic);
    item[Definitions.roughnessXIndex] =
        Definitions.getProperty(sceneData, Definitions.roughnessXIndex);
    item[Definitions.roughnessYIndex] =
        Definitions.getProperty(sceneData, Definitions.roughnessYIndex);
    item[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.outerRefractiveIndex);
    item[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.innerRefractiveIndex);
  }
}
