/*!
  \file NLogView.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3

Rectangle {
  id: log_view
//  width: 640
//  height: 480

  TextArea {
    id: log_text_editor
    anchors.fill: parent

//    backgroundVisible: false
    frameVisible: false
    verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
    readOnly: true
    textFormat: TextEdit.PlainText
  }

  function appendLog(log) {
    log_text_editor.append(log)
  }
}
