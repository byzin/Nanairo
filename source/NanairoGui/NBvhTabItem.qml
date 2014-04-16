/*!
  \file NBvhTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "BvhItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: bvh_tab_item 
  width: 640
  height: 274

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: bvh_tab_item.color
    }
  }

  ComboBox {
    id: bvh_type_combobox
    x: 8
    y: 8
    width: 130
    height: 20
    style: combobox_style

    model: [Keyword.binaryRadixTreeBvh,
            Keyword.approximateAgglomerativeClusteringBvh,
            Keyword.agglomerativeTreeletRestructuringBvh]
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
    id: bvh_tab_view
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
      title: Keyword.binaryRadixTreeBvh
      active: true
      NBinaryRadixTreeBvhItem {
        color: bvh_tab_item.color
      }
    }

    Tab {
      title: Keyword.approximateAgglomerativeClusteringBvh
      active: true
      NApproximateAgglomerativeClusteringBvhItem {
        color: bvh_tab_item.color
      }
    }

    Tab {
      title: Keyword.agglomerativeTreeletRestructuringBvh
      active: true
      NAgglomerativeTreeletRestructuringBvhItem {
        color: bvh_tab_item.color
      }
    }

    currentIndex: bvh_type_combobox.currentIndex

    onCurrentIndexChanged: {
      if (currentIndex != -1) {
        var tabItem = getBvhTabItem(currentIndex)
        tabItem.setDefault()
      }
    }
  }

  function getBvhTabItem(index) {
    return bvh_tab_view.getTab(index).item
  }

  function loadSceneSettings() {
    var prefix = Keyword.bvh + "/"
    bvh_type_combobox.currentIndex = bvh_type_combobox.find(
      scene_settings.stringValue(prefix + Keyword.bvhType, 
                                 Keyword.approximateAgglomerativeClusteringBvh))
    var tabItem = getBvhTabItem(bvh_type_combobox.currentIndex)
    tabItem.loadSceneSettings(prefix)
  }

  function saveSceneSettings() {
    var prefix = Keyword.bvh + "/"
    scene_settings.setStringValue(prefix + Keyword.bvhType,
                                  bvh_type_combobox.currentText)
    var tabItem = getBvhTabItem(bvh_type_combobox.currentIndex)
    tabItem.saveSceneSettings(prefix)
  }
}
