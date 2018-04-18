/*!
  \file NTransformationListItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.3
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: transformationItem

  readonly property int fontSize: 10
  readonly property int elementItemSize: 80
  readonly property bool isRotationTransformation: itemTransformationType == Definitions.rotation
  property color backgroundColor: Qt.White
  property bool isFocused: false
  // Item property
  property int itemIndex: -1
  property bool itemEnabled: false 
  property string itemTransformationType: ""
  property real itemX: 0.0
  property real itemY: 0.0
  property real itemZ: 0.0
  property string itemAxis: ""
  property string itemUnit: ""

  width: 3 * elementItemSize
  height: 2 * Definitions.defaultBlockSize + 3 * Definitions.defaultItemSpace

  MouseArea {
    anchors.fill: parent
    onClicked: itemClicked()
  }

  RowLayout {
    id: labelLayout

    y: Definitions.defaultItemSpace
    width: transformationItem.width
    height: Definitions.defaultBlockSize
    spacing: 0

    NCheckBox {
      id: enabledCheckBox

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      indicator.width: Definitions.defaultBlockSize - 4
      indicator.height: Definitions.defaultBlockSize - 4
      contentItem.enabled: false
      contentItem.visible: false
      text: ""
      checked: transformationItem.itemEnabled

      onCheckedChanged: transformationItem.itemEnabled = checked
    }

    NLabel {
      id: indexLabel

      Layout.preferredWidth: 2 * Definitions.defaultBlockSize
      Layout.fillHeight: true
      font.pixelSize: transformationItem.fontSize
      leftPadding: Definitions.defaultItemSpace
      horizontalAlignment: Text.AlignLeft
      verticalAlignment: Text.AlignVCenter
      text: Number(transformationItem.itemIndex + 1).toString()
    }

    NLabel {
      id: nameLabel

      Layout.fillWidth: true
      Layout.fillHeight: true
      rightPadding: indexLabel.width
      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      text: transformationItem.itemTransformationType
    }
  }

  RowLayout {
    id: valueLayout

    y: (labelLayout.y + labelLayout.height) + Definitions.defaultItemSpace
    width: transformationItem.width
    height: Definitions.defaultBlockSize
    spacing: 0

    NLabel {
      id: xLabel

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      font.pixelSize: transformationItem.fontSize
      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignBottom
      text: transformationItem.isRotationTransformation ? "axs" : "x"
    }

    Component {
      id: xSpinBoxComponent
      NFloatSpinBox {
        id: xSpinBox

        font.pixelSize: transformationItem.fontSize
        decimals: 4
        floatFrom: -realMax 
        floatTo: realMax
        floatValue: transformationItem.itemX

        onFloatValueChanged: transformationItem.itemX = floatValue
      }
    }

    Component {
      id: axisComboBoxComponent
      NComboBox {
        id: axisComboBox

        contentItem.anchors.fill: axisComboBox
        indicator.enabled: false
        indicator.visible: false
        font.pixelSize: transformationItem.fontSize
        popup.font.pixelSize: transformationItem.fontSize
        model: [Definitions.xAxis, Definitions.yAxis, Definitions.zAxis]
        currentIndex: find(transformationItem.itemAxis)

        Component.onCompleted: {
          currentIndex = find(transformationItem.itemAxis)
          contentItem.padding = 0;
          contentItem.leftPadding = 0;
          contentItem.rightPadding = 0;
          contentItem.topPadding = 0;
          contentItem.bottomPadding = 0;
          contentItem.horizontalAlignment = TextInput.AlignHCenter;
          contentItem.verticalAlignment = TextInput.AlignVCenter;
        }

        onCurrentTextChanged: transformationItem.itemAxis = currentText
      }
    }

    Loader {
      id: xLoader

      Layout.preferredWidth: (transformationItem.width - 3 * Definitions.defaultBlockSize) / 3
      Layout.fillHeight: true
      sourceComponent: transformationItem.isRotationTransformation
          ? axisComboBoxComponent
          : xSpinBoxComponent
    }

    NLabel {
      id: yLabel

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      font.pixelSize: transformationItem.fontSize
      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignBottom
      text: transformationItem.isRotationTransformation ? "ang" : "y"
    }

    NFloatSpinBox {
      id: ySpinBox

      Layout.preferredWidth: (transformationItem.width - 3 * Definitions.defaultBlockSize) / 3
      Layout.fillHeight: true
      decimals: 4
      font.pixelSize: transformationItem.fontSize
      floatFrom: -realMax 
      floatTo: realMax
      floatValue: transformationItem.itemY

      onFloatValueChanged: transformationItem.itemY = floatValue
    }

    NLabel {
      id: zLabel

      Layout.preferredWidth: Definitions.defaultBlockSize
      Layout.fillHeight: true
      font.pixelSize: transformationItem.fontSize
      horizontalAlignment: Text.AlignRight
      verticalAlignment: Text.AlignBottom
      text: transformationItem.isRotationTransformation ? "unt" : "z"
    }

    Component {
      id: zSpinBoxComponent
      NFloatSpinBox {
        id: zSpinBox

        font.pixelSize: transformationItem.fontSize
        decimals: 4
        floatFrom: -realMax 
        floatTo: realMax
        floatValue: transformationItem.itemZ

        onFloatValueChanged: transformationItem.itemZ = floatValue
      }
    }

    Component {
      id: unitComboBoxComponent
      NComboBox {
        id: unitComboBox

        background.anchors.fill: unitComboBox
        contentItem.anchors.fill: unitComboBox
        indicator.enabled: false
        indicator.visible: false
        font.pixelSize: transformationItem.fontSize
        popup.font.pixelSize: transformationItem.fontSize
        model: [Definitions.degreeUnit, Definitions.radianUnit]
        currentIndex: find(transformationItem.itemUnit)

        Component.onCompleted: {
          currentIndex = find(transformationItem.itemUnit);
          contentItem.padding = 0;
          contentItem.leftPadding = 0;
          contentItem.rightPadding = 0;
          contentItem.topPadding = 0;
          contentItem.bottomPadding = 0;
          contentItem.horizontalAlignment = TextInput.AlignHCenter;
          contentItem.verticalAlignment = TextInput.AlignVCenter;
        }

        onCurrentTextChanged: transformationItem.itemUnit = currentText
      }
    }

    Loader {
      id: zLoader

      Layout.preferredWidth: (transformationItem.width - 3 * Definitions.defaultBlockSize) / 3
      Layout.fillHeight: true
      sourceComponent: transformationItem.isRotationTransformation
          ? unitComboBoxComponent
          : zSpinBoxComponent
    }
  }

  onIsFocusedChanged: setBackgroundColor()

  onItemIndexChanged: setBackgroundColor()

  function setBackgroundColor() {
    background.color = (isFocused)
        ? Material.color(Material.Lime)
        : (itemIndex % 2 == 0)
            ? backgroundColor
            : Qt.darker(backgroundColor, 1.1);
  }

  signal itemClicked()
}
