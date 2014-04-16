/*!
 \file NSpinBoxStyle.qml
 \author Sho Ikeda

 Copyright (c) 2015 Sho Ikeda
 This software is released under the MIT License.
 http://opensource.org/licenses/mit-license.php
 */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

NSpinBoxButtonStyle {
  decrementControl: Rectangle {
    width: 0
    height: 0
    enabled: false
    visible: false
  }
  incrementControl: Rectangle {
    width: 0
    height: 0
    enabled: false
    visible: false
  }
}
