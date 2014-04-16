/*!
  \file NEmitterModelTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import "EmitterItems"
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

Rectangle {
  id: emitter_model_tab_item
  width: 640
  height: 274

  readonly property int marginSize: 2

  property int removedEmitterIndex: -1
  property var currentEmitterItem: null
  property bool itemChangeMode: false

  Component {
    id: combobox_style
    NComboBoxStyle {
      backgroundColor: emitter_model_tab_item.color
    }
  }

  ComboBox {
    id: emitter_model_combobox
    enabled: currentEmitterItem != null
    x: 8
    y: 8
    width: 150
    height: 20
    style: combobox_style

    model: [Keyword.nonDirectionalEmitter]

    onCurrentIndexChanged: {
      if (currentIndex != -1 && currentEmitterItem != null) {
        currentEmitterItem.type = currentText
        if (!itemChangeMode)
          setDefaultEmitter(currentEmitterItem, currentText)
        else
          itemChangeMode = false
        resetEmitterItem(currentEmitterItem, currentText)
      }
    }
  }

  TextField {
    id: emitter_model_name_editor
    enabled: currentEmitterItem != null
    x: 176
    y: 8
    width: 170
    height: 20
    text: ""
    placeholderText: qsTr("Emitter model name")
    style: NTextFieldStyle{}

    onEditingFinished: {
      if (currentEmitterItem != null) {
        currentEmitterItem.name = displayText
        currentEmitterItem.text = displayText
      }
    }
  }

  TabView {
    id: emitter_model_tab_view
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
      title: qsTr(Keyword.nonDirectionalEmitter)
      active: true
      NNonDirectionalEmitterItem {
        color: emitter_model_tab_item.color
      }
    }

    currentIndex: emitter_model_combobox.currentIndex
  }

  Component {
    id: emitter_model_delegate
    Rectangle {
      enabled: true
      visible: true
      width: emitter_model_list.width - (2 * emitter_model_tab_item.marginSize)
      height: 16
      color: ListView.isCurrentItem
          ? Qt.darker(emitter_model_tab_item.color, 1.3)
          : (model.index % 2 == 0)
              ? emitter_model_tab_item.color
              : Qt.darker(emitter_model_tab_item.color, 1.05)
      
      Label {
        anchors.fill: parent
        text: model.name
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        textFormat: Text.PlainText

        MouseArea {
          anchors.fill: parent
          onClicked: emitter_model_list.currentIndex = model.index
        }
      }
    }
  }

  Rectangle {
    id: emitter_model_view
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
      id: emitter_model_list
      anchors.fill: parent

      clip: true
      delegate: emitter_model_delegate
      model: emitter_model

      leftMargin: emitter_model_tab_item.marginSize
      rightMargin: emitter_model_tab_item.marginSize
      topMargin: emitter_model_tab_item.marginSize
      bottomMargin: emitter_model_tab_item.marginSize

      onCurrentIndexChanged: {
        emitter_model_tab_item.currentEmitterItem = emitter_model.get(currentIndex)
      }
    }
  }

  ListModel {
    id: emitter_model
  }

  ListModel {
    id: default_emitter_model
    ListElement {
      name: "Emitter model"
      text: "Emitter model"
    }
  }

  Component {
    id: emitter_model_button_style
    NImageButtonStyle {
      backgroundColor: emitter_model_tab_item.color
    }
  }

  Button {
    id: add_emitter_model_button
    x: 482
    y: 223
    width: 24
    height: 24
    style: emitter_model_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 134
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Add emitter model")
    iconSource: enabled ? "image/add_emitter_model.png"
                        : "image/add_emitter_model_disabled.png"

    onClicked: addEmitterModel()
  }

  Button {
    id: delete_emitter_model_button
    x: 512
    y: 223
    width: 24
    height: 24
    style: emitter_model_button_style

    text: qsTr("")
    anchors.right: parent.right
    anchors.rightMargin: 104
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 27
    iconName: qsTr("Delete emitter model")
    iconSource: enabled ? "image/delete_emitter_model.png"
                        : "image/delete_emitter_model_disabled.png"

    onClicked: removeEmitterModel()
  }

  Component.onCompleted: {
    var item = default_emitter_model.get(0)
    item.type = Keyword.nonDirectionalEmitter
    item.radiantExitance = 128
  }

  onCurrentEmitterItemChanged: {
    if (currentEmitterItem != null) {
      emitter_model_name_editor.text = currentEmitterItem.name
      if (emitter_model_combobox.currentText == currentEmitterItem.type) {
        resetEmitterItem(currentEmitterItem, currentEmitterItem.type)
      }
      else {
        itemChangeMode = true
        emitter_model_combobox.currentIndex = getIndex(currentEmitterItem.type)
      }
    }
  }

  function addEmitterModel() {
    emitter_model.append(default_emitter_model.get(0))
    var item = emitter_model.get(emitter_model.count - 1)
    item.lightColor = [true, Qt.rgba(1, 1, 1, 1), Qt.resolvedUrl("")]
  }

  function removeEmitterModel() {
    var index = emitter_model_list.currentIndex
    var count = emitter_model_list.count
    if (index != -1 && count > 1) {
      emitter_model.remove(index)
      count = count - 1
      emitter_model_list.currentIndex = -1
      emitter_model_list.currentIndex = (index >= count) ? count - 1 : index
      removedEmitterIndex = index
      removedEmitterIndex = -1
    }
  }

  function getIndex(type) {
    return emitter_model_combobox.find(type)
  }

  function getEmitterModel() {
    return emitter_model
  }

  function getEmitterItem(index) {
    return emitter_model.get(index)
  }

  function getEmitterModelTabItem(index) {
    return (index == -1) ? null : emitter_model_tab_view.getTab(index).item
  }

  function initialize() {
    addEmitterModel()
  }

  function setDefaultEmitter(emitterItem, type) {
    var tabItem = getEmitterModelTabItem(getIndex(type))
    tabItem.setDefaultEmitter(emitterItem)
  }

  function resetEmitterItem(emitterItem, type) {
    var count = emitter_model_tab_view.count
    for (var i = 0; i < count; ++i) {
      var tabItem = getEmitterModelTabItem(i)
      tabItem.currentEmitterItem = null
    }
    var tabItem = getEmitterModelTabItem(getIndex(type))
    tabItem.currentEmitterItem = emitterItem
  }

  function validateData(messageList) {
    var count = emitter_model.count
    for (var i = 0; i < count; ++i) {
      var item = getEmitterItem(i)
      switch (item.type) {
       case Keyword.nonDirectionalEmitter:
        if (!item.lightColor[0] && item.lightColor[2].toString() == "") {
          var text = "[EmitterModel: " + item.name + "] Spectra file path is empty."
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
    emitter_model.clear()
    var prefix = Keyword.emitterModel + "/"
    var count = scene_settings.intValue(prefix + Keyword.count, 1)
    for (var i = 0; i < count; ++i) {
      addEmitterModel()
      var p = prefix + i + "/"
      var item = getEmitterItem(i)
      item.name = scene_settings.stringValue(p + Keyword.name, "EmitterModel")
      item.text = item.name
      item.type = scene_settings.stringValue(p + Keyword.type, 
                                             Keyword.nonDirectionalEmitter)
      var tabItem = getEmitterModelTabItem(getIndex(item.type))
      tabItem.loadSceneSettings(item, p)
    }
    emitter_model_list.currentIndex = -1
    emitter_model_list.currentIndex = 0
  }

  function saveSceneSettings() {
    var prefix = Keyword.emitterModel + "/"
    var count = emitter_model.count
    scene_settings.setIntValue(prefix + Keyword.count, count)
    for (var i = 0; i < count; ++i) {
      var p = prefix + i + "/"
      var item = getEmitterItem(i)
      scene_settings.setStringValue(p + Keyword.name, item.name)
      scene_settings.setStringValue(p + Keyword.type, item.type)
      var tabItem = getEmitterModelTabItem(getIndex(item.type))
      tabItem.saveSceneSettings(item, p)
    }
  }
}
