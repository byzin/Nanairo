/*!
  \file NIsotropicScatteringCoefficientSettingBlock.qml
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
  id: isotropic_scattering_coefficient_setting_block_item

  property var currentItem: null
  property bool isItemChangeMode: false

  labelText: qsTr("Isotropic coefficient")

  NSpinBox {
    id: isotropic_scattering_coefficient_spinbox

    y: isotropic_scattering_coefficient_setting_block_item.labelHeight + Nanairo.itemOffset

    maximumValue: 1.0
    minimumValue: 0.0000001

    onValueChanged: setCurrentItemValue(Nanairo.isotropicScatteringCoefficient, value)
  }

  function initializeItem(item) {
    item[Nanairo.isotropicScatteringCoefficient] = 1.0;
  }

  function reflectItem() {
    var item = currentItem;
    isotropic_scattering_coefficient_spinbox.value = item[Nanairo.isotropicScatteringCoefficient];
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

  function setValue(coefficient) {
    isotropic_scattering_coefficient_spinbox.value = coefficient;
  }

  function getItemData(item) {
    var itemData = {};
    itemData[Nanairo.isotropicScatteringCoefficient] = item[Nanairo.isotropicScatteringCoefficient];
    return itemData;
  }

  function makeItem(itemData) {
    var item = {};
    item[Nanairo.isotropicScatteringCoefficient] = itemData[Nanairo.isotropicScatteringCoefficient];
    return item;
  }

}
