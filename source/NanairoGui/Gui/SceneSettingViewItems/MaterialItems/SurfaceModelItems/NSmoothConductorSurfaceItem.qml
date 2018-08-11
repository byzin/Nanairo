/*!
  \file NSmoothConductorSurfaceItem.qml
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
  property int outerRefractiveIndex
  property int innerRefractiveIndex
  property int innerExtinction

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
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

    NLabel {
      text: "inner extinction"
    }

    NComboBox {
      id: innerExtinctionComboBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      currentIndex: surfaceItem.innerExtinction
      model: surfaceItem.textureModelList
      textRole: Definitions.modelNameKey

      onCurrentIndexChanged: surfaceItem.innerExtinction = currentIndex
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.outerRefractiveIndex] = 0;
    item[Definitions.innerRefractiveIndex] = 0;
    item[Definitions.innerExtinction] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    outerRefractiveIndex = Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    innerRefractiveIndex = Definitions.getProperty(item, Definitions.innerRefractiveIndex);
    innerExtinction = Definitions.getProperty(item, Definitions.innerExtinction);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    sceneData[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.innerRefractiveIndex);
    sceneData[Definitions.innerExtinction] =
        Definitions.getProperty(item, Definitions.innerExtinction);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.outerRefractiveIndex);
    item[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.innerRefractiveIndex);
    item[Definitions.innerExtinction] =
        Definitions.getProperty(sceneData, Definitions.innerExtinction);
  }
}
