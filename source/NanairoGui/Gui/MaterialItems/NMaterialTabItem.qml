/*!
  \file NMaterialTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../SettingBlockItems"
import "../nanairo.js" as Nanairo

Rectangle {
  id: material_tab_item

  property alias materialModel: material_list_view.materialModel 
  property alias currentItem: material_list_view.currentMaterialItem
  property alias currentMaterialTypeIndex: material_type_combobox.currentIndex
  property string materialName: qsTr("Material")
  property var materialTypeList: null
  property var materialComponent: null
  property string addMaterialButtonImage
  property string addMaterialButtonDisabledImage
  property string deleteMaterialButtonImage
  property string deleteMaterialButtonDisabledImage

  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  NSettingBlock {
    id: material_name_setting_block
    x: Nanairo.blockOffset
    y: Nanairo.blockOffset
    color: material_tab_item.color
    labelText: materialName + qsTr("Name")
    NTextField {
      id: material_name_editor
      y: material_name_setting_block.labelHeight + Nanairo.itemOffset
      placeholderText: qsTr("Name")
      onEditingFinished: setCurrentItemValue("name", text)
    }
  }

  NSettingBlock {
    id: material_type_setting_block
    x: material_name_setting_block.x
    y: (material_name_setting_block.y + material_name_setting_block.height) + Nanairo.blockOffset
    color: material_tab_item.color
    labelText: materialName + qsTr("Type")
    NComboBox {
      id: material_type_combobox
      y: material_type_setting_block.labelHeight + Nanairo.itemOffset
      backgroundColor: material_type_setting_block.color

      model: materialTypeList
      onCurrentIndexChanged: setCurrentItemValue(Nanairo.type, currentText)
    }
  }

  Rectangle {
    id: material_item_setting_view
    x: (material_name_setting_block.x + material_name_setting_block.width) + Nanairo.blockOffset
    y: 0
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    color: material_tab_item.color

    Loader {
      id: setting_view_loader
      anchors.fill: parent
      sourceComponent: materialComponent
    }
  }

  NMaterialListView {
    id: material_list_view

    anchors.top: parent.top
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    color: material_tab_item.color
  
    addMaterialButton.iconSource: (addMaterialButton.enabled)
        ? addMaterialButtonImage
        : addMaterialButtonDisabledImage
    deleteMaterialButton.iconSource: (deleteMaterialButton.enabled)
        ? deleteMaterialButtonImage
        : deleteMaterialButtonDisabledImage
  
    addMaterialButton.onClicked: addTextureItem()
  }

  onCurrentItemChanged: {
    setCurrentItem(currentItem, true);
    if (currentItem != null) {
      reflectItem();
    }
    setCurrentItem(currentItem, false);
  }

  function addTextureItem() {
    var item = materialModel.createGeneralElement();
    initializeItem(item);
    materialModel.addMaterialElement(item);
  }

  function initializeItem(item) {
    // Name
    item["name"] = materialName;
    // Type
    item[Nanairo.type] = material_type_combobox.textAt(0);
    // Property
    setting_view_loader.item.initializeItem(item, 0);
  }

  function reflectItem() {
    var item = currentItem;
    // Name
    material_name_editor.text = item["name"];
    // Type
    var typeIndex = material_type_combobox.find(item[Nanairo.type]);
    console.assert(typeIndex != -1, materialName, " type isn't found: ", item[Nanairo.type]);
    material_type_combobox.currentIndex = typeIndex;
    // Property
    setting_view_loader.item.reflectItem(typeIndex);
  }

  function setCurrentItem(item, itemChangeMode) {
    if (setting_view_loader.item != null) {
      setting_view_loader.item.setCurrentItem(item, itemChangeMode);
    }
  }

  function setCurrentItemValue(key, value) {
    if (currentItem != null) {
      currentItem[key] = value;
    }
  }

  function getItemData(index) {
    var item = materialModel.model.get(index);
    var itemData = setting_view_loader.item.getItemData(item);
    itemData[Nanairo.name] = item["name"];
    itemData[Nanairo.type] = item[Nanairo.type];
    return itemData;
  }

  function makeItem(itemData) {
    var item = setting_view_loader.item.makeItem(itemData);
    item["name"] = itemData[Nanairo.name];
    item[Nanairo.type] = itemData[Nanairo.type];
    return item;
  }

  function getSceneData() {
    var sceneData = [];
    for (var i = 0; i < materialModel.model.count; ++i) {
      sceneData.push(getItemData(i));
    }
    return sceneData;
  }

  function setSceneData(sceneData) {
    materialModel.model.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var itemData = sceneData[i];
      var item = makeItem(itemData);
      materialModel.model.append(item);
    }
  }
}
