/*!
  \file NColorPalette.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import Qt.labs.platform 1.0
import "../../../Items"
import "../../../definitions.js" as Definitions

NPane {
  id: colorPalette

  readonly property int paletteSpace: 2
  readonly property int colorSwitchWidth: 2 * Definitions.defaultBlockSize
  readonly property color defaultRgbColor: "#CCCCCC"
  readonly property bool isRgbMode: colorMode == Definitions.rgb
  property string prefixKey: "ColorPalette"
  // Properties
  property string colorMode: Definitions.rgb
  property color rgbPaletteColor: defaultRgbColor
  property string spectraFilePath: ""

  RowLayout {
    id: paletteLayout

    anchors.fill: parent
    anchors.margins: colorPalette.paletteSpace
    spacing: colorPalette.paletteSpace

    NButton {
      id: rgbSpectraSwitch

      Layout.preferredWidth: colorPalette.colorSwitchWidth
      Layout.fillHeight: true
      text: (colorPalette.colorMode == Definitions.rgb) ? "RGB" : "SPC"

      onClicked: {
        colorPalette.colorMode = colorPalette.isRgbMode ? Definitions.spectra
                                                        : Definitions.rgb;
        // Initialize the color palette
        colorPalette.rgbPaletteColor = Definitions.copyColor(colorPalette.defaultRgbColor);
        colorPalette.spectraFilePath = "";
      }
    }

    StackLayout {
      Layout.fillWidth: true
      Layout.fillHeight: true
      currentIndex: isRgbMode ? 0 : 1

      NButton {
        id: rgbPalette

        ColorDialog {
          id: rgbPaletteDialog

          title: "RGB color"
          color: colorPalette.rgbPaletteColor
          onColorChanged: colorPalette.rgbPaletteColor = color
        }

        Component.onCompleted: background.color = colorPalette.rgbPaletteColor
        onClicked: rgbPaletteDialog.open()
      }

      NButton {
        id: spectraPalette

        font.weight: Font.Light
        font.pixelSize: 10
        text: (colorPalette.spectraFilePath != "")
            ? nanairoManager.getFileName(colorPalette.spectraFilePath)
            : "spectra file"

        FileDialog {
          id: spectraPaletteDialog

          title: "Open spectra file"
          nameFilters: ["Spectra file (*.csv)"]

          onAccepted: colorPalette.spectraFilePath = nanairoManager.getRelativePath(file)
        }

        onClicked: spectraPaletteDialog.open()
      }
    }
  }

  onRgbPaletteColorChanged: rgbPalette.background.color = rgbPaletteColor

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    // Set the properties
    var properties = {};
    properties[Definitions.colorMode] = Definitions.rgb;
    properties[Definitions.rgb] = Definitions.copyColor(defaultRgbColor);
    properties[Definitions.spectra] = "";
    item[prefixKey] = properties;
  }

  function setValue(item) {
    console.assert(item != null, "The item is null.");
    var properties = Definitions.getProperty(item, prefixKey);
    colorMode = Definitions.getProperty(properties, Definitions.colorMode);
    var c = Definitions.getProperty(properties, Definitions.rgb);
    rgbPaletteColor = Definitions.copyColor(c);
    spectraFilePath = Definitions.getProperty(properties, Definitions.spectra);
  }

  function getSceneData(item) {
    var sceneData = {};

    var colorMode = Definitions.getProperty(item, Definitions.colorMode);
    var value = Definitions.getProperty(item, colorMode);
    sceneData[Definitions.colorMode] = colorMode;
    switch (colorMode) {
      case Definitions.rgb: {
        var rgbValue = [value.r, value.g, value.b];
        sceneData[Definitions.value] = rgbValue;
        break;
      }
      case Definitions.spectra: {
        sceneData[Definitions.value] = value;
        break;
      }
    }

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var properties = {};

    var colorMode = Definitions.getProperty(sceneData, Definitions.colorMode);
    var value = Definitions.getProperty(sceneData, Definitions.value);

    properties[Definitions.colorMode] = colorMode;
    switch (colorMode) {
      case Definitions.rgb: {
        properties[Definitions.rgb] = Qt.rgba(value[0], value[1], value[2], 1.0);
        properties[Definitions.spectra] = "";
        break;
      }
      case Definitions.spectra: {
        properties[Definitions.rgb] = Definitions.copyColor(defaultRgbColor);
        properties[Definitions.spectra] = value;
        break;
      }
    }

    item[prefixKey] = properties;
  }
}
