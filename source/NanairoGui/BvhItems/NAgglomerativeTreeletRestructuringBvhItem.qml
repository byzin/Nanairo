/*!
  \file NAgglomerativeTreeletRestructuringBvhItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import ".."
import "../keyword.js" as Keyword

Rectangle {
  id: agglomerative_treelet_restructuring_bvh_item
  anchors.fill: parent
//  width: 480
//  height: 274

  readonly property int defaultTreeletSize: 9
  readonly property int defaultLoopCount: 2

  Component {
    id: spinbox_button_style
    NSpinBoxButtonStyle {
    }
  }

  Label {
    id: parameter_label
    x: 8
    y: 8
    text: qsTr("Parameter")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: treelet_size_spinbox
    x: 15
    y: 27
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignLeft
    decimals: 0
    prefix: qsTr("TreeletSize") + ": "
    maximumValue: 99
    minimumValue: 4
  }

  SpinBox {
    id: loop_count_spinbox
    x: 15
    y: 58
    width: 140
    height: 20

    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignLeft
    decimals: 0
    prefix: qsTr("Loop") + ": "
    maximumValue: 99
    minimumValue: 1
  }

  Rectangle {
    id: separator1
    x: 159
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
    border.width: 0
  }

  Rectangle {
    id: separator2
    x: 319
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
    border.width: 0
  }


  function setDefault() {
    treelet_size_spinbox.value = defaultTreeletSize
    loop_count_spinbox.value = defaultLoopCount
  }

  function loadSceneSettings(p) {
    var prefix = p + Keyword.agglomerativeTreeletRestructuringBvh + "/"
    treelet_size_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.treeletSize, 
                                defaultTreeletSize)
    loop_count_spinbox.value = 
        scene_settings.intValue(prefix + Keyword.optimizationLoopCount, 
                                 defaultLoopCount)
  }

  function saveSceneSettings(p) {
    var prefix = p + Keyword.agglomerativeTreeletRestructuringBvh + "/"
    scene_settings.setIntValue(prefix + Keyword.treeletSize, 
                               treelet_size_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.optimizationLoopCount, 
                               loop_count_spinbox.value)
  }
}
