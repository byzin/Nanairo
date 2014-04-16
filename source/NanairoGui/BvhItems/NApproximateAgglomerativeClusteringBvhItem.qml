/*!
  \file NApproximateAgglomerativeClusteringBvhItem.qml
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
  id: approximate_agglomerative_clustering_bvh_item
  anchors.fill: parent
//  width: 480
//  height: 274

  readonly property int defaultDelta: 4
  readonly property real defaultEpsilon: 0.2

  Component {
    id: spinbox_button_style
    NSpinBoxButtonStyle {
    }
  }

  Component {
    id: spinbox_style
    NSpinBoxStyle {
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
    id: delta_spinbox
    x: 15
    y: 27
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_button_style

    horizontalAlignment: Qt.AlignLeft
    decimals: 0
    prefix: qsTr("Delta") + ": "
    maximumValue: 99
    minimumValue: 1
  }

  SpinBox {
    id: epsilon_spinbox
    x: 15
    y: 58
    width: 140
    height: 20

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    decimals: 7
    prefix: qsTr("Epsilon") + ": "
    maximumValue: 0.5
    minimumValue: 0.0
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
    delta_spinbox.value = defaultDelta
    epsilon_spinbox.value = defaultEpsilon
  }

  function loadSceneSettings(p) {
    var prefix = p + Keyword.approximateAgglomerativeClusteringBvh + "/"
    delta_spinbox.value = scene_settings.intValue(prefix + Keyword.delta, 
                                                  defaultDelta)
    epsilon_spinbox.value = scene_settings.realValue(prefix + Keyword.epsilon, 
                                                     defaultEpsilon)
  }

  function saveSceneSettings(p) {
    var prefix = p + Keyword.approximateAgglomerativeClusteringBvh + "/"
    scene_settings.setIntValue(prefix + Keyword.delta, delta_spinbox.value)
    scene_settings.setRealValue(prefix + Keyword.epsilon, epsilon_spinbox.value)
  }
}
