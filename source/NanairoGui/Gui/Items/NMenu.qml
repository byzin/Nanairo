/*!
  \file NMenu.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import "../definitions.js" as Definitions

Menu {
  id: menu

  // Material
  Material.elevation: 1

  font.pixelSize: Definitions.defaultTextFontSize
}
