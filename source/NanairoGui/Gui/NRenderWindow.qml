/*!
  \file NRenderWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import "Items"
import "definitions.js" as Definitions

Window {
  id: renderWindow

  property bool isPreviewMode: false

  ColumnLayout {
    anchors.fill: parent
    enabled: renderWindow.visible
    spacing: 0

    NLabel {
      id: infoLabel
      Layout.fillWidth: true
      font.family: nanairoManager.getDefaultFixedFontFamily()
      font.pixelSize: 11
      text: "000.00 fps,  0000000000 cycles,  0000 h 00 m 00.000 s"
    }

    NProgressBar {
      id: progress_bar
      Layout.fillWidth: true
      value: 0.0
    }

    NPane {
      id: renderFrame
      Layout.fillWidth: true
      Layout.fillHeight: true

      Image {
        id: renderImage

        property int imageNumber

        anchors.fill: parent
        cache: false
        fillMode: Image.PreserveAspectFit
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        smooth: false

        NPreviewEventArea {
          id: eventArea
          enabled: renderWindow.isPreviewMode
          anchors.fill: parent
        }

        function updateRenderImage() {
          imageNumber = imageNumber + 1;
          source = "image://renderedImage/preview?" + imageNumber;
        }
      }
    }
  }

  Connections {
    target: nanairoManager
    onNotifyOfRenderingProgress: {
      progress_bar.indeterminate = false;
      progress_bar.value = progress;
      infoLabel.text = status;
      renderImage.updateRenderImage();
    }
  }

  onClosing: nanairoManager.stopRendering()

  function initForRendering(imageResolution, isPreviewing) {
    width = imageResolution[0];
    height = imageResolution[1] + infoLabel.height + progress_bar.height;
    progress_bar.indeterminate = true;
    renderWindow.isPreviewMode = isPreviewing;
    renderImage.imageNumber = 0;
  }
}
