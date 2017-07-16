/*!
  \file NObjectTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "Styles"
import "TabObjectItems"
import "nanairo.js" as Nanairo

Rectangle {
  id: object_tab_item
  width: Nanairo.tabItemWidth
  height: Nanairo.tabItemHeight

  property var emitterModel: null
  property var surfaceModel: null
  property alias currentObjectItem: object_tree_view.currentObjectItem

  NObjectSettingView {
    id: object_setting_view
    x: 0
    y: 0
    color: object_tab_item.color

    emitterModel: object_tab_item.emitterModel
    surfaceModel: object_tab_item.surfaceModel
  }

  NObjectTreeView {
    id: object_tree_view

    x: (object_setting_view.x + object_setting_view.width)
    y: 0
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    color: object_tab_item.color
    addObjectButton.onClicked: addObjectItem()
    addGroupButton.onClicked: addGroupItem()
  }

  onCurrentObjectItemChanged: {
    setCurrentItem(currentObjectItem, true);
    if (currentObjectItem != null) {
      reflectItem();
    }
    setCurrentItem(currentObjectItem, false);
  }

  function initializeItem(item) {
    object_setting_view.initializeItem(item);
  }

  function reflectItem() {
    object_setting_view.reflectItem();
  }

  function setCurrentItem(item, itemChangeMode) {
    object_setting_view.setCurrentItem(item, itemChangeMode);
  }

  function addObjectItem() {
    var objectModel = object_tree_view.objectModel;
    var item = objectModel.createObjectElement();
    initializeItem(item);
    objectModel.addElement(item);
    objectModel.moveObjects(objectModel.model.count - 1, 1, 1);
  }

  function addGroupItem() {
    var objectModel = object_tree_view.objectModel;
    var item = objectModel.createGroupElement();
    initializeItem(item);
    objectModel.addElement(item);
    objectModel.moveObjects(objectModel.model.count - 1, 1, 1);
  }

  function getItemData(index) {
    var objectModel = object_tree_view.objectModel;
    var item = objectModel.model.get(index);
    var itemData = object_setting_view.getItemData(item);
    itemData[Nanairo.name] = item["name"];
    itemData[Nanairo.type] = item["type"];
    itemData[Nanairo.groupLevel] = item["groupLevel"];
    itemData[Nanairo.enabled] = item["enabled"];
    return itemData;
  }

  function makeItem(itemData) {
    var item = object_setting_view.makeItem(itemData);
    item["name"] = itemData[Nanairo.name];
    item["type"] = itemData[Nanairo.type];
    item["groupLevel"] = itemData[Nanairo.groupLevel];
    item["enabled"] = itemData[Nanairo.enabled];
    item["expanded"] = false;
    item["visible"] = true;
    return item;
  }

  function getSceneData() {
    var objectModel = object_tree_view.objectModel;
    var sceneData = [];
    for (var i = 0; i < objectModel.model.count; ++i) {
      sceneData.push(getItemData(i));
    }
    return sceneData;
  }

  function setSceneData(sceneData) {
    var objectModel = object_tree_view.objectModel;
    objectModel.clear();
    for (var i = 0; i < sceneData.length; ++i) {
      var itemData = sceneData[i];
      var item = makeItem(itemData);
      objectModel.model.append(item);
    }
    objectModel.updateItemVisible();
  }
}
