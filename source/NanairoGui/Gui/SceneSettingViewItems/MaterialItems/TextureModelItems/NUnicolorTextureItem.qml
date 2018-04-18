/*!
  \file NUnicolorTextureItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: textureItem

  readonly property alias palettePrefixKey: colorPalette.prefixKey
  readonly property alias colorMode: colorPalette.colorMode
  readonly property alias rgbPaletteColor: colorPalette.rgbPaletteColor
  readonly property alias spectraFilePath: colorPalette.spectraFilePath

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "color"
    }

    NColorPalette {
      id: colorPalette

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      prefixKey: Definitions.color
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    colorPalette.initItem(item);
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    colorPalette.setValue(item);
  }

  function getSceneData(item) {
    var sceneData = {};

    var paletteData = Definitions.getProperty(item, colorPalette.prefixKey);
    sceneData[colorPalette.prefixKey] = colorPalette.getSceneData(paletteData);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var paletteData = Definitions.getProperty(sceneData, colorPalette.prefixKey);
    colorPalette.setSceneData(paletteData, item);
  }
}
