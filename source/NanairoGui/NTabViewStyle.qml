/*!
  \file NTabViewStyle.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

TabViewStyle {
  property color backgroundColor
  tab: Rectangle {
    implicitWidth: Math.max(tab_name_label.width + 10, 60)
    implicitHeight: 20
    enabled: styleData.enabled

    color: styleData.selected ? Qt.darker(backgroundColor, 1.2)
                              : backgroundColor
    border.color: Qt.darker(color, 1.2)

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
