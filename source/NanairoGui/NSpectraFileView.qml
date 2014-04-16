/*!
  \file NSpectraFileView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import "keyword.js" as Keyword

Rectangle {
  id: spectra_file_view
  anchors.fill: parent

  property url spectraFilePath: Qt.resolvedUrl("")
  property string spectraFileName: ""
  property var currentItem: null

  FileDialog {
    id: open_spectra_dialog
    folder: Qt.resolvedUrl(".")
    title: qsTr("Open spectra file")
    nameFilters: [qsTr("Spectra file") + " (*.csv)"]
    onAccepted: spectraFilePath = fileUrl
  }

  Component {
    id: button_style
    NButtonStyle {
      backgroundColor: spectra_file_view.color
    }
  }

  Button {
    id: open_spectra_button
    anchors.fill: parent
    text: (spectraFileName == "") ? qsTr("Open spectra file") : spectraFileName
    style: button_style
    onClicked: open_spectra_dialog.open()
  }

  onSpectraFilePathChanged: {
    spectraFileName = manager.getFileName(spectraFilePath)
    if (currentItem != null)
      setSpectraFilePath(currentItem, spectraFilePath)
  }

  onCurrentItemChanged: {
    if (currentItem != null)
      spectraFilePath = getSpectraFilePath(currentItem)
  }

  function getSpectraFilePath(item) {
    return item[2]
  }

  function setSpectraFilePath(item, filePath) {
    item[2] = filePath
  }

  function setDefaultColor(item) {
    item[2] = Qt.resolvedUrl("")
  }

  function loadSceneSettings(item, prefix) {
    var filePath = scene_settings.urlValue(prefix + Keyword.spectraFilePath, 
                                           Qt.resolvedUrl(""))
    setSpectraFilePath(item, filePath)
    currentItem = null
    currentItem = item
  }

  function saveSceneSettings(item, prefix) {
    var filePath = getSpectraFilePath(item)
    scene_settings.setUrlValue(prefix + Keyword.spectraFilePath, filePath)
  }
}
