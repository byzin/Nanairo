/*!
  \file NRoughConductorSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "../../../Items"
import "../../../definitions.js" as Definitions

NScrollView {
  id: surfaceItem

  property var textureModelList: null
  // Properties
  property bool anisotropic
  property int roughnessXIndex 
  property int roughnessYIndex 
  property int outerRefractiveIndex
  property int innerRefractiveIndex
  property int innerExtinction

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "roughness"
    }

    NCheckBox {
      id: anisotropicCheckBox

      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      text: "anisotropic"
      checked: surfaceItem.anisotropic

      onCheckedChanged: surfaceItem.anisotropic = checked
    }

    NComboBox {
      id: roughnessXComboBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.roughnessXIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.roughnessXIndex = currentIndex
    }

    NComboBox {
      id: roughnessYComboBox

      enabled: anisotropicCheckBox.checked
      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.roughnessYIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.roughnessYIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "outer refractive index"
    }

    NComboBox {
      id: outerRefractiveIndexComboBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.outerRefractiveIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.outerRefractiveIndex = currentIndex
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "inner refractive index"
    }

    NComboBox {
      id: innerRefractiveIndexComboBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.innerRefractiveIndex
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.innerRefractiveIndex = currentIndex
    }

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "inner extinction"
    }

    NComboBox {
      id: innerExtinctionComboBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.innerExtinction
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.innerExtinction = currentIndex
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.anisotropic] = false;
    item[Definitions.roughnessXIndex] = 0;
    item[Definitions.roughnessYIndex] = 0;
    item[Definitions.outerRefractiveIndex] = 0;
    item[Definitions.innerRefractiveIndex] = 0;
    item[Definitions.innerExtinction] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    anisotropic = Definitions.getProperty(item, Definitions.anisotropic);
    roughnessXIndex = Definitions.getProperty(item, Definitions.roughnessXIndex);
    roughnessYIndex = Definitions.getProperty(item, Definitions.roughnessYIndex);
    outerRefractiveIndex = Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    innerRefractiveIndex = Definitions.getProperty(item, Definitions.innerRefractiveIndex);
    innerExtinction = Definitions.getProperty(item, Definitions.innerExtinction);
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
    sceneData[Definitions.innerExtinction] =
        Definitions.getProperty(item, Definitions.innerExtinction);

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
    item[Definitions.innerExtinction] =
        Definitions.getProperty(sceneData, Definitions.innerExtinction);
  }
}
