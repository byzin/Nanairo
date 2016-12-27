/*!
  \file NComplexRefractiveIndexSettingBlock.qml
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

NRefractiveIndexSettingBlock {
  id: complex_refractive_index_setting_block

  property string extinctionKey: Nanairo.value
  labelText: qsTr("ComplexRefractiveIndex")

  NSpectraFileView {
    id: extinction_file_view

    anchors.horizontalCenter: complex_refractive_index_setting_block.horizontalCenter
    y: (refractiveIndexFileView.y + refractiveIndexFileView.height) + Nanairo.itemOffset
    width: 130
    height: Nanairo.defaultButtonHeight
    color: complex_refractive_index_setting_block.color

    filePathKey: extinctionKey
    buttonTitle: qsTr("Open extinction")
  }

  function initializeItem(item) {
    refractiveIndexFileView.initializeItem(item);
    extinction_file_view.initializeItem(item);
  }

  function reflectItem() {
    refractiveIndexFileView.reflectItem();
    extinction_file_view.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    refractiveIndexFileView.setCurrentItem(item, itemChangeMode);
    extinction_file_view.setCurrentItem(item, itemChangeMode);
  }

  function getItemData(item) {
    var dataList = [];
    dataList.push(refractiveIndexFileView.getItemData(item));
    dataList.push(extinction_file_view.getItemData(item));
    return Nanairo.mergeDataList(dataList);
  }

  function makeItem(itemData) {
    var dataList = [];
    dataList.push(refractiveIndexFileView.makeItem(itemData));
    dataList.push(extinction_file_view.makeItem(itemData));
    return Nanairo.mergeDataList(dataList);
  }
}
