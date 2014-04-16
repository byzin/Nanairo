/*!
  \file NTextureTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "TextureItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: texture_tab_item
  width: 640
  height: 274

  readonly property alias textureModel: texture_model
  readonly property int marginSize: 2

  property bool itemChangeMode: false
  property int removedTextureIndex: -1
  property var currentTextureItem: null

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: texture_tab_item.color
    }
  }

  ComboBox {
    id: texture_combobox
    enabled: currentTextureItem != null
    x: 8
    y: 8
    width: 150
    height: 20
    style: combobox_style

    model: [Keyword.valueTexture,
            Keyword.unicolorTexture,
            Keyword.checkerboardTexture,
            Keyword.imageTexture]

    onCurrentIndexChanged: {
      if (currentIndex != -1 && currentTextureItem != null) {
        currentTextureItem.type = currentText
        if (!itemChangeMode)
          setDefaultTexture(currentTextureItem, currentText)
        else
          itemChangeMode = false
        resetTextureItem(currentTextureItem, currentText)
      }
    }
  }

  TextField {
    id: texture_name_editor
    enabled: currentTextureItem != null
    x: 176
    y: 8
    width: 170
    height: 20
    text: ""
    placeholderText: qsTr("Texture name")
    style: NTextFieldStyle{}

    onEditingFinished: {
      if (currentTextureItem != null) {
        currentTextureItem.name = displayText
        currentTextureItem.text = displayText
      }
    }
  }

  TabView {
    id: texture_tab_view
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
      title: qsTr(Keyword.valueTexture)
      active: true
      NValueTextureItem {
        color: texture_tab_item.color
      }
    }

    Tab {
      title: qsTr(Keyword.unicolorTexture)
      active: true
      NUnicolorTextureItem {
        color: texture_tab_item.color
      }
    }

    Tab {
      title: qsTr(Keyword.checkerboardTexture)
      active: true
      NCheckerboardTextureItem {
        color: texture_tab_item.color
      }
    }

    Tab {
      title: qsTr(Keyword.imageTexture)
      active: true
      NImageTextureItem {
        color: texture_tab_item.color
      }
    }

    currentIndex: texture_combobox.currentIndex
  }

  Component {
    id: texture_delegate
    Rectangle {
      enabled: true
      visible: true
      width: texture_list.width - (2 * texture_tab_item.marginSize)
      height: 16
      color: ListView.isCurrentItem
          ? Qt.darker(texture_tab_item.color, 1.3)
          : (model.index % 2 == 0)
              ? texture_tab_item.color
              : Qt.darker(texture_tab_item.color, 1.05)
      
      Label {
        anchors.fill: parent
        text: model.name
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.PlainText

        MouseArea {
          anchors.fill: parent
          onClicked: texture_list.currentIndex = model.index
        }
      }
    }
  }

  Rectangle {
    id: texture_view
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
      id: texture_list
      anchors.fill: parent

      clip: true
      delegate: texture_delegate
      model: texture_model

      leftMargin: texture_tab_item.marginSize
      rightMargin: texture_tab_item.marginSize
      topMargin: texture_tab_item.marginSize
      bottomMargin: texture_tab_item.marginSize

      onCurrentIndexChanged: {
        texture_tab_item.currentTextureItem = texture_model.get(currentIndex)
      }
    }
  }

  ListModel {
    id: texture_model
  }

  ListModel {
    id: default_texture
    ListElement {
      name: "Texture"
      text: "Texture"
    }
  }

  Component {
    id: texture_button_style
    NImageButtonStyle {
      backgroundColor: texture_tab_item.color
    }
  }

  Button {
    id: add_texture_button
    x: 482
    y: 223
    width: 24
    height: 24
    style: texture_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 134
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Add texture")
    iconSource: enabled ? "image/add_texture.png"
                        : "image/add_texture_disabled.png"

    onClicked: addTexture()
  }

  Button {
    id: delete_texture_button
    x: 512
    y: 223
    width: 24
    height: 24
    style: texture_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 104
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Delete texture")
    iconSource: enabled ? "image/delete_texture.png"
                        : "image/delete_texture_disabled.png"

    onClicked: removeTexture()
  }

  Component.onCompleted: {
    var item = default_texture.get(0)
    item.type = Keyword.valueTexture
    item.value = 0.8
  }

  onCurrentTextureItemChanged: {
    if (currentTextureItem != null) {
      texture_name_editor.text = currentTextureItem.name
      if (texture_combobox.currentText == currentTextureItem.type) {
        resetTextureItem(currentTextureItem, currentTextureItem.type)
      }
      else {
        itemChangeMode = true
        texture_combobox.currentIndex = getIndex(currentTextureItem.type)
      }
    }
  }

  function addTexture() {
    texture_model.append(default_texture.get(0))
  }

  function removeTexture() {
    var index = texture_list.currentIndex
    var count = texture_list.count
    if (index != -1 && count > 1) {
      texture_model.remove(index)
      count = count - 1
      texture_list.currentIndex = -1
      texture_list.currentIndex = (index >= count) ? count - 1 : index
      removedTextureIndex = index
      removedTextureIndex = -1
    }
  }

  function getIndex(type) {
    return texture_combobox.find(type)
  }

  function getTextureItem(index) {
    return texture_model.get(index)
  }

  function getTextureTabItem(index) {
    return (index == -1) ? null : texture_tab_view.getTab(index).item
  }

  function initialize() {
    addTexture()
  }

  function setDefaultTexture(textureItem, type) {
    var tabItem = getTextureTabItem(getIndex(type))
    tabItem.setDefaultTexture(textureItem)
  }

  function resetTextureItem(textureItem, type) {
    var count = texture_tab_view.count
    for (var i = 0; i < count; ++i) {
      var tabItem = getTextureTabItem(i)
      tabItem.currentTextureItem = null
    }
    var tabItem = getTextureTabItem(getIndex(type))
    tabItem.currentTextureItem = textureItem
  }

  function validateData(messageList) {
    var count = texture_model.count
    for (var i = 0; i < count; ++i) {
      var item = getTextureItem(i)
      switch (item.type) {
       case Keyword.unicolorTexture:
        if (!item.textureColor[0] && item.textureColor[2].toString() == "") {
          var text = "[Texture: " + item.name + "] Spectra file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        break
       case Keyword.checkerboardTexture:
        if ((!item.textureColor1[0] && item.textureColor1[2].toString() == "") ||
            (!item.textureColor2[0] && item.textureColor2[2].toString() == "")) {
          var text = "[Texture: " + item.name + "] Spectra file path is empty."
          var message = {"messageType" : Nanairo.errorMessageType, "text" : text}
          messageList.append(message)
        }
        break
       case Keyword.imageTexture:
        if (item.imageFilePath.toString() == "") {
          var text = "[Texture: " + item.name + "] Image file path is empty."
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
    texture_model.clear()
    var prefix = Keyword.texture + "/"
    var count = scene_settings.intValue(prefix + Keyword.count, 1)
    for (var i = 0; i < count; ++i) {
      addTexture()
      var p = prefix + i + "/"
      var item = getTextureItem(i)
      item.name = scene_settings.stringValue(p + Keyword.name, "Texture")
      item.text = item.name
      item.type = scene_settings.stringValue(p + Keyword.type, Keyword.valueTexture)
      var tabItem = getTextureTabItem(getIndex(item.type))
      tabItem.loadSceneSettings(item, p)
    }
    texture_list.currentIndex = -1
    texture_list.currentIndex = 0
  }

  function saveSceneSettings() {
    var prefix = Keyword.texture + "/"
    var count = texture_model.count
    scene_settings.setIntValue(prefix + Keyword.count, count)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var item = getTextureItem(i)
      scene_settings.setStringValue(p + Keyword.name, item.name)
      scene_settings.setStringValue(p + Keyword.type, item.type)
      var tabItem = getTextureTabItem(getIndex(item.type))
      tabItem.saveSceneSettings(item, p)
    }
  }
}
