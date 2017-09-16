/*!
  \file NCameraTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "TabObjectItems"
import "TabCameraItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: camera_tab_item

  width: Nanairo.objectSettingTabWidth
  height: Nanairo.objectSettingTabHeight

  NObjectNameSettingBlock {
    id: camera_name_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: camera_tab_item.color

    labelText: qsTr("CameraName")
  }

  NCameraTypeSettingBlock {
    id: camera_type_setting_block
    x: camera_name_setting_block.x
    y: (camera_name_setting_block.y + camera_name_setting_block.height) + Nanairo.blockOffset
    color: camera_tab_item.color
  }

  NJitteringSettingBlock {
    id: jittering_setting_block
    x: camera_type_setting_block.x
    y: (camera_type_setting_block.y + camera_type_setting_block.height) + Nanairo.blockOffset
    color: camera_tab_item.color
  }

  TabView {
    id: camera_tab_view
    x: (camera_name_setting_block.x + camera_name_setting_block.width) + Nanairo.blockOffset
    y: 0
    width: camera_tab_item.width - x
    height: camera_tab_item.height
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: Nanairo.pinholeCamera
      active: true
      NPinholeCameraItem {
        color: camera_tab_item.color
      }
    }

    currentIndex: camera_type_setting_block.cameraTypeIndex
  }

  function getCameraTabIndex(type) {
    var tabIndex = -1;
    for (var i = 0; i < camera_tab_view.count; ++i) {
      var tab = camera_tab_view.getTab(i);
      if (type == tab.title) {
        tabIndex = i;
        break;
      }
    }
    console.assert(tabIndex != -1, "Unsupported camera type is specified: ", type);
    return tabIndex;
  }

  function getCameraTab(index) {
    return camera_tab_view.getTab(index).item;
  }

  function initializeItem(item) {
    camera_type_setting_block.initializeItem(item);
    jittering_setting_block.initializeItem(item);
    var cameraTab = getCameraTab(0);
    cameraTab.initializeItem(item);
  }

  function reflectItem() {
    camera_name_setting_block.reflectItem();
    camera_type_setting_block.reflectItem();
    jittering_setting_block.reflectItem();
    var cameraTab = getCameraTab(camera_type_setting_block.cameraTypeIndex);
    cameraTab.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    camera_name_setting_block.setCurrentItem(item, itemChangeMode);
    camera_type_setting_block.setCurrentItem(item, itemChangeMode);
    jittering_setting_block.setCurrentItem(item, itemChangeMode);
    for (var i = 0; i < camera_tab_view.count; ++i) {
      var cameraTab = getCameraTab(i);
      cameraTab.setCurrentItem(item, itemChangeMode);
    }
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(camera_type_setting_block.getItemData(item));
    dataList.push(jittering_setting_block.getItemData(item));
    var cameraTab = getCameraTab(getCameraTabIndex(item[Nanairo.cameraType]));
    dataList.push(cameraTab.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(camera_type_setting_block.makeItem(itemData));
    dataList.push(jittering_setting_block.makeItem(itemData));
    var cameraTab = getCameraTab(getCameraTabIndex(itemData[Nanairo.cameraType]));
    dataList.push(cameraTab.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
