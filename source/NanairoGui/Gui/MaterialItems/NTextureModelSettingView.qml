/*!
  \file NTextureModelSettingView.qml
  \author Sho Ikeda
  
  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "TabTextureModelItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: texture_model_setting_view

  property int currentTypeIndex: 0
  property var currentItem: null
  property bool isItemChangeMode: false

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  TabView {
    id: texture_model_tab_view

    width: texture_model_setting_view.width
    height: texture_model_setting_view.height - y

    style: tab_view_style

    Tab {
      title: Nanairo.valueTexture
      active: true
      NValueTextureItem {
        color: texture_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.unicolorTexture
      active: true
      NUnicolorTextureItem {
        color: texture_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.checkerboardTexture
      active: true
      NCheckerboardTextureItem {
        color: texture_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.imageTexture
      active: true
      NImageTextureItem {
        color: texture_model_setting_view.color
      }
    }
  }

  onCurrentTypeIndexChanged: {
    texture_model_tab_view.currentIndex = currentTypeIndex;
    if ((currentItem != null) && (!isItemChangeMode)) {
      initializeItem(currentItem, currentTypeIndex);
      reflectItem(currentTypeIndex);
    }
  }

  Component {
    id: tab_view_style

    TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }
  }

  function getTextureTabIndex(type) {
    var tabIndex = -1;
    for (var i = 0; i < texture_model_tab_view.count; ++i) {
      var tab = texture_model_tab_view.getTab(i);
      if (type == tab.title) {
        tabIndex = i;
        break;
      }
    }
    console.assert(tabIndex != -1, "Unsupported texture type is specified: ", type);
    return tabIndex;
  }

  function getTextureTab(index) {
    return texture_model_tab_view.getTab(index).item;
  }

  function initializeItem(item, tabIndex) {
    var textureTab = getTextureTab(tabIndex);
    textureTab.initializeItem(item);
  }

  function reflectItem(tabIndex) {
    var textureTab = getTextureTab(tabIndex);
    textureTab.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
    for (var i = 0; i < texture_model_tab_view.count; ++i) {
      var textureTab = getTextureTab(i);
      textureTab.setCurrentItem(item, itemChangeMode);
    }
  }

  function getItemData(item) {
    var textureTab = getTextureTab(getTextureTabIndex(item[Nanairo.type]));
    return textureTab.getItemData(item);
  }

  function makeItem(itemData) {
    var textureTab = getTextureTab(getTextureTabIndex(itemData[Nanairo.type]));
    return textureTab.makeItem(itemData);
  }
}
