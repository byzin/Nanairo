/*!
  \file NThreadSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: thread_setting_block

  labelText: qsTr("Threading")

  NIntSpinBox {
    id: thread_spinbox

    y: thread_setting_block.labelHeight + Nanairo.itemOffset

    maximumValue: 128
    minimumValue: 1
  }

  NButton {
    id: thread_auto_button

    y: (thread_spinbox.y + thread_spinbox.height) + Nanairo.itemOffset
    backgroundColor: thread_setting_block.color

    text: qsTr("Auto")
    onClicked: thread_spinbox.value = nanairoManager.idealThreadCount()
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.numOfThreads] = thread_spinbox.value;
    return sceneData;
  }

  function setSceneData(sceneData) {
    thread_spinbox.value = sceneData[Nanairo.numOfThreads];
  }
}
