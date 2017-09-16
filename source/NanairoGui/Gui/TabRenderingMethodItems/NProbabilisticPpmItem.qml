/*!
  \file NProbabilisticPpmItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

Rectangle {
  id: probabilistic_ppm_item

  width: Nanairo.renderingTabItemWidth
  height: Nanairo.renderingTabItemHeight

  NNumOfPhotonsSettingBlock {
    id: num_of_photons_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: probabilistic_ppm_item.color
  }

  NPhotonSearchRadiusSettingBlock {
    id: photon_search_radius_setting_block
    x: num_of_photons_setting_block.x
    y: (num_of_photons_setting_block.y + num_of_photons_setting_block.height) + Nanairo.blockOffset
    color: probabilistic_ppm_item.color
  }

  NRadiusReductionRateSettingBlock {
    id: radius_reduction_rate_setting_block
    x: photon_search_radius_setting_block.x
    y: (photon_search_radius_setting_block.y + photon_search_radius_setting_block.height) + Nanairo.blockOffset
    color: probabilistic_ppm_item.color
  }

  NKNearestNeighborSettingBlock {
    id: k_nearest_neighbor_setting_block
    x: radius_reduction_rate_setting_block.x
    y: (radius_reduction_rate_setting_block.y + radius_reduction_rate_setting_block.height) + Nanairo.blockOffset
    color: probabilistic_ppm_item.color
  }

  function initialize() {
    num_of_photons_setting_block.initialize();
    photon_search_radius_setting_block.initialize();
    radius_reduction_rate_setting_block.initialize();
    k_nearest_neighbor_setting_block.initialize();
  }

  function getSceneData() {
    var dataList = [];
    dataList.push(num_of_photons_setting_block.getSceneData());
    dataList.push(photon_search_radius_setting_block.getSceneData());
    dataList.push(radius_reduction_rate_setting_block.getSceneData());
    dataList.push(k_nearest_neighbor_setting_block.getSceneData());
    return Nanairo.mergeDataList(dataList);
  }

  function setSceneData(sceneData) {
    num_of_photons_setting_block.setSceneData(sceneData);
    photon_search_radius_setting_block.setSceneData(sceneData);
    radius_reduction_rate_setting_block.setSceneData(sceneData);
    k_nearest_neighbor_setting_block.setSceneData(sceneData);
  }
}
