/*!
  \file NSurfaceModelTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "SurfaceItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: surface_model_tab_item
  width: 640
  height: 274

  readonly property int marginSize: 2

  property int removedSurfaceIndex: -1
  property ListModel textureModel: null
  property var currentSurfaceItem: null
  property bool itemChangeMode: false

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: surface_model_tab_item.color
    }
  }

  ComboBox {
    id: surface_model_combobox
    enabled: currentSurfaceItem != null
    x: 8
    y: 8
    width: 150
    height: 20
    style: combobox_style

    model: [Keyword.smoothDiffuseSurface,
            Keyword.smoothDielectricSurface,
            Keyword.smoothConductorSurface,
            Keyword.roughDielectricSurface,
            Keyword.roughConductorSurface,
            Keyword.roughPlasticSurface]

    onCurrentIndexChanged: {
      if (currentIndex != -1 && currentSurfaceItem != null) {
        currentSurfaceItem.type = currentText
        if (!itemChangeMode)
          setDefaultSurface(currentSurfaceItem, currentText)
        else
          itemChangeMode = false
        resetSurfaceItem(currentSurfaceItem, currentText)
      }
    }
  }

  TextField {
    id: surface_model_name_editor
    enabled: currentSurfaceItem != null
    x: 176
    y: 8
    width: 170
    height: 20
    text: ""
    placeholderText: qsTr("SurfaceModel name")
    style: NTextFieldStyle{}

    onEditingFinished: {
      if (currentSurfaceItem != null) {
        currentSurfaceItem.name = displayText
        currentSurfaceItem.text = displayText
      }
    }
  }

  TabView {
    id: surface_model_tab_view
    x: 0
    y: 34
    width: 476
    height: 240
    style: TabViewStyle {
      tab: Rectangle {
        implicitWidth: 0
        implicitHeight: 0
      }
    }

    Tab {
      title: qsTr(Keyword.smoothDiffuseSurface)
      active: true
      NSmoothDiffuseSurfaceItem {
        color: surface_model_tab_item.color
        textureModel: surface_model_tab_item.textureModel
      }
    }

    Tab {
      title: qsTr(Keyword.smoothDielectricSurface)
      active: true
      NSmoothDielectricSurfaceItem {
        color: surface_model_tab_item.color
      }
    }

    Tab {
      title: qsTr(Keyword.smoothConductorSurface)
      active: true
      NSmoothConductorSurfaceItem {
        color: surface_model_tab_item.color
      }
    }

    Tab {
      title: qsTr(Keyword.roughDielectricSurface)
      active: true
      NRoughDielectricSurfaceItem {
        color: surface_model_tab_item.color
        textureModel: surface_model_tab_item.textureModel
      }
    }

    Tab {
      title: qsTr(Keyword.roughConductorSurface)
      active: true
      NRoughConductorSurfaceItem {
        color: surface_model_tab_item.color
        textureModel: surface_model_tab_item.textureModel
      }
    }

    Tab {
      title: qsTr(Keyword.roughPlasticSurface)
      active: true
      NRoughPlasticSurfaceItem {
        color: surface_model_tab_item.color
        textureModel: surface_model_tab_item.textureModel
      }
    }

    currentIndex: surface_model_combobox.currentIndex
  }

  Component {
    id: surface_model_delegate
    Rectangle {
      enabled: true
      visible: true
      width: surface_model_list.width - (2 * surface_model_tab_item.marginSize)
      height: 16
      color: ListView.isCurrentItem
          ? Qt.darker(surface_model_tab_item.color, 1.3)
          : (model.index % 2 == 0)
              ? surface_model_tab_item.color
              : Qt.darker(surface_model_tab_item.color, 1.05)
      
      Label {
        anchors.fill: parent
        text: model.name
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.PlainText

        MouseArea {
          anchors.fill: parent
          onClicked: surface_model_list.currentIndex = model.index
        }
      }
    }
  }

  Rectangle {
    id: surface_model_view
    x: 482
    width: 150
    color: parent.color
    anchors.right: parent.right
    anchors.rightMargin: 8
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 57
    anchors.top: parent.top
    anchors.topMargin: 8
    border.color: Qt.darker(color, 1.2)
    border.width: 1

    ListView {
      id: surface_model_list
      anchors.fill: parent

      clip: true
      delegate: surface_model_delegate
      model: surface_model

      leftMargin: surface_model_tab_item.marginSize
      rightMargin: surface_model_tab_item.marginSize
      topMargin: surface_model_tab_item.marginSize
      bottomMargin: surface_model_tab_item.marginSize

      onCurrentIndexChanged: {
        surface_model_tab_item.currentSurfaceItem = surface_model.get(currentIndex)
      }
    }
  }

  ListModel {
    id: surface_model
  }

  ListModel {
    id: default_surface_model
    ListElement {
      name: "SurfaceModel"
      text: "SurfaceModel"
    }
  }

  Component {
    id: surface_model_button_style
    NImageButtonStyle {
      backgroundColor: surface_model_tab_item.color
    }
  }

  Button {
    id: add_surface_model_button
    x: 482
    y: 223
    width: 24
    height: 24
    style: surface_model_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 134
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Add surface model")
    iconSource: enabled ? "image/add_surface_model.png"
                        : "image/add_surface_model_disabled.png"

    onClicked: addSurfaceModel()
  }

  Button {
    id: delete_surface_model_button
    x: 512
    y: 223
    width: 24
    height: 24
    style: surface_model_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 104
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Delete surface model")
    iconSource: enabled ? "image/delete_surface_model.png"
                        : "image/delete_surface_model_disabled.png"

    onClicked: removeSurfaceModel()
  }

  Component.onCompleted: {
    var item = default_surface_model.get(0)
    item.type = Keyword.smoothDiffuseSurface
    item.reflectanceIndex = 0
  }

  onCurrentSurfaceItemChanged: {
    if (currentSurfaceItem != null) {
      surface_model_name_editor.text = currentSurfaceItem.name
      if (surface_model_combobox.currentText == currentSurfaceItem.type) {
        resetSurfaceItem(currentSurfaceItem, currentSurfaceItem.type)
      }
      else {
        itemChangeMode = true
        surface_model_combobox.currentIndex = getIndex(currentSurfaceItem.type)
      }
    }
  }

  function addSurfaceModel() {
    surface_model.append(default_surface_model.get(0))
  }

  function removeSurfaceModel() {
    var index = surface_model_list.currentIndex
    var count = surface_model_list.count
    if (index != -1 && count > 1) {
      surface_model.remove(index)
      count = count - 1
      surface_model_list.currentIndex = -1
      surface_model_list.currentIndex = (index >= count) ? count - 1 : index
      removedSurfaceIndex = index
      removedSurfaceIndex = -1
    }
  }

  function getIndex(type) {
    return surface_model_combobox.find(type)
  }

  function getTextureIndex(index, removedIndex) {
    var i = index
    if (index == removedIndex)
      i = 0
    else if (index > removedIndex)
      i = index - 1
    return i
  }

  function getSurfaceModel() {
    return surface_model
  }

  function getSurfaceItem(index) {
    return surface_model.get(index)
  }

  function getSurfaceModelTabItem(index) {
    return (index == -1) ? null : surface_model_tab_view.getTab(index).item
  }
  
  function initialize() {
    addSurfaceModel()
  }

  function setDefaultSurface(surfaceItem, type) {
    var tabItem = getSurfaceModelTabItem(getIndex(type))
    tabItem.setDefaultSurface(surfaceItem)
  }

  function setRemovedTextureIndex(index) {
    var count = surface_model.count
    for (var i = 0; i < count; ++i) {
      var item = surface_model.get(i)
      if (item.type == Keyword.smoothDiffuseSurface) {
        item.reflectanceIndex = getTextureIndex(item.reflectanceIndex, index)
      }
      else if (item.type == Keyword.roughDielectricSurface ||
               item.type == Keyword.roughConductorSurface) {
        item.roughnessIndex = getTextureIndex(item.roughnessIndex, index)
      }
      else if (item.type == Keyword.roughPlasticSurface) {
        item.diffuseColorIndex = getTextureIndex(item.diffuseColorIndex, index)
        item.roughnessIndex = getTextureIndex(item.roughnessIndex, index)
      }
    }
  }

  function resetSurfaceItem(surfaceItem, type) {
    var count = surface_model_tab_view.count
    for (var i = 0; i < count; ++i) {
      var tabItem = getSurfaceModelTabItem(i)
      tabItem.currentSurfaceItem = null
    }
    var tabItem = getSurfaceModelTabItem(getIndex(type))
    tabItem.currentSurfaceItem = surfaceItem
  }

  function validateData(messageList) {
    var count = surface_model.count
    for (var i = 0; i < count; ++i) {
      var item = getSurfaceItem(i)
      switch (item.type) {
       case Keyword.smoothDielectricSurface:
       case Keyword.roughDielectricSurface:
       case Keyword.roughPlasticSurface:
        if (item.outerRefractiveIndex[2].toString() == "") {
          var text = "[SurfaceModel: " + item.name + "] Outer refractive index file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        if (item.innerRefractiveIndex[2].toString() == "") {
          var text = "[SurfaceModel: " + item.name + "] Inner refractive index file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        break
       case Keyword.smoothConductorSurface:
       case Keyword.roughConductorSurface:
        if (item.outerRefractiveIndex[2].toString() == "") {
          var text = "[SurfaceModel: " + item.name + "] Outer refractive index file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        if (item.innerRefractiveIndex[2].toString() == "") {
          var text = "[SurfaceModel: " + item.name + "] Inner refractive index file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        if (item.innerExtinction[2].toString() == "") {
          var text = "[SurfaceModel: " + item.name + "] Inner extinction file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        break
       default:
        break
      }
    }
  }

  function loadSceneSettings() {
    surface_model.clear()
    var prefix = Keyword.surfaceModel + "/"
    var count = scene_settings.intValue(prefix + Keyword.count, 1)
    for (var i = 0; i < count; ++i) {
      addSurfaceModel()
      var p = prefix + i + "/"
      var item = getSurfaceItem(i)
      item.name = scene_settings.stringValue(p + Keyword.name, "SurfaceModel")
      item.text = item.name
      item.type = scene_settings.stringValue(p + Keyword.type, 
                                             Keyword.smoothDiffuseSurface)

      var tabItem = getSurfaceModelTabItem(getIndex(item.type))
      tabItem.loadSceneSettings(item, p)
    }
    surface_model_list.currentIndex = -1
    surface_model_list.currentIndex = 0
  }

  function saveSceneSettings() {
    var prefix = Keyword.surfaceModel + "/"
    var count = surface_model.count
    scene_settings.setIntValue(prefix + Keyword.count, count)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var item = getSurfaceItem(i)
      scene_settings.setStringValue(p + Keyword.name, item.name)
      scene_settings.setStringValue(p + Keyword.type, item.type)
      var tabItem = getSurfaceModelTabItem(getIndex(item.type))
      tabItem.saveSceneSettings(item, p)
    }
  }
}
