/*!
  \file NSpectraFileView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: spectra_file_view

  property string keyPrefix: ""
  property string filePathKey: Nanairo.value
  property url spectraFilePath: Qt.resolvedUrl("")
  property string spectraFileName: ""
  property string buttonTitle: qsTr("Open spectra file")
  property var currentItem: null
  property bool isItemChangeMode: false

  NFileDialog {
    id: open_spectra_dialog
    title: buttonTitle
    nameFilters: [qsTr("Spectra file") + " (*.csv)"]
    onAccepted: spectraFilePath = fileUrl
  }

  NButton {
    id: open_spectra_button
    anchors.fill: parent
    backgroundColor: spectra_file_view.color
    text: (spectraFileName == "") ? buttonTitle
                                  : spectraFileName
    onClicked: open_spectra_dialog.openDialog()
  }

  onSpectraFilePathChanged: {
    var filePath = spectraFilePath.toString();
    if (filePath != "") {
      setCurrentItemValue(keyPrefix + filePathKey, filePath);
    }
    spectraFileName = nanairoManager.getFileName(spectraFilePath);
  }

  function initializeItem(item) {
    // Spectra file
    item[keyPrefix + filePathKey] = "";
  }

  function reflectItem() {
    var item = currentItem;
    // Spectra file
    spectraFilePath = Qt.resolvedUrl(item[keyPrefix + filePathKey]);
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function setCurrentItemValue(key, value) {
    if (currentItem != null) {
      currentItem[key] = value;
      currentItem[key + "String"] = value.toString();
    }
  }

  function getItemData(item) {
    var itemData = {};
    var filePath = Qt.resolvedUrl(item[keyPrefix + filePathKey]);
    itemData[filePathKey] = nanairoManager.toRelativeFilePath(filePath);
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    var filePath = nanairoManager.toAbsoluteFileUrl(itemData[filePathKey]);
    item[keyPrefix + filePathKey] = filePath.toString();
    return item;
  }
}
