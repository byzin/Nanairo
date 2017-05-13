/*!
  \file NVolumeReflectanceGaussianWidthSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NSettingBlock {
  id: volume_reflectance_gaussian_width_setting_block_item

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("Volume gaussian width")

  NSpinBox {
    id: volume_reflectance_gaussian_width_spinbox

    y: volume_reflectance_gaussian_width_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: Nanairo.integer32Max
    minimumValue: 0.0000001

    onValueChanged: setCurrentItemValue(Nanairo.volumeReflectanceGaussianWidth, value)
  }

  function initializeItem(item) {
    item[Nanairo.volumeReflectanceGaussianWidth] = 1.0;
  }

  function reflectItem() {
    var item = currentItem;
    volume_reflectance_gaussian_width_spinbox.value = item[Nanairo.volumeReflectanceGaussianWidth];
  }

  function setCurrentItem(item, itemChangeMode) {
    currentItem = item;
    isItemChangeMode = itemChangeMode;
  }

  function setCurrentItemValue(key, value) {
    if (currentItem != null) {
      currentItem[key] = value;
    }
  }

  function setValue(width) {
    volume_reflectance_gaussian_width_spinbox.value = width;
  }

  function getItemData(item) {
    var itemData = {};
    itemData[Nanairo.volumeReflectanceGaussianWidth] = item[Nanairo.volumeReflectanceGaussianWidth];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.volumeReflectanceGaussianWidth] = itemData[Nanairo.volumeReflectanceGaussianWidth];
    return item;
  }

}
