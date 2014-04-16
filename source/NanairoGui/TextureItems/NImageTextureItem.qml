/*!
  \file NImageTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2
import ".."
import "../keyword.js" as Keyword

Rectangle {
  id: image_texture_item
  anchors.fill: parent
//  width: 476
//  height: 240

  property url imageFilePath
  property var currentTextureItem: null

  readonly property int borderWidth: 1

  Label {
    id: image_label
    x: 8
    y: 8
    text: qsTr("Image")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  Rectangle {
    id: image_frame
    x: 16
    y: 27
    width: 194
    height: 110
    color: image_open_area.containsMouse ? Qt.darker(parent.color, 1.1) : parent.color
    border.color: Qt.darker(color, 1.2)
    border.width: borderWidth

    Image {
      id: texture_image
      x: borderWidth
      y: borderWidth
      width: parent.width - 2 * borderWidth
      height: parent.height - 2 * borderWidth
      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter

      source: imageFilePath
      sourceSize.width: width
      sourceSize.height: height

      FileDialog {
        id: open_image_dialog
        folder: Qt.resolvedUrl(".")
        title: "Open image file"
        nameFilters: ["Image files (*.jpg *.png *.bmp)"]
        onAccepted: {
          imageFilePath = fileUrl
          if (currentTextureItem != null)
            currentTextureItem.imageFilePath = fileUrl
        }
      }

      MouseArea {
        id: image_open_area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: open_image_dialog.open()
      }
    }
  }

  onCurrentTextureItemChanged: {
    if (currentTextureItem != null)
      imageFilePath = currentTextureItem.imageFilePath
  }

  function setDefaultTexture(textureItem) {
    textureItem.imageFilePath = Qt.resolvedUrl("")
  }

  function loadSceneSettings(item, p) {
    var prefix = p + Keyword.imageTexture + "/"
    item.imageFilePath = scene_settings.urlValue(prefix + Keyword.imageFilePath, 
                                                 Qt.resolvedUrl(""))
  }

  function saveSceneSettings(item, p) {
    var prefix = p + Keyword.imageTexture + "/"
    scene_settings.setUrlValue(prefix + Keyword.imageFilePath, item.imageFilePath)
  }
}
