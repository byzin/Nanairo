/*!
  \file NAgglomerativeTreeletRestructuringBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: bvhItem

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "treelet size"
    }

    NSpinBox {
      id: treeletSizeSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 4
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "optimization loop"
    }

    NSpinBox {
      id: optimizationLoopSpinBox

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 1
      to: Definitions.intMax
    }
  }

  function getSceneData() {
    var sceneData = {};

    sceneData[Definitions.treeletSize] = treeletSizeSpinBox.value;
    sceneData[Definitions.optimizationLoopCount] = optimizationLoopSpinBox.value;

    return sceneData;
  }

  function initSceneData() {
    treeletSizeSpinBox.value = 9;
    optimizationLoopSpinBox.value = 2;
  }

  function setSceneData(sceneData) {
    treeletSizeSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.treeletSize);
    optimizationLoopSpinBox.value =
        Definitions.getProperty(sceneData, Definitions.optimizationLoopCount);
  }
}
