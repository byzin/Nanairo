/*!
  \file NSmoothDiffuseSurfaceItem.qml
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
  property int reflectanceIndex

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
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.reflectanceIndex] = 0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    reflectanceIndex = Definitions.getProperty(item, Definitions.reflectanceIndex);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.reflectanceIndex] =
        Definitions.getProperty(item, Definitions.reflectanceIndex);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.reflectanceIndex] =
        Definitions.getProperty(sceneData, Definitions.reflectanceIndex);
  }
}
