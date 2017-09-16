/*!
  \file NShapeTypeSettingBlock.qml
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
  id: shape_type_setting_block

  property url objectFilePath: Qt.resolvedUrl("")
  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("ShapeType")

  NComboBox {
    id: shape_type_combobox

    y: shape_type_setting_block.labelHeight + Nanairo.itemOffset
    backgroundColor: shape_type_setting_block.color

    model: [Nanairo.planeObject,
            Nanairo.meshObject]

    onCurrentIndexChanged: setCurrentItemValue(Nanairo.shapeType, currentText)
  }

  NButton {
    id: open_object_button

    property string objectFileName: ""

    y: (shape_type_combobox.y + shape_type_combobox.height) + Nanairo.itemOffset
    width: 120
    enabled: shape_type_combobox.currentText == Nanairo.meshObject
    backgroundColor: shape_type_setting_block.color

    text: (objectFileName == "") ? qsTr("Open object file") : objectFileName

    NFileDialog {
      id: open_object_dialog
      title: qsTr("Open object file")
      nameFilters: ["Wavefront file (*.obj)"]
      onAccepted: shape_type_setting_block.objectFilePath = fileUrl
    }

    onEnabledChanged: {
      if (!enabled) {
        objectFilePath = Qt.resolvedUrl("");
      }
    }

    onClicked: {
      open_object_dialog.openDialog()
    }
  }

  onObjectFilePathChanged: {
    var filePath = objectFilePath.toString();
    if (filePath != "") {
      setCurrentItemValue(Nanairo.objectFilePath, filePath);
    }
    open_object_button.objectFileName = nanairoManager.getFileName(objectFilePath);
  }

  function initializeItem(item) {
    item[Nanairo.shapeType] = shape_type_combobox.textAt(0);
    item[Nanairo.objectFilePath] = "";
  }

  function reflectItem() {
    var item = currentItem;
    // Shape type
    var type = item[Nanairo.shapeType];
    var typeIndex = shape_type_combobox.find(type);
    console.assert(typeIndex != -1, "Shape type isn't found: ", type);
    shape_type_combobox.currentIndex = typeIndex;
    // Object button
    if (type == Nanairo.meshObject) {
      objectFilePath = Qt.resolvedUrl(item[Nanairo.objectFilePath]);
    }
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
    var type = item[Nanairo.shapeType];
    itemData[Nanairo.shapeType] = type;
    if (type == Nanairo.meshObject) {
      var filePath = Qt.resolvedUrl(item[Nanairo.objectFilePath]);
      itemData[Nanairo.objectFilePath] = nanairoManager.toRelativeFilePath(filePath);
    }
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    var type = itemData[Nanairo.shapeType];
    item[Nanairo.shapeType] = type;
    if (type == Nanairo.meshObject) {
      var filePath = nanairoManager.toAbsoluteFileUrl(itemData[Nanairo.objectFilePath]);
      item[Nanairo.objectFilePath] = filePath.toString();
    }
    return item;
  }
}
