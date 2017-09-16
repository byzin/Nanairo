/*!
  \file NImageSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NSettingBlock {
  id: image_setting_block

  property url imageFilePath: Qt.resolvedUrl("")
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText:qsTr("Image")

  height: Nanairo.defaultBlockHeight * 2 + Nanairo.blockOffset

  Rectangle {
    id: image_frame

    readonly property int borderSize: 2

    anchors.horizontalCenter: image_setting_block.horizontalCenter
    y: image_setting_block.labelHeight + Nanairo.itemOffset
    width: image_setting_block.width - 2 * Nanairo.itemOffset
    height: image_setting_block.height - (y + Nanairo.itemOffset)
    color: image_open_area.containsMouse 
        ? Qt.darker(image_setting_block.color, Nanairo.defaultDarkerScale)
        : image_setting_block.color

    border.color: Qt.darker(color, Nanairo.defaultDarkerScale)
    border.width: borderSize

    Image {
      id: image_board

      anchors.horizontalCenter: image_frame.horizontalCenter
      anchors.verticalCenter: image_frame.verticalCenter
      width: image_frame.width - 2 * image_frame.borderSize
      height: image_frame.height - 2 * image_frame.borderSize

      fillMode: Image.PreserveAspectFit
      horizontalAlignment: Image.AlignHCenter
      verticalAlignment: Image.AlignVCenter

      source: imageFilePath
      sourceSize.width: width
      sourceSize.height: height

      NFileDialog {
        id: open_image_dialog
        title: qsTr("Open image file")
        nameFilters: ["Image files (*.jpg *.png *.bmp)"]
        onAccepted: imageFilePath = fileUrl
      }

      MouseArea {
        id: image_open_area
        anchors.fill: parent
        hoverEnabled: true
        onClicked: open_image_dialog.openDialog()
      }
    }
  }

  onImageFilePathChanged: {
    var filePath = imageFilePath.toString();
    if (filePath != "") {
      setCurrentItemValue(Nanairo.imageFilePath, filePath);
    }
  }

  function initializeItem(item) {
    item[Nanairo.imageFilePath] = Qt.resolvedUrl("");
  }

  function reflectItem() {
    var item = currentItem;
    // Image file path
    imageFilePath = Qt.resolvedUrl(item[Nanairo.imageFilePath]);
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function setCurrentItemValue(key, value) {
    if (currentItem != null) {
      currentItem[key] = value;
    }
  }

  function getItemData(item) {
    var itemData = {};
    var filePath = Qt.resolvedUrl(item[Nanairo.imageFilePath]);
    itemData[Nanairo.imageFilePath] = nanairoManager.toRelativeFilePath(filePath);
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    var filePath = nanairoManager.toAbsoluteFileUrl(itemData[Nanairo.imageFilePath]);
    item[Nanairo.imageFilePath] = filePath.toString();
    return item;
  }
}
