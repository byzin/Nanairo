/*!
  \file NPinholeCameraItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../nanairo.js" as Nanairo

Rectangle {
  id: pinhole_camera_item

  width: Nanairo.cameraTabItemWidth
  height: Nanairo.cameraTabItemHeight

  NAngleOfViewSettingBlock {
    id: angle_of_view_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: pinhole_camera_item.color
  }

  function initializeItem(item) {
    angle_of_view_setting_block.initializeItem(item);
  }

  function reflectItem() {
    angle_of_view_setting_block.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    angle_of_view_setting_block.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    return angle_of_view_setting_block.getItemData(item);
  }

  function makeItem(itemData) {
    return angle_of_view_setting_block.makeItem(itemData);
  }
}
