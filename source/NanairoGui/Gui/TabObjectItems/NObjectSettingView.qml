/*!
  \file NObjectSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../Styles"
import "../nanairo.js" as Nanairo

Rectangle {
  id: object_setting_view

  property var emitterModel: null
  property var surfaceModel: null
  property var currentItem: null
  property bool isItemChangeMode: false

  width: Nanairo.objectSettingViewWidth
  height: Nanairo.objectSettingViewHeight

  TabView {
    id: object_tab_view

    x: 0
    y: 8
    width: object_setting_view.width
    height: object_setting_view.height - y

    style: tab_view_style
  
    Tab {
      title: "Camera"
      active: true
      enabled: isCameraObject(currentItem)
      NCameraTabItem {
        color: object_setting_view.color
      }
    }
 
    Tab {
      title: "Object"
      active: true
      enabled: isSingleObject(currentItem)
      NSingleObjectTabItem {
        color: object_setting_view.color
        emitterModel: object_setting_view.emitterModel
        surfaceModel: object_setting_view.surfaceModel
      }
    }
  
    Tab {
      title: "Group"
      active: true
      enabled: isGroupObject(currentItem)
      NGroupTabItem {
        color: object_setting_view.color
      }
    }
  
    Tab {
      title: "Transformation"
      active: true
      NTransformationTabItem {
        color: object_setting_view.color
      }
    }
  }

  Component {
    id: tab_view_style

    NTabViewStyle {
      backgroundColor: object_setting_view.color
    }
  }

  function isCameraObject(item) {
    return (item != null) && (item["type"] == Nanairo.cameraObject);
  }

  function isSingleObject(item) {
    return (item != null) && (item["type"] == Nanairo.singleObject);
  }

  function isGroupObject(item) {
    return (item != null) && (item["type"] == Nanairo.groupObject);
  }

  function getObjectTabIndex(type) {
    // Check object type
    var tabIndex = -1;
    switch (type) {
      case Nanairo.cameraObject:
        tabIndex = 0;
        break;
      case Nanairo.singleObject:
        tabIndex = 1;
        break;
      case Nanairo.groupObject:
        tabIndex = 2;
        break;
      default:
        console.assert(false, "Unsupported object type is specified: ", type);
        break;
    }
    return tabIndex;
  }

  function getObjectTab(tabIndex) {
    var objectTab = object_tab_view.getTab(tabIndex).item;
    return objectTab;
  }

  function initializeItem(item) {
    var objectType = item["type"];
    var tabIndex = getObjectTabIndex(objectType);
    var objectTab = getObjectTab(tabIndex);
    objectTab.initializeItem(item);
    var transformationTab = getObjectTab(3);
    transformationTab.initializeItem(item);
  }

  function reflectItem() {
    var item = currentItem;
    var tabIndex = getObjectTabIndex(item["type"]);
    var objectTab = getObjectTab(tabIndex);
    objectTab.reflectItem();
    object_tab_view.currentIndex = tabIndex;
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
    for (var i = 0; i < object_tab_view.count; ++i) {
      var objectTab = getObjectTab(i);
      objectTab.setCurrentItem(item, itemChangeMode);
    }
  }

  function getItemData(item) {
    var objectTab = getObjectTab(getObjectTabIndex(item["type"]));
    var itemData = objectTab.getItemData(item);

    var transformationTab = getObjectTab(3);
    itemData[Nanairo.transformation] = 
        transformationTab.getTransformationData(item[Nanairo.transformation]);

    return itemData;
  }

  function makeItem(itemData) {
    var objectTab = getObjectTab(getObjectTabIndex(itemData[Nanairo.type]));
    var item = objectTab.makeItem(itemData);

    var transformationTab = getObjectTab(3);
    item[Nanairo.transformation] =
        transformationTab.makeTransformationItem(itemData[Nanairo.transformation]);

    return item;
  }

  function addCameraEvent(matrix) {
    console.log("CameraEvent: ", matrix);
  }

  Connections {
    target: nanairoManager
    onCameraEventHandled: addCameraEvent(matrix)
  }
}
