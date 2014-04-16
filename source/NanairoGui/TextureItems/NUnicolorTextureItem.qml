/*!
  \file NUnicolorTextureItem.qml
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
  anchors.fill: parent
//  width: 476
//  height: 240

  property var currentTextureItem: null

  Label {
    id: color_label
    x: 8
    y: 8
    text: qsTr("Color")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  NColorView {
    id: color_palette
    x: 16
    y: 27
    color: parent.color
  }

  onCurrentTextureItemChanged: {
    if (currentTextureItem != null) {
      color_palette.currentItem = null
      color_palette.currentItem = currentTextureItem.textureColor
    }
  }

  function setDefaultTexture(textureItem) {
    textureItem.textureColor = [true, Qt.rgba(0.8, 0.8, 0.8, 1), Qt.resolvedUrl("")]
  }

  function loadSceneSettings(item, p) {
    setDefaultTexture(item)
    var prefix = p + Keyword.unicolorTexture + "/"
    color_palette.loadSceneSettings(item.textureColor, 
                                    prefix + Keyword.textureColor + "/")
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.unicolorTexture + "/"
    color_palette.saveSceneSettings(item.textureColor, 
                                    prefix + Keyword.textureColor + "/")
  }
}
