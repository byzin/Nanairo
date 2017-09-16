/*!
  \file NTabViewStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "../nanairo.js" as Nanairo

TabViewStyle {
  property color backgroundColor

  tab: Rectangle {
    implicitWidth: Math.max(tab_name_label.width + 10, 60)
    implicitHeight: 20
    enabled: styleData.enabled

    color: styleData.selected ? Qt.darker(backgroundColor, Nanairo.defaultDarkerScale)
                              : backgroundColor
    border.color: Qt.darker(color, Nanairo.defaultDarkerScale)

    Label {
      id: tab_name_label
      anchors.centerIn: parent
      text: styleData.title
      verticalAlignment: Text.AlignVCenter
      horizontalAlignment: Text.AlignHCenter
      textFormat: Text.PlainText
    }

    radius: 5.0
  }
  tabOverlap: -8
  tabsAlignment: Qt.AlignHCenter
}
