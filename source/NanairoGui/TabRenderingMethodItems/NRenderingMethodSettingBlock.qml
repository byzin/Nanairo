/*!
  \file NRenderingMethodSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: rendering_method_setting_block
  readonly property int currentMethodIndex: rendering_method_type_combobox.currentIndex

  labelText: qsTr("RenderingMethod")

  NComboBox {
    id: rendering_method_type_combobox

    y: rendering_method_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: rendering_method_setting_block.color

    model: [Nanairo.pathTracing,
            Nanairo.lightTracing,
            Nanairo.probabilisticPpm]
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.type] = rendering_method_type_combobox.currentText;
    return sceneData;
  }

  function setSceneData(sceneData) {
    rendering_method_type_combobox.currentIndex = 
        rendering_method_type_combobox.find(sceneData[Nanairo.type]);
  }
}
