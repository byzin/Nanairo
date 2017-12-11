/*!
  \file NImageButton.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import "../definitions.js" as Definitions

NButton {
  id: imageButton

  property string imageSource: ""

  width: Definitions.defaultImageButtonSize
  height: Definitions.defaultImageButtonSize
  padding: 0

  contentItem: Image {
    fillMode: Image.Pad
    horizontalAlignment: Image.AlignHCenter
    verticalAlignment: Image.AlignVCenter
    source: imageButton.imageSource
  }
}
