/*!
  \file NAgglomerativeTreeletRestructuringBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NScrollView {
  id: bvhItem

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "treelet size"
    }

    NSpinBox {
      id: treeletSizeSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: bvhItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      from: 4
      to: Definitions.intMax
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "optimization loop"
    }

    NSpinBox {
      id: optimizationLoopSpinBox

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: bvhItem.width
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
