/*!
  \file NProbabilisticPpmItem.qml
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
  id: probabilistic_ppm_item
  anchors.fill: parent
//  width: 480
//  height: 274

  readonly property int defaultNumOfPhotons: 131072
  readonly property real defaultPhotonSearchRadius: 0.02
  readonly property real defaultRadiusReductionRate: 0.6666667
  readonly property int defaultKNearestNeighbor: 10

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: probabilistic_ppm_item.color
      fontPixelSize: 10
    }
  }

  Component {
    id: checkbox_style
    NCheckBoxStyle {}
  }

  Label {
    id: num_of_photons_label
    x: 16
    y: 8
    text: qsTr("Number of photons")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: num_of_photons_spinbox
    x: 16
    y: 27
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: Math.pow(2, 31)
    minimumValue: 1
    decimals: 0
  }

  Label {
    id: photn_search_radius_label
    x: 16
    y: 61
    text: qsTr("Photon search radius")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: photon_search_radius_spinbox
    x: 16
    y: 80
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: 99.9999999
    minimumValue:  0.0000001
    decimals: 7
  }

  Label {
    id: radius_reduction_rate_label
    x: 16
    y: 115
    text: qsTr("Radius reduction rate")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: radius_reduction_rate_spinbox
    x: 16
    y: 134
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: 0.9999999
    minimumValue: 0.0000001
    decimals: 7
  }

  Label {
    id: k_nearest_neighbor_label
    x: 16
    y: 171
    text: qsTr("K nearest neighbor")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: k_nearest_neighbor_spinbox
    x: 16
    y: 190
    width: 100
    height: 20

    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignRight
    maximumValue: 99
    minimumValue: 1
    decimals: 0
  }

  CheckBox {
    id: glossy_photon_map_checkbox
    x: 16
    y: 227
    height: 20
    text: qsTr("Glossy photon map")
    style: checkbox_style
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

  Label {
    id: kernel_label
    x: 168
    y: 8
    text: qsTr("Kernel")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  ComboBox {
    id: kernel_combobox
    x: 168
    y: 27
    width: 120
    height: 20
    style: combobox_style

    model : [Keyword.noKernel,
             Keyword.coneKernel,
             Keyword.perlinKernel]
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
    num_of_photons_spinbox.value = defaultNumOfPhotons
    photon_search_radius_spinbox.value = defaultPhotonSearchRadius
    radius_reduction_rate_spinbox.value = defaultRadiusReductionRate
    k_nearest_neighbor_spinbox.value = defaultKNearestNeighbor
    glossy_photon_map_checkbox.checked = false
    kernel_combobox.currentIndex = 0
  }

  function loadSceneSettings(p) {
    var prefix = p + Keyword.probabilisticPpm + "/"
    num_of_photons_spinbox.value =
        scene_settings.intValue(prefix + Keyword.numOfPhotons, 
                                defaultNumOfPhotons)
    photon_search_radius_spinbox.value =
        scene_settings.realValue(prefix + Keyword.photonSearchRadius, 
                                 defaultPhotonSearchRadius)
    radius_reduction_rate_spinbox.value =
        scene_settings.realValue(prefix + Keyword.radiusReductionRate, 
                                 defaultRadiusReductionRate)
    k_nearest_neighbor_spinbox.value =
        scene_settings.intValue(prefix + Keyword.kNearestNeighbor, 
                                defaultKNearestNeighbor)
    glossy_photon_map_checkbox.checked =
        scene_settings.booleanValue(prefix + Keyword.glossyPhotonMap, false)
    kernel_combobox.currentIndex = kernel_combobox.find(
        scene_settings.stringValue(prefix + Keyword.kernelType, 
                                   Keyword.perlinKernel))
  }

  function saveSceneSettings(p) {
    var prefix = p + Keyword.probabilisticPpm + "/"
    scene_settings.setIntValue(prefix + Keyword.numOfPhotons,
                               num_of_photons_spinbox.value)
    scene_settings.setRealValue(prefix + Keyword.photonSearchRadius,
                                photon_search_radius_spinbox.value)
    scene_settings.setRealValue(prefix + Keyword.radiusReductionRate,
                                radius_reduction_rate_spinbox.value)
    scene_settings.setIntValue(prefix + Keyword.kNearestNeighbor,
                               k_nearest_neighbor_spinbox.value)
    scene_settings.setBooleanValue(prefix + Keyword.glossyPhotonMap,
                                   glossy_photon_map_checkbox.checked)
    scene_settings.setStringValue(prefix + Keyword.kernelType,
                                  kernel_combobox.currentText)
  }
}
