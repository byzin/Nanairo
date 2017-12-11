/*!
  \file NImageTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.platform 1.0
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: textureItem

  property string imageFilePath: ""

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "image"
    }

    NImageButton {
      id: imageButton

      Layout.fillWidth: true
      Layout.preferredHeight: column1.width
      imageSource: (textureItem.imageFilePath != "")
          ? nanairoManager.getAbsolutePath(textureItem.imageFilePath)
          : ""

      onClicked: imageFileDialog.open()

      Component.onCompleted: contentItem.fillMode = Image.PreserveAspectFit
    }
  }

  FileDialog {
    id: imageFileDialog

    title: "Open image file"
    nameFilters: ["Image files (*.bmp *.jpg *.jpeg *.png)",
                  "Windows Bitmap (*.bmp)",
                  "Joint Photographic Experts Group (*.jpg *.jpeg)",
                  "Portable Network Graphics (*.png)"]

    onAccepted: imageFilePath = nanairoManager.getRelativePath(file)
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.imageFilePath] = "";
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    imageFilePath = Definitions.getProperty(item, Definitions.imageFilePath);
  }

  function getSceneData(item) {
    var sceneData = {};

    sceneData[Definitions.imageFilePath] =
        Definitions.getProperty(item, Definitions.imageFilePath);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    item[Definitions.imageFilePath] =
        Definitions.getProperty(sceneData, Definitions.imageFilePath);
  }
}
