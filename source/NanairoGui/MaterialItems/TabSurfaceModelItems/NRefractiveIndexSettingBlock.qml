/*!
  \file NRefractiveIndexSettingBlock.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import ".."
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NSettingBlock {
  id: refractive_index_setting_block

  property string refractiveIndexKey: Nanairo.value
  readonly property alias refractiveIndexFileView: refractive_index_file_view

  labelText: qsTr("RefractiveIndex")

  NSpectraFileView {
    id: refractive_index_file_view

    anchors.horizontalCenter: refractive_index_setting_block.horizontalCenter
    y: refractive_index_setting_block.labelHeight + Nanairo.itemOffset
    width: 130
    height: Nanairo.defaultButtonHeight
    color: refractive_index_setting_block.color

    filePathKey: refractiveIndexKey
    buttonTitle: qsTr("Open refractive index")
  }

  function initializeItem(item) {
    refractive_index_file_view.initializeItem(item);
  }

  function reflectItem() {
    refractive_index_file_view.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    refractive_index_file_view.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    return refractive_index_file_view.getItemData(item);
  }

  function makeItem(itemData) {
    return refractive_index_file_view.makeItem(itemData);
  }
}
