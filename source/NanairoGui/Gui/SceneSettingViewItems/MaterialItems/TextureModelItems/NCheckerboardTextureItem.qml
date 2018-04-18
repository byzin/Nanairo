/*!
  \file NCheckerboardTextureItem.qml
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

  readonly property alias color1PalettePrefixKey: color1Palette.prefixKey
  readonly property alias color1ColorMode: color1Palette.colorMode
  readonly property alias color1RgbPaletteColor: color1Palette.rgbPaletteColor
  readonly property alias color1SpectraFilePath: color1Palette.spectraFilePath
  readonly property alias color2PalettePrefixKey: color2Palette.prefixKey
  readonly property alias color2ColorMode: color2Palette.colorMode
  readonly property alias color2RgbPaletteColor: color2Palette.rgbPaletteColor
  readonly property alias color2SpectraFilePath: color2Palette.spectraFilePath
  property int checkerWidth: 1
  property int checkerHeight: 1

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "resolution"
    }

    RowLayout {
      id: row1
      Layout.fillWidth: true
      Layout.preferredHeight: 20

      NLabel {
        Layout.preferredWidth: Definitions.defaultBlockSize
        text: "w"
      }

      NSpinBox {
        id: xResolution
        Layout.fillWidth: true
        Layout.fillHeight: true
        from: 1
        to: Definitions.intMax
        value: textureItem.checkerWidth

        onValueChanged: textureItem.checkerWidth = value
      }
    }

    RowLayout {
      id: row2
      Layout.fillWidth: true
      Layout.preferredHeight: 20

      NLabel {
        Layout.preferredWidth: Definitions.defaultBlockSize
        text: "h"
      }

      NSpinBox {
        id: yResolution
        Layout.fillWidth: true
        Layout.fillHeight: true
        from: 1
        to: Definitions.intMax
        value: textureItem.checkerHeight

        onValueChanged: textureItem.checkerHeight = value
      }
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "color1"
    }

    NColorPalette {
      id: color1Palette

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      prefixKey: Definitions.color1
    }

    NLabel {
      Layout.topMargin: Definitions.defaultBlockSize
      text: "color2"
    }

    NColorPalette {
      id: color2Palette

      Layout.fillWidth: true
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      prefixKey: Definitions.color2
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    item[Definitions.width] = 1;
    item[Definitions.height] = 1;
    color1Palette.initItem(item);
    color2Palette.initItem(item);
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    checkerWidth = Definitions.getProperty(item, Definitions.width);
    checkerHeight = Definitions.getProperty(item, Definitions.height);
    color1Palette.setValue(item);
    color2Palette.setValue(item);
  }

  function getSceneData(item) {
    var sceneData = {};

    var imageResolution = [Definitions.getProperty(item, Definitions.width),
                           Definitions.getProperty(item, Definitions.height)];
    sceneData[Definitions.imageResolution] = imageResolution;
    var palette1Data = Definitions.getProperty(item, color1Palette.prefixKey);
    sceneData[color1Palette.prefixKey] = color1Palette.getSceneData(palette1Data);
    var palette2Data = Definitions.getProperty(item, color2Palette.prefixKey);
    sceneData[color2Palette.prefixKey] = color2Palette.getSceneData(palette2Data);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var imageResolution =
        Definitions.getProperty(sceneData, Definitions.imageResolution);
    item[Definitions.width] = imageResolution[0];
    item[Definitions.height] = imageResolution[1];
    var palette1Data = Definitions.getProperty(sceneData, color1Palette.prefixKey);
    color1Palette.setSceneData(palette1Data, item);
    var palette2Data = Definitions.getProperty(sceneData, color2Palette.prefixKey);
    color2Palette.setSceneData(palette2Data, item);
  }
}
