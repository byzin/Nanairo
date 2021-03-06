/*!
  \file NPinholeCameraItem.qml
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
  id: cameraItem

  // Properties
  property real angleOfView: 45.0

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "angle of view (degree)"
    }

    NFloatSpinBox {
      id: angleSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: cameraItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      floatFrom: 0.0001
      floatTo: 89.9999
      floatValue: cameraItem.angleOfView

      onFloatValueChanged: cameraItem.angleOfView = floatValue
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
