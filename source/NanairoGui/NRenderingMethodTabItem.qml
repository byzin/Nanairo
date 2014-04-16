/*!
  \file NRenderingMethodTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "RenderingMethodItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: rendering_method_tab_item 
  width: 640
  height: 274

  readonly property real defaultRayCastEpsilon: 0.0000001
  readonly property int defaultPathLength: 3

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: rendering_method_tab_item.color
    }
  }

  Component {
    id: spinbox_style
    NSpinBoxStyle {
    }
  }

  ComboBox {
    id: rendering_method_type_combobox
    x: 8
    y: 8
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.pathTracing,
            Keyword.lightTracing,
            Keyword.probabilisticPpm]
  }

  Label {
    id: ray_cast_label
    x: 8
    y: 37
    text: qsTr("Ray cast")
    font.bold: true
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  SpinBox {
    id: ray_cast_epsilon_spinbox
    x: 16
    y: 59
    width: 130
    height: 20
    font.pixelSize: 10
    style: spinbox_style

    horizontalAlignment: Qt.AlignLeft
    prefix: qsTr("Epsilon") + ": "
    decimals: 7
    maximumValue: 1
    minimumValue: 0.0000001
  }

  Label {
    id: russian_roulette_label
    x: 16
    y: 96
    text: qsTr("Russian roulette")
    textFormat: Text.PlainText
    verticalAlignment: Text.AlignVCenter
  }

  ComboBox {
    id: russian_roulette_combobox
    x: 16
    y: 113
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.rouletteMaxReflectance,
            Keyword.rouletteAverageReflectance,
            Keyword.roulettePathLength]

    onCurrentIndexChanged: {
      path_length_spinbox.value = defaultPathLength
    }
  }

  SpinBox {
    id: path_length_spinbox
    x: 16
    y: 139
    width: 100
    height: 20
    enabled: russian_roulette_combobox.currentText == Keyword.roulettePathLength
    font.pixelSize: 10
    style: NSpinBoxButtonStyle {}

    horizontalAlignment: Qt.AlignLeft
    maximumValue: 99
    minimumValue: 1
    decimals: 0
    prefix: qsTr("Path") + ": "
  }

  Rectangle {
    id: separator
    x: 159
    y: 32
    width: 1
    height: 210
    color: Qt.darker(parent.color, 1.1)
  }

  TabView {
    id: rendering_method_tab_view
    x: 160
    y: 0
    width: 480
    height: 274
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: Keyword.pathTracing
      active: true
      NPathTracingItem {
        color: rendering_method_tab_item.color
      }
    }

    Tab {
      title: Keyword.lightTracing
      active: true
      NLightTracingItem {
        color: rendering_method_tab_item.color
      }
    }

    Tab {
      title: Keyword.probabilisticPpm
      active: true
      NProbabilisticPpmItem {
        color: rendering_method_tab_item.color
      }
    }

    currentIndex: rendering_method_type_combobox.currentIndex

    onCurrentIndexChanged: {
      if (currentIndex != -1) {
        var tabItem = getRenderingMethodTabItem(currentIndex)
        tabItem.setDefault()
      }
    }
  }

  function getRenderingMethodTabItem(index) {
    return rendering_method_tab_view.getTab(index).item
  }

  function loadSceneSettings() {
    var prefix = Keyword.renderingMethod + "/"
    rendering_method_type_combobox.currentIndex = rendering_method_type_combobox.find(
        scene_settings.stringValue(prefix + Keyword.renderingMethodType, 
                                   Keyword.pathTracing))
    ray_cast_epsilon_spinbox.value =
        scene_settings.realValue(prefix + Keyword.rayCastEpsilon, 
                                 defaultRayCastEpsilon)
    russian_roulette_combobox.currentIndex = russian_roulette_combobox.find(
        scene_settings.stringValue(prefix + Keyword.russianRoulette, 
                                   Keyword.rouletteMaxReflectance))
    if (russian_roulette_combobox.currentText == Keyword.roulettePathLength) {
      path_length_spinbox.value = 
          scene_settings.intValue(prefix + Keyword.pathLength, defaultPathLength)
    }
    var tabItem = getRenderingMethodTabItem(rendering_method_type_combobox.currentIndex)
    tabItem.loadSceneSettings(prefix)
  }

  function saveSceneSettings() {
    var prefix = Keyword.renderingMethod + "/"
    scene_settings.setStringValue(prefix + Keyword.renderingMethodType,
                                  rendering_method_type_combobox.currentText)
    scene_settings.setRealValue(prefix + Keyword.rayCastEpsilon,
                                ray_cast_epsilon_spinbox.value)
    scene_settings.setStringValue(prefix + Keyword.russianRoulette,
                                  russian_roulette_combobox.currentText)
    if (russian_roulette_combobox.currentText == Keyword.roulettePathLength) {
      scene_settings.setIntValue(prefix + Keyword.pathLength,
                                 path_length_spinbox.value)
    }
    var tabItem = getRenderingMethodTabItem(rendering_method_type_combobox.currentIndex)
    tabItem.saveSceneSettings(prefix)
  }
}
