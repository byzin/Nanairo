/*!
  \file NSmoothDiffuseSurfaceItem.qml
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
  id: smooth_diffuse_surface_item
  anchors.fill: parent
//  width: 476
//  height: 240

  readonly property int defaultReflectanceIndex: 0
  property ListModel textureModel
  property var currentSurfaceItem: null

  Component {
    id: combobox_style 
    NComboBoxStyle {
      backgroundColor: smooth_diffuse_surface_item.color
      fontPixelSize: 10
    }
  }

  Label {
    id: value_label
    x: 8
    y: 8
    text: qsTr("Reflectance")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  ComboBox {
    id: reflectance_combobox
    x: 16
    y: 27
    width: 125
    height: 20
    style: combobox_style
    model: textureModel

    onCurrentIndexChanged: {
      if (currentSurfaceItem != null) {
        currentSurfaceItem.reflectanceIndex = currentIndex
      }
    }
  }

  onCurrentSurfaceItemChanged: {
    if (currentSurfaceItem != null) {
      reflectance_combobox.currentIndex = currentSurfaceItem.reflectanceIndex
    }
  }

  function setDefaultSurface(surfaceItem) {
    surfaceItem.reflectanceIndex = defaultReflectanceIndex
  }

  function loadSceneSettings(item, p) {
    var prefix = p + Keyword.smoothDiffuseSurface + "/"
    item.reflectanceIndex = 
        scene_settings.intValue(prefix + Keyword.reflectanceIndex, 
                                defaultReflectanceIndex)
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.smoothDiffuseSurface + "/"
    scene_settings.setIntValue(prefix + Keyword.reflectanceIndex, 
                               item.reflectanceIndex)
  }
}
