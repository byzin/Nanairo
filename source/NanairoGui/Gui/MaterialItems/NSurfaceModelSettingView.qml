/*!
  \file NSurfaceModelSettingView.qml
  \author Sho Ikeda
  
  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "TabSurfaceModelItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: surface_model_setting_view

  property int currentTypeIndex: 0
  property var textureModel: null
  property var currentItem: null
  property bool isItemChangeMode: false

  width: Nanairo.materialSettingViewWidth
  height: Nanairo.materialSettingViewHeight

  TabView {
    id: surface_model_tab_view

    width: surface_model_setting_view.width
    height: surface_model_setting_view.height - y

    style: tab_view_style

    Tab {
      title: Nanairo.smoothDiffuseSurface
      active: true

      NSmoothDiffuseSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.smoothDielectricSurface
      active: true

      NSmoothDielectricSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.smoothConductorSurface
      active: true

      NSmoothConductorSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.roughDielectricSurface
      active: true

      NRoughDielectricSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.roughConductorSurface
      active: true

      NRoughConductorSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.layeredDiffuseSurface
      active: true

      NLayeredDiffuseSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

    Tab {
      title: Nanairo.clothSurface
      active: true

      NClothSurfaceItem {
        textureModel: surface_model_setting_view.textureModel
        color: surface_model_setting_view.color
      }
    }

  }

  onCurrentTypeIndexChanged: {
    surface_model_tab_view.currentIndex = currentTypeIndex;
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

  function getSurfaceTabIndex(type) {
    var tabIndex = -1;
    for (var i = 0; i < surface_model_tab_view.count; ++i) {
      var tab = surface_model_tab_view.getTab(i);
      if (type == tab.title) {
        tabIndex = i;
        break;
      }
    }
    console.assert(tabIndex != -1, "Unsupported surface type is specified: ", type);
    return tabIndex;
  }

  function getSurfaceTab(index) {
    return surface_model_tab_view.getTab(index).item
  }

  function initializeItem(item, tabIndex) {
    var surfaceTab = getSurfaceTab(tabIndex);
    surfaceTab.initializeItem(item);
  }

  function reflectItem(tabIndex) {
    var surfaceTab = getSurfaceTab(tabIndex);
    surfaceTab.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
    for (var i = 0; i < surface_model_tab_view.count; ++i) {
      var surfaceTab = getSurfaceTab(i);
      surfaceTab.setCurrentItem(item, itemChangeMode);
    }
  }

  function getItemData(item) {
    var surfaceTab = getSurfaceTab(getSurfaceTabIndex(item[Nanairo.type]));
    return surfaceTab.getItemData(item);
  }

  function makeItem(itemData) {
    var surfaceTab = getSurfaceTab(getSurfaceTabIndex(itemData[Nanairo.type]));
    return surfaceTab.makeItem(itemData);
  }
}
