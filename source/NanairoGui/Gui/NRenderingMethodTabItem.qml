/*!
  \file NRenderingMethodTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Controls.Styles 1.4
import "Styles"
import "TabRenderingMethodItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: rendering_method_tab_item
  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NRenderingMethodSettingBlock {
    id: rendering_method_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: rendering_method_tab_item.color
  }

  NRayCastSettingBlock {
    id: ray_cast_setting_block
    x: rendering_method_setting_block.x
    y: (rendering_method_setting_block.y + rendering_method_setting_block.height) + Nanairo.blockOffset
    color: rendering_method_tab_item.color
  }

  NRussianRouletteSettingBlock {
    id: russian_roulette_setting_block
    x: ray_cast_setting_block.x
    y: (ray_cast_setting_block.y + ray_cast_setting_block.height) + Nanairo.blockOffset
    color: ray_cast_setting_block.color
  }

  NEyePathLightSamplerSettingBlock {
    id: eye_path_light_sampler_setting_block
    x: russian_roulette_setting_block.x
    y: (russian_roulette_setting_block.y + russian_roulette_setting_block.height) + Nanairo.blockOffset
    color: russian_roulette_setting_block.color
  }

  TabView {
    id: rendering_method_tab_view
    x: (rendering_method_setting_block.x + rendering_method_setting_block.width) + Nanairo.blockOffset
    y: 0
    width: rendering_method_tab_item.width - x
    height: rendering_method_tab_item.height
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: Nanairo.pathTracing
      active: true
      NPathTracingItem {
        color: rendering_method_tab_item.color
      }
    }

    Tab {
      title: Nanairo.lightTracing
      active: true
      NLightTracingItem {
        color: rendering_method_tab_item.color
      }
    }

    Tab {
      title: Nanairo.probabilisticPpm
      active: true
      NProbabilisticPpmItem {
        color: rendering_method_tab_item.color
      }
    }

    currentIndex: rendering_method_setting_block.currentMethodIndex

    onCurrentIndexChanged: {
      var index = currentIndex;
      if ((0 <= index) && (index < count)) {
        var item = rendering_method_tab_item.getRenderingMethodTab(currentIndex);
        item.initialize();
      }
    }
  }

  function getRenderingMethodTab(tabIndex) {
    return rendering_method_tab_view.getTab(tabIndex).item;
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(rendering_method_setting_block.getSceneData());
    dataList.push(ray_cast_setting_block.getSceneData());
    dataList.push(russian_roulette_setting_block.getSceneData());
    dataList.push(eye_path_light_sampler_setting_block.getSceneData());
    var renderingMethodTab =
        getRenderingMethodTab(rendering_method_tab_view.currentIndex);
    dataList.push(renderingMethodTab.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    rendering_method_setting_block.setSceneData(sceneData);
    ray_cast_setting_block.setSceneData(sceneData);
    russian_roulette_setting_block.setSceneData(sceneData);
    eye_path_light_sampler_setting_block.setSceneData(sceneData);
    var renderingMethodTab =
        getRenderingMethodTab(rendering_method_tab_view.currentIndex);
    renderingMethodTab.setSceneData(sceneData);
  }
}
