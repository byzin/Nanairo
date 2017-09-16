/*!
  \file NEmitterModelSettingView.qml
  \author Sho Ikeda
  
  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "TabEmitterModelItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: emitter_model_setting_view

  property int currentTypeIndex: 0
  property var currentItem: null
  property bool isItemChangeMode: false
  property var textureModel: null

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  TabView {
    id: emitter_model_tab_view

    width: emitter_model_setting_view.width
    height: emitter_model_setting_view.height - y

    style: tab_view_style

    Tab {
      title: Nanairo.nonDirectionalEmitter
      active: true

      NNonDirectionalEmitterItem {
        textureModel: emitter_model_setting_view.textureModel
        color: emitter_model_setting_view.color
      }
    }
  }

  onCurrentTypeIndexChanged: {
    emitter_model_tab_view.currentIndex = currentTypeIndex;
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

  function getEmitterTabIndex(type) {
    var tabIndex = -1;
    for (var i = 0; i < emitter_model_tab_view.count; ++i) {
      var tab = emitter_model_tab_view.getTab(i);
      if (type == tab.title) {
        tabIndex = i;
        break;
      }
    }
    console.assert(tabIndex != -1, "Unsupported emitter type is specified: ", type);
    return tabIndex;
  }
 
  function getEmitterTab(index) {
    return emitter_model_tab_view.getTab(index).item;
  }

  function initializeItem(item, tabIndex) {
    var emitterTab = getEmitterTab(tabIndex);
    emitterTab.initializeItem(item);
  }

  function reflectItem(tabIndex) {
    var emitterTab = getEmitterTab(tabIndex);
    emitterTab.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
    for (var i = 0; i < emitter_model_tab_view.count; ++i) {
      var emitterTab = getEmitterTab(i);
      emitterTab.setCurrentItem(item, itemChangeMode);
    }
  }

  function getItemData(item) {
    var emitterTab = getEmitterTab(getEmitterTabIndex(item[Nanairo.type]));
    return emitterTab.getItemData(item);
  }

  function makeItem(itemData) {
    var emitterTab = getEmitterTab(getEmitterTabIndex(itemData[Nanairo.type]));
    return emitterTab.makeItem(itemData);
  }
}
