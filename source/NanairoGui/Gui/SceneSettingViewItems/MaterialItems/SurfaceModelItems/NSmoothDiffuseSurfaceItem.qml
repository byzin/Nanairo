/*!
  \file NSmoothDiffuseSurfaceItem.qml
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

NScrollView {
  id: surfaceItem

  property var textureModelList: null
  // Properties
  property int reflectanceIndex

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "reflectance"
    }

    NComboBox {
      id: reflectanceComboBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: surfaceItem.width
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
