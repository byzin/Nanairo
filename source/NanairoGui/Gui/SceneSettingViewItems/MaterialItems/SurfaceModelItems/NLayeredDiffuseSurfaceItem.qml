/*!
  \file NLayeredDiffuseSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: surfaceItem

  property var textureModelList: null
  // Properties
  property int reflectanceIndex
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
      text: "reflectance"
    }

    NComboBox {
      id: reflectanceComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.reflectanceIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.reflectanceIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
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
    item[Definitions.reflectanceIndex] = 0;
    item[Definitions.anisotropic] = false;
    item[Definitions.roughnessXIndex] = 0;
    item[Definitions.roughnessYIndex] = 0;
    item[Definitions.outerRefractiveIndex] = 0;
    item[Definitions.innerRefractiveIndex] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    reflectanceIndex = Definitions.getProperty(item, Definitions.reflectanceIndex);
    anisotropic = Definitions.getProperty(item, Definitions.anisotropic);
    roughnessXIndex = Definitions.getProperty(item, Definitions.roughnessXIndex);
    roughnessYIndex = Definitions.getProperty(item, Definitions.roughnessYIndex);
    outerRefractiveIndex = Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    innerRefractiveIndex = Definitions.getProperty(item, Definitions.innerRefractiveIndex);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.reflectanceIndex] =
        Definitions.getProperty(item, Definitions.reflectanceIndex);
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
    item[Definitions.reflectanceIndex] =
        Definitions.getProperty(sceneData, Definitions.reflectanceIndex);
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
