/*!
  \file NRoughConductorSurfaceItem.qml
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
  id: rough_conductor_surface_item
  anchors.fill: parent
//  width: 476
//  height: 240

  readonly property int defaultRoughnessIndex: 0
  property ListModel textureModel
  property var currentSurfaceItem: null

  Label {
    id: outer_label
    x: 8
    y: 8
    text: qsTr("Outer")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignLeft
  }

  Label {
    id: outer_refractive_index_label
    x: 16
    y: 27
    text: qsTr("Refractive index")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  Rectangle {
    id: outer_refractive_index_view
    x: 16
    y: 46
    width: 114
    height: 20
    color: parent.color

    NSpectraFileView {
      id: outer_refractive_index_spectra_view
      color: parent.color
    }
  }

  Label {
    id: inner_label
    x: 8
    y: 81
    text: qsTr("Inner")
    textFormat: Text.PlainText
    horizontalAlignment: Text.AlignLeft
    font.bold: true
    verticalAlignment: Text.AlignVCenter
  }

  Label {
    id: inner_refractive_index_label
    x: 16
    y: 100
    text: qsTr("Refractive index")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  Rectangle {
    id: inner_refractive_index_view
    x: 16
    y: 119
    width: 114
    height: 20
    color: parent.color

    NSpectraFileView {
      id: inner_refractive_index_spectra_view
      color: parent.color
    }
  }

  Label {
    id: inner_extinction_label
    x: 16
    y: 151
    text: qsTr("Extinction")
    verticalAlignment: Text.AlignVCenter
    textFormat: Text.PlainText
  }

  Rectangle {
    id: inner_extinction_view
    x: 16
    y: 170
    width: 114
    height: 20
    color: parent.color

    NSpectraFileView {
      id: inner_extinction_spectra_view
      color: parent.color
    }
  }

  Label {
    id: roughness_label
    x: 148
    y: 8
    text: qsTr("Roughness")
    verticalAlignment: Text.AlignVCenter
    font.bold: true
    textFormat: Text.PlainText
  }

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: rough_conductor_surface_item.color
      fontPixelSize: 10
    }
  }

  ComboBox {
    id: roughness_combobox
    x: 156
    y: 32
    width: 125
    height: 20
    style: combobox_style
    model: textureModel

    onCurrentIndexChanged: {
      if (currentSurfaceItem != null) {
        currentSurfaceItem.roughnessIndex = currentIndex
      }
    }
  }

  onCurrentSurfaceItemChanged: {
    if (currentSurfaceItem != null) {
      outer_refractive_index_spectra_view.currentItem =
          currentSurfaceItem.outerRefractiveIndex
      inner_refractive_index_spectra_view.currentItem =
          currentSurfaceItem.innerRefractiveIndex
      inner_extinction_spectra_view.currentItem =
          currentSurfaceItem.innerExtinction
      roughness_combobox.currentIndex = currentSurfaceItem.roughnessIndex
    }
  }

  function setDefaultSurface(surfaceItem) {
    surfaceItem.outerRefractiveIndex = 
        [false, Qt.rgba(0, 0, 0, 0), Qt.resolvedUrl("")]
    surfaceItem.innerRefractiveIndex = 
        [false, Qt.rgba(0, 0, 0, 0), Qt.resolvedUrl("")]
    surfaceItem.innerExtinction = 
        [false, Qt.rgba(0, 0, 0, 0), Qt.resolvedUrl("")]
    surfaceItem.roughnessIndex = defaultRoughnessIndex
  }

  function loadSceneSettings(item, p) {
    setDefaultSurface(item)
    var prefix = p + Keyword.roughConductorSurface + "/"
    outer_refractive_index_spectra_view.loadSceneSettings(
        item.outerRefractiveIndex, 
        prefix + Keyword.outerRefractiveIndex + "/")
    inner_refractive_index_spectra_view.loadSceneSettings(
        item.innerRefractiveIndex,
        prefix + Keyword.innerRefractiveIndex + "/")
    inner_extinction_spectra_view.loadSceneSettings(
        item.innerExtinction, 
        prefix + Keyword.innerExtinction + "/")
    item.roughnessIndex = scene_settings.intValue(prefix + Keyword.roughnessIndex, 
                                                  defaultRoughnessIndex)
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.roughConductorSurface + "/"
    outer_refractive_index_spectra_view.saveSceneSettings(
        item.outerRefractiveIndex, 
        prefix + Keyword.outerRefractiveIndex + "/")
    inner_refractive_index_spectra_view.saveSceneSettings(
        item.innerRefractiveIndex,
        prefix + Keyword.innerRefractiveIndex + "/")
    inner_extinction_spectra_view.saveSceneSettings(
        item.innerExtinction, 
        prefix + Keyword.innerExtinction + "/")
    scene_settings.setIntValue(prefix + Keyword.roughnessIndex, 
                               item.roughnessIndex)
  }
}
