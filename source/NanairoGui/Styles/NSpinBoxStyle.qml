/*!
 \file NSpinBoxStyle.qml
 \author Sho Ikeda

 Copyright (c) 2015-2016 Sho Ikeda
 This software is released under the MIT License.
 http://opensource.org/licenses/mit-license.php
 */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4

NIntSpinBoxStyle {
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
