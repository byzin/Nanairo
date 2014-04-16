/*!
  \file NSmoothDielectricSurfaceItem.qml
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
  id: smooth_dielectric_surface_item
  anchors.fill: parent
//  width: 476
//  height: 240

  property var currentSurfaceItem: null

  Label {
    id: outer_label
    x: 8
    y: 8
    text: qsTr("Outer")
    textFormat: Text.PlainText
    font.bold: true
    verticalAlignment: Text.AlignVCenter
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

  onCurrentSurfaceItemChanged: {
    if (currentSurfaceItem != null) {
      outer_refractive_index_spectra_view.currentItem = 
          currentSurfaceItem.outerRefractiveIndex
      inner_refractive_index_spectra_view.currentItem = 
          currentSurfaceItem.innerRefractiveIndex
    }
  }

  function setDefaultSurface(surfaceItem) {
    surfaceItem.outerRefractiveIndex = 
        [false, Qt.rgba(0, 0, 0, 0), Qt.resolvedUrl("")]
    surfaceItem.innerRefractiveIndex = 
        [false, Qt.rgba(0, 0, 0, 0), Qt.resolvedUrl("")]
  }

  function loadSceneSettings(item, p) {
    setDefaultSurface(item)
    var prefix = p + Keyword.smoothDielectricSurface + "/"
    outer_refractive_index_spectra_view.loadSceneSettings(
        item.outerRefractiveIndex, 
        prefix + Keyword.outerRefractiveIndex + "/")
    inner_refractive_index_spectra_view.loadSceneSettings(
        item.innerRefractiveIndex,
        prefix + Keyword.innerRefractiveIndex + "/")
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.smoothDielectricSurface + "/"
    outer_refractive_index_spectra_view.saveSceneSettings(
        item.outerRefractiveIndex, 
        prefix + Keyword.outerRefractiveIndex + "/")
    inner_refractive_index_spectra_view.saveSceneSettings(
        item.innerRefractiveIndex,
        prefix + Keyword.innerRefractiveIndex + "/")
  }
}
