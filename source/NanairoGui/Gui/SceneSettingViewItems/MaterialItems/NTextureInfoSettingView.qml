/*!
  \file NTextureInfoSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../Items"
import "TextureModelItems"
import "../../definitions.js" as Definitions

NScrollView {
  id: infoSettingView

  property var materialItem: null
  property string textureType: ""

  GridLayout {
    columns: 3
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "texture type"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: textureTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: find(infoSettingView.textureType)
          model: [Definitions.valueTexture,
                  Definitions.unicolorTexture,
                  Definitions.checkerboardTexture,
                  Definitions.imageTexture]

          onCurrentTextChanged: infoSettingView.textureType = currentText
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "texture parameters"
      color: infoSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: textureItemLayout

        anchors.fill: parent
        currentIndex: 0

        NValueTextureItem {
          id: valueTextureItem
          onTextureValueChanged: infoSettingView.setProperty(Definitions.value, textureValue)
        }

        NUnicolorTextureItem {
          id: unicolorTextureItem
          onColorModeChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, palettePrefixKey);
              Definitions.setProperty(properties, Definitions.colorMode, colorMode);
            }
          }
          onRgbPaletteColorChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, palettePrefixKey);
              var c = Definitions.copyColor(rgbPaletteColor);
              Definitions.setProperty(properties, Definitions.rgb, c);
            }
          }
          onSpectraFilePathChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, palettePrefixKey);
              Definitions.setProperty(properties, Definitions.spectra, spectraFilePath);
            }
          }
        }

        NCheckerboardTextureItem {
          id: checkerboardTextureItem
          onCheckerWidthChanged: infoSettingView.setProperty(Definitions.width, checkerWidth)
          onCheckerHeightChanged: infoSettingView.setProperty(Definitions.height, checkerHeight)
          onColor1ColorModeChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color1PalettePrefixKey);
              Definitions.setProperty(properties, Definitions.colorMode, color1ColorMode);
            }
          }
          onColor1RgbPaletteColorChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color1PalettePrefixKey);
              var c = Definitions.copyColor(color1RgbPaletteColor);
              Definitions.setProperty(properties, Definitions.rgb, c);
            }
          }
          onColor1SpectraFilePathChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color1PalettePrefixKey);
              Definitions.setProperty(properties, Definitions.spectra, color1SpectraFilePath);
            }
          }
          onColor2ColorModeChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color2PalettePrefixKey);
              Definitions.setProperty(properties, Definitions.colorMode, color2ColorMode);
            }
          }
          onColor2RgbPaletteColorChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color2PalettePrefixKey);
              var c = Definitions.copyColor(color2RgbPaletteColor);
              Definitions.setProperty(properties, Definitions.rgb, c);
            }
          }
          onColor2SpectraFilePathChanged: {
            if (materialItem != null) {
              var properties = Definitions.getProperty(materialItem, color2PalettePrefixKey);
              Definitions.setProperty(properties, Definitions.spectra, color2SpectraFilePath);
            }
          }
        }

        NImageTextureItem {
          id: imageTextureItem
          onImageFilePathChanged: infoSettingView.setProperty(Definitions.imageFilePath, imageFilePath)
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < textureItemLayout.children.length; ++i) {
          var textureView = textureItemLayout.children[i];
          textureView.background.color = background.color;
        }
      }
    }
  }

  onTextureTypeChanged: {
    var viewIndex = textureTypeComboBox.find(textureType);
    if ((materialItem != null) && (viewIndex != -1)) {
      // Set base texture properties
      setProperty(Definitions.type, textureType);
      // Set other texture properties
      var settingView = textureItemLayout.children[viewIndex];
      settingView.initItem(materialItem);
      // Change the texture setting view
      textureItemLayout.currentIndex = viewIndex;
      // Set the texture properties to the view
      settingView.setValue(materialItem);
    }
  }

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    // Set the properties of value texture
    item[Definitions.type] = Definitions.valueTexture;
    valueTextureItem.initItem(item);
  }

  function setProperty(propertyName, value) {
    if (materialItem != null)
      Definitions.setProperty(materialItem, propertyName, value);
  }

  function setItem(item) {
    materialItem = null;
    if (item != null) {
      // Set texture type to the view
      textureType = Definitions.getProperty(item, Definitions.type);
      // Change the texture setting view
      var viewIndex = textureTypeComboBox.find(textureType);
      console.assert(viewIndex != -1, "The item has invalid texture type.");
      textureItemLayout.currentIndex = viewIndex;
      // Set the item properties to the view
      var settingView = textureItemLayout.children[viewIndex];
      settingView.setValue(item);
    }
    materialItem = item;
  }

  function getSceneData(item) {
    var type = Definitions.getProperty(item, Definitions.type);

    var viewIndex = textureTypeComboBox.find(type);
    var textureView = textureItemLayout.children[viewIndex];

    var sceneData = textureView.getSceneData(item);
    sceneData[Definitions.type] = type;

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var type = Definitions.getProperty(sceneData, Definitions.type);

    var viewIndex = textureTypeComboBox.find(type);
    var textureView = textureItemLayout.children[viewIndex];

    item[Definitions.type] = type;
    textureView.setSceneData(sceneData, item);
  }
}
