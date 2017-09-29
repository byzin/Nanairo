/*!
  \file NRenderedImageWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Window 2.2
import "RenderedImageWindowItems"

Window {
  id: rendered_image_window

  readonly property int infoViewAreaHeight: 16
  readonly property int textFontSize: 10
  readonly property int windowWidth: imageWidth
  readonly property int windowHeight: imageHeight + infoViewAreaHeight
  property int imageWidth
  property int imageHeight
  property bool isPreviewMode: false

  width: windowWidth
  height: windowHeight

  maximumWidth: windowWidth
  maximumHeight: windowHeight
  minimumWidth: windowWidth
  minimumHeight: windowHeight

  Rectangle {
    id: view_area
    anchors.fill: parent 
    color: rendered_image_window.color

    Rectangle {
      id: rendering_info_view_area
      width: rendered_image_window.windowWidth
      height: rendered_image_window.infoViewAreaHeight
      color: view_area.color

      NText {
        id: rendering_info_text
        x: 4
        y: 0
        height: rendered_image_window.infoViewAreaHeight
        text: qsTr("")
      }
    }

    Rectangle {
      id: rendered_image_view_area
      y: (rendering_info_view_area.y + rendering_info_view_area.height)
      width: rendered_image_window.imageWidth
      height: rendered_image_window.imageHeight
      color: view_area.color

      Image {
        id: rendered_image_view

        property int imageNumber: 0

        anchors.fill: parent
        cache: false
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        smooth: false
        sourceSize.width: rendered_image_window.imageWidth
        sourceSize.height: rendered_image_window.imageHeight

        function updateRenderedImage(info) {
          source = "image://renderedImage/preview?" + imageNumber;
          imageNumber = imageNumber + 1;
          rendering_info_text.text = info;
        }

        NPreviewEventArea {
          id: preview_event_area
          anchors.fill: parent
          enabled: rendered_image_window.isPreviewMode
        }
      }
    }
  }

  onClosing: nanairoManager.stopRendering()

  Connections {
    target: nanairoManager
    onNotifyOfRenderingInfo: rendered_image_view.updateRenderedImage(info)
    onFinished: close()
  }

  function initForRendering(isPreviewing) {
    rendered_image_window.isPreviewMode = isPreviewing;
    rendered_image_view.imageNumber = 0;
  }
}
