/*!
  \file NColorView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "keyword.js" as Keyword

Rectangle {
  id: color_view
  width: 160
  height: 20

  readonly property bool defaultMode: true
  property bool isRgbMode: true
  property bool itemChangeMode: false
  property var currentItem: null

  Component {
    id: button_style
    NButtonStyle {
      backgroundColor: color_view.color
    }
  }

  Button {
    id: mode_change_button
    x: 0
    y: 0
    width: 40
    height: 20
    text: isRgbMode ? qsTr("RGB") : qsTr("SPC")
    style: button_style
    onClicked: {
      var mode = !isRgbMode
      if (currentItem != null) {
        setItemMode(currentItem, mode)
        if (!itemChangeMode)
          setDefaultColor(currentItem, mode)
        else
          itemChangeMode = false
        resetPaletteItem(currentItem, mode)
      }
      isRgbMode = mode
    }
  }

  TabView {
    id: color_tab_view
    x: 46
    y: 0
    width: 114
    height: 20
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: qsTr("RGB")
      active: true
      NRgbColorPalette {
        color: color_view.color
      }
    }

    Tab {
      title: qsTr("Spectra")
      active: true
      NSpectraFileView {
        color: color_view.color
      }
    }

    currentIndex: isRgbMode ? 0 : 1
  }

  onCurrentItemChanged: {
    if (currentItem != null) {
      var mode = getItemMode(currentItem)
      if (isRgbMode == mode) {
        resetPaletteItem(currentItem, mode)
      }
      else {
        itemChangeMode = true 
        isRgbMode = mode
      }
    }
  }

  function getPalette(mode) {
    var index = mode ? 0 : 1
    return color_tab_view.getTab(index).item
  }

  function getItemMode(item) {
    return item[0]
  }

  function setItemMode(item, mode) {
    item[0] = mode
  }

  function resetPaletteItem(item, mode) {
    var rgbPalette = getPalette(true)
    var spectraPalette = getPalette(false)
    rgbPalette.currentItem = null
    spectraPalette.currentItem = null
    if (mode)
      rgbPalette.currentItem = item
    else
      spectraPalette.currentItem = item
  }

  function setDefaultColor(item, mode) {
    var palette = getPalette(mode)
    palette.setDefaultColor(item)
  }

  function loadSceneSettings(item, prefix) {
    var mode = scene_settings.booleanValue(prefix + Keyword.isRgbMode, defaultMode)
    setItemMode(item, mode)
    var palette = getPalette(mode)
    palette.loadSceneSettings(item, prefix)
  }

  function saveSceneSettings(item, prefix) {
    var mode = getItemMode(item)
    scene_settings.setBooleanValue(prefix + Keyword.isRgbMode, mode)
    var palette = getPalette(mode)
    palette.saveSceneSettings(item, prefix)
  }
}
