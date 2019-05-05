/*!
  \file NSmoothDielectricSurfaceItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
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
  property int outerRefractiveIndex
  property int innerRefractiveIndex

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
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
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.outerRefractiveIndex] = 0;
    item[Definitions.innerRefractiveIndex] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    outerRefractiveIndex = Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    innerRefractiveIndex = Definitions.getProperty(item, Definitions.innerRefractiveIndex);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.outerRefractiveIndex);
    sceneData[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(item, Definitions.innerRefractiveIndex);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.outerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.outerRefractiveIndex);
    item[Definitions.innerRefractiveIndex] =
        Definitions.getProperty(sceneData, Definitions.innerRefractiveIndex);
  }
}
