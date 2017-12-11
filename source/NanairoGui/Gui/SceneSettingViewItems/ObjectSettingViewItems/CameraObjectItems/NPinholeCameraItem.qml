/*!
  \file NPinholeCameraItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: pinholeCameraItem

  // Properties
  property real angleOfView: 45.0

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "angle of view (degree)"
    }

    NFloatSpinBox {
      id: angleSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      decimals: 4
      floatFrom: 0.0001
      floatTo: 89.9999
      floatValue: pinholeCameraItem.angleOfView

      onFloatValueChanged: pinholeCameraItem.angleOfView = floatValue
    }
  }

  onAngleOfViewChanged: angleSpinBox.floatValue = angleOfView

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.angleOfView] = 45.0;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    angleOfView = Definitions.getProperty(item, Definitions.angleOfView);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.angleOfView] =
        Definitions.getProperty(item, Definitions.angleOfView);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.angleOfView] =
        Definitions.getProperty(sceneData, Definitions.angleOfView);
  }
}
