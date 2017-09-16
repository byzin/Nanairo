/*!
  \file NImageResolutionSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../Styles"
import "../nanairo.js" as Nanairo

NSettingBlock {
  id: image_resolution_setting_block

  labelText: qsTr("ImageResolution")

  NIntSpinBox {
    id: width_resolution_spinbox

    anchors.horizontalCenter: undefined
    x: cross_label.x - (width + 5)
    y: image_resolution_setting_block.labelHeight + Nanairo.itemOffset
    width: 60
    height: 20

    maximumValue: 3840
    minimumValue: 320
  }

  Label {
    id: cross_label

    anchors.horizontalCenter: image_resolution_setting_block.horizontalCenter
    y: width_resolution_spinbox.y

    text: qsTr("x")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
    horizontalAlignment: Text.AlignHCenter
  }

  NIntSpinBox {
    id: height_resolution_spinbox

    anchors.horizontalCenter: undefined
    x: cross_label.x + cross_label.width + 5
    y: cross_label.y
    width: 60
    height: 20

    maximumValue: 2160
    minimumValue: 240
  }

  NComboBox {
    id: image_resolution_template

    y: (height_resolution_spinbox.y + height_resolution_spinbox.height) + Nanairo.itemOffset

    style: resolution_combobox_style
    model: ["---",
            // 4:3
            "Quarter VGA: 320x240",
            "VGA: 640x480",
            "Super VGA: 800x600",
            "XGA: 1024x768",
            "Quad VGA: 1280x960",
            "Ultra XGA: 1600x1200",
            "Quad XGA: 2048x1536",
            // 16:9
            "Wide QVGA+: 427x240",
            "Half VGA Wide: 640x360",
            "Quarter HD: 960x540",
            "HD: 1280x720",
            "HD+: 1600x900",
            "Full HD: 1920x1080",
            "Wide Quad HD: 2560x1440",
            "4K UHDTV: 3840x2160"
            ]

    onCurrentIndexChanged: {
      var resolutionIsSpecified = 0 < image_resolution_template.currentIndex;
      if (resolutionIsSpecified) {
        var templateSize = image_resolution_template.currentText;
        var regex = /.+:\s(\d+)x(\d+)/;
        templateSize.match(regex);
        width_resolution_spinbox.value = Number(RegExp.$1);
        height_resolution_spinbox.value = Number(RegExp.$2);
        image_resolution_template.currentIndex = 0;
      }
    }
  }

  Component {
    id: resolution_combobox_style
    NComboBoxStyle {
      backgroundColor: image_resolution_setting_block.color
      label: Label {
        text: qsTr("Template size")
        font.pixelSize: 10
        horizontalAlignment: Text.AlignHLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.PlainText
      }
    }
  }

  function getImageResolution() {
    return [width_resolution_spinbox.value, height_resolution_spinbox.value];
  }

  function getSceneData() {
    var sceneData = {};
    sceneData[Nanairo.imageResolution] = [width_resolution_spinbox.value,
                                          height_resolution_spinbox.value];
    return sceneData;
  }

  function setSceneData(sceneData) {
    var imageResolution = sceneData[Nanairo.imageResolution];
    width_resolution_spinbox.value = imageResolution[0];
    height_resolution_spinbox.value = imageResolution[1];
  }
}
