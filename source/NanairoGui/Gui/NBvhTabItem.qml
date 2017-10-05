/*!
  \file NBvhTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "Styles"
import "TabBvhItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: bvh_tab_item

  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NBvhSettingBlock {
    id: bvh_setting_block

    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: bvh_tab_item.color
  }

  TabView {
    id: bvh_tab_view
    x: (bvh_setting_block.x + bvh_setting_block.width) + Nanairo.blockOffset
    y: 0
    width: bvh_tab_item.width - x
    height: bvh_tab_item.height
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: Nanairo.binaryRadixTreeBvh
      active: true
      NBinaryRadixTreeBvhItem {
        color: bvh_tab_item.color
      }
    }

    Tab {
      title: Nanairo.agglomerativeTreeletRestructuringBvh
      active: true
      NAgglomerativeTreeletRestructuringBvhItem {
        color: bvh_tab_item.color
      }
    }

    currentIndex: bvh_setting_block.currentBvhIndex

    onCurrentIndexChanged: {
      var index = currentIndex;
      if ((0 <= index) && (index < count)) {
        var item = bvh_tab_item.getBvhTab(currentIndex);
        item.initialize();
      }
    }
  }

  function getBvhTab(tabIndex) {
    return bvh_tab_view.getTab(tabIndex).item;
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(bvh_setting_block.getSceneData());
    var bvhTab = getBvhTab(bvh_tab_view.currentIndex);
    dataList.push(bvhTab.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    bvh_setting_block.setSceneData(sceneData);
    var bvhTab = getBvhTab(bvh_tab_view.currentIndex);
    bvhTab.setSceneData(sceneData);
  }
}
