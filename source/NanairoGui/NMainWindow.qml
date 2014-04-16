/*!
  \file NMainWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import "nanairo_definitions.js" as Nanairo
import "keyword.js" as Keyword

ApplicationWindow {
  id: main_window

  readonly property int defaultWidth: 960
  readonly property int defaultHeight: 540
  readonly property int defaultSplitWidht: 320
  readonly property int defaultSplitHeight: 240
  readonly property int splitMinimumSize: 5

  property bool renderingMode: false

  width: defaultWidth
  height: defaultHeight
  minimumWidth: 100
  minimumHeight: 100
  color: "#EBEBEB"
  visible: true
  title: "Nanairo"

  menuBar: MenuBar {
    Menu {
      title: qsTr("File")
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Load scene")
        shortcut: Keyword.ctrlKey + "+L"
        onTriggered: {
          setFileDialogAsLoadingMode()
          setting_file_dialog.open()
        }
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Save scene")
        shortcut: Keyword.ctrlKey + "+S"
        onTriggered: {
          setFileDialogAsSavingMode()
          setting_file_dialog.open()
        }
      }
      MenuSeparator {}
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Quit")
        shortcut: Keyword.ctrlKey + "+Q"
        onTriggered: Qt.quit()
      }
    }
    Menu {
      title: qsTr("Scene")
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Default")
        onTriggered: loadTestScene("DefaultScene")
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("CornellBox")
        onTriggered: loadTestScene(text)
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("JensenCornellBox")
        onTriggered: loadTestScene(text)
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("CausticsTest")
        onTriggered: loadTestScene(text)
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("VeachMisTest")
        onTriggered: loadTestScene(text)
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("MaterialTest")
        onTriggered: loadTestScene(text)
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("FitnessRoom")
        onTriggered: loadTestScene(text)
      }
    }
    Menu {
      title: qsTr("Render")
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Render")
        shortcut: Keyword.ctrlKey + "+R"
        onTriggered: render()
      }
      MenuItem {
        enabled: !renderingMode
        text: qsTr("Preview")
        shortcut: Keyword.ctrlKey + "+P"
        onTriggered: previewRendering()
      }
      MenuItem {
        enabled: renderingMode
        text: qsTr("Stop")
        shortcut: Keyword.ctrlKey + "+F"
        onTriggered: manager.stopRendering()
      }
    }
    Menu {
      title: qsTr("Layout")
      MenuItem {
        text: qsTr("Default layout")
        shortcut: Keyword.ctrlKey + "+B"
        onTriggered: setDefaultLayout()
      }
      MenuItem {
        text: qsTr("Setting layout")
        shortcut: Keyword.ctrlKey + "+N"
        onTriggered: setSettingLayout()
      }
      MenuItem {
        text: qsTr("Rendering layout")
        shortcut: Keyword.ctrlKey + "+M"
        onTriggered: setRenderingLayout()
      }
      MenuItem {
        text: qsTr("Edit layout")
        shortcut: Keyword.ctrlKey + "+,"
        onTriggered: setEditLayout()
      }
    }
  }

  SplitView {
    id: window_split1
    anchors.fill: parent

    Rectangle {
      id: left_area
      color: main_window.color

      Layout.minimumWidth: splitMinimumSize
      Layout.minimumHeight: splitMinimumSize

      SplitView {
        id: window_split2
        anchors.fill: parent
        orientation: Qt.Vertical
  
        Layout.minimumWidth: splitMinimumSize
        Layout.minimumHeight: splitMinimumSize

        NRenderedImageCanvas {
          id: rendered_image_canvas
          color: main_window.color

          Layout.minimumWidth: splitMinimumSize
          Layout.minimumHeight: splitMinimumSize

          Connections {
            target: manager
            onStarted: {
              rendered_image_canvas.enableImageCanvas(true)
              scene_settings.close()
            }
            onFinished: {
              rendered_image_canvas.enableImageCanvas(false)
              setRenderingMode(false)
            }
            onUpdated: {
              rendered_image_canvas.setRenderingInfo(fps, cycle, time)
            }
            onOutputMessage: {
              logMessage(message)
            }
          }

          Component.onCompleted: rendered_image_canvas.enableImageCanvas(false)
        }
  
        NLogView {
          id: log_view
          color: main_window.color
  
          Layout.minimumWidth: splitMinimumSize
          Layout.minimumHeight: splitMinimumSize
        }
      }
      onHeightChanged: {
        var diff = height - (rendered_image_canvas.height + log_view.height)
        if (diff < 0)
          rendered_image_canvas.height += diff
      }
    }

    Rectangle {
      id: right_area
      color: main_window.color

      Layout.minimumWidth: splitMinimumSize
      Layout.minimumHeight: splitMinimumSize

      SplitView {
        id: window_split3
        anchors.fill: parent
        orientation: Qt.Vertical
  
        Layout.minimumWidth: splitMinimumSize
        Layout.minimumHeight: splitMinimumSize
  
        NObjectView {
          id: object_view
          enabled: !renderingMode
          color: main_window.color
  
          Layout.minimumWidth: splitMinimumSize
          Layout.minimumHeight: splitMinimumSize
        }
  
        NSettingView {
          id: setting_view
          enabled: !renderingMode
          color: main_window.color
  
          Layout.minimumWidth: splitMinimumSize
          Layout.minimumHeight: splitMinimumSize

          onRemovedSurfaceIndexChanged: {
            if (removedSurfaceIndex != -1)
              object_view.setRemovedSurfaceIndex(removedSurfaceIndex)
          }

          onRemovedEmitterIndexChanged: {
            if (removedEmitterIndex != -1)
              object_view.setRemovedEmitterIndex(removedEmitterIndex)
          }
        }
      }

      onHeightChanged: {
        var diff = height - (object_view.height + setting_view.height)
        if (diff < 0)
          object_view.height += diff
      }
    }
  }

  FileDialog {
    id: setting_file_dialog
    folder: manager.currentWorkingDir()
    nameFilters: [qsTr("Scene setting file") + " (*." + Keyword.nanairoFileFormat + ")"]

    readonly property int savingMode: 0
    readonly property int loadingMode: 1
    property int setting_mode: 0

    onAccepted: {
      var filePath = setting_file_dialog.fileUrl
      var fileName = manager.getFileName(filePath)
      var regex = new RegExp(".*\." + Keyword.nanairoFileFormat)
      if (fileName.match(regex) == null) {
        var newFile = filePath.toString() + "." + Keyword.nanairoFileFormat
        filePath = Qt.resolvedUrl(newFile)
      }
      scene_settings.open(filePath)
      if (setting_mode == savingMode) {
        scene_settings.clear()
        saveSceneSettings()
        scene_settings.close()
      }
      else {
        loadSceneSettings()
        scene_settings.close()
      }
    }
  }

  Component {
    id: message_list
    ListModel {}
  }

  Component.onCompleted: {
    setDefaultLayout()
    initialize()
    loadTestScene("DefaultScene")
  }

  onWidthChanged: {
    var diff = width - (left_area.width + right_area.width)
    if (diff < 0)
      left_area.width += diff
  }

  function initialize() {
    setting_view.initialize()
    object_view.initialize()
    object_view.setMaterialModel(setting_view.getSurfaceModel(),
                                 setting_view.getEmitterModel())
  }

  function getCurrentTimeString() {
    var time_string = new Date().toLocaleString(Qt.locale(), "yyyy_MM_dd_HH_mm_ss")
    return time_string
  }

  function loadSceneSettings() {
    setting_view.loadSceneSettings()
    object_view.loadSceneSettings()
  }

  function saveSceneSettings() {
    setting_view.saveSceneSettings()
    object_view.saveSceneSettings()
  }

  function setFileDialogAsSavingMode() {
    setting_file_dialog.setting_mode = setting_file_dialog.savingMode
    setting_file_dialog.title = qsTr("Save scene settings")
    setting_file_dialog.selectExisting = false
  }

  function setFileDialogAsLoadingMode() {
    setting_file_dialog.setting_mode = setting_file_dialog.loadingMode
    setting_file_dialog.title = qsTr("Load scene settings")
    setting_file_dialog.selectExisting = true
  }

  function loadTestScene(sceneName) {
    var filePath = ":/scene/" + sceneName + ".nana"
    scene_settings.open(filePath)
    loadSceneSettings()
    scene_settings.close()
  }

  function setDefaultLayout() {
    main_window.width = defaultWidth
    main_window.height = defaultHeight
    left_area.width = defaultSplitWidht
    rendered_image_canvas.height = defaultSplitHeight
    object_view.height = defaultSplitHeight
  }

  function setSettingLayout() {
    main_window.width = defaultWidth + splitMinimumSize - defaultSplitWidht
    main_window.height = defaultHeight + splitMinimumSize
    left_area.width = splitMinimumSize
    rendered_image_canvas.height = defaultSplitHeight
    object_view.height = defaultSplitHeight
  }

  function setRenderingLayout() {
    var imageWidth = setting_view.getImageWidth()
    var imageHeight = setting_view.getImageHeight()
    main_window.width = imageWidth + splitMinimumSize
    main_window.height = imageHeight + splitMinimumSize 
    left_area.width = imageWidth
    rendered_image_canvas.height = imageHeight
  }

  function setEditLayout() {
    var imageWidth = setting_view.getImageWidth()
    var imageHeight = setting_view.getImageHeight()
    var settingWidth = defaultWidth - defaultSplitWidht
    var settingHeight = defaultHeight - defaultSplitHeight
    main_window.width = imageWidth + settingWidth
    main_window.height = imageHeight + settingHeight
    left_area.width = imageWidth
    rendered_image_canvas.height = imageHeight
    object_view.height = defaultSplitHeight
  }

  function previewRendering() {
    // Validate scene setting data
    if (!validateSceneData())
      return
    setRenderingMode(true)
    // Save scene setting data
    var filePath = manager.currentWorkingDir() + 
                   "/preview." + Keyword.nanairoFileFormat
    scene_settings.open(filePath)
    scene_settings.clear()
    saveSceneSettings()
    // Preview rendering
    manager.preview()
  }

  function render() {
    // Validate scene setting data
    if (!validateSceneData())
      return
    setRenderingMode(true)
    // Make output directory
    var dirName = setting_view.getSceneName() + "_" + getCurrentTimeString()
    manager.makeDir(dirName)
    // Save scene setting data
    var outputDir = manager.currentWorkingDir() + "/" + dirName
    var filePath = outputDir + "/settings." + Keyword.nanairoFileFormat
    scene_settings.open(filePath)
    scene_settings.clear()
    saveSceneSettings()
    // Render image
    manager.render(outputDir)
  }

  function validateSceneData() {
    var isValid = true
    var messageList = message_list.createObject(main_window)
    setting_view.validateData(messageList)
    object_view.validateData(messageList)
    var count = messageList.count
    if (count == 0)
      return isValid
    for (var i = 0; i < count; ++i) {
      var message = messageList.get(i)
      switch (message.messageType) {
       case Nanairo.warningMessageType:
        logMessage("Warning: " + message.text)
        break
       case Nanairo.errorMessageType:
        isValid = false
        logMessage("Error: " + message.text)
        break
       default:
        break
      }
    }
    return isValid
  }

  function logMessage(message) {
    log_view.appendLog(message)
  }

  function setRenderingMode(mode) {
    renderingMode = mode
  }
}
