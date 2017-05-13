/*!
  \file NMainWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Layouts 1.3
import "SettingBlockItems"
import "nanairo.js" as Nanairo

ApplicationWindow {
  id: main_window

  readonly property string ctrlKey: Nanairo.ctrlKey
  readonly property string nanairoFileFormat: Nanairo.nanairoFileFormat
  property bool isRenderingMode: false

  width: Nanairo.mainWindowWidth
  height: Nanairo.mainWindowHeight
  minimumWidth: width
  minimumHeight: height
  color: Nanairo.backgroundColor
  visible: true
  title: Nanairo.title

  menuBar: MenuBar {
    id: menu_bar

    Menu {
      title: qsTr("File")

      MenuItem {
        enabled: !isRenderingMode
        text: qsTr("Load scene")
        shortcut: ctrlKey + "+L"
        onTriggered: loading_scene_dialog.openDialog()
      }

      MenuItem {
        enabled: !isRenderingMode
        text: qsTr("Save scene")
        shortcut: ctrlKey + "+S"
        onTriggered: saving_scene_dialog.openDialog()
      }

      MenuSeparator {}
      MenuItem {
        enabled: !isRenderingMode
        text: qsTr("Quit")
        shortcut: ctrlKey + "+Q"
        onTriggered: Qt.quit()
      }
    }

    Menu {
      title: qsTr("Scene")

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.defaultScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.cornellBoxScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.jensenCornellBoxScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.causticsTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.dispersionTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.veachMisTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.veachBdptTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.materialTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.smoothDiffuseTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.smoothDielectricTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.smoothConductorTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.roughDielectricTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.roughConductorTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.layeredDiffuseTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.clothTestScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.colorCheckerSpectrumScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.colorCheckerSRgbScene
        onTriggered: loadPresetSceneData(text)
      }

      MenuItem {
        enabled: !isRenderingMode
        text: Nanairo.raytracingCamp4Scene
        onTriggered: loadPresetSceneData(text)
      }
    }

    Menu {
      title: qsTr("Render")

      MenuItem {
        enabled: !isRenderingMode
        text: qsTr("Render")
        shortcut: ctrlKey + "+R"
        onTriggered: {
          isRenderingMode = true;
          invokeRendering();
        }

      }
      MenuItem {
        enabled: !isRenderingMode
        text: qsTr("Preview")
        shortcut: ctrlKey + "+P"
        onTriggered: {
          isRenderingMode = true;
          invokePreviewing()
        }
      }

      MenuItem {
        enabled: isRenderingMode
        text: qsTr("Stop")
        shortcut: ctrlKey + "+F"
        onTriggered: nanairoManager.stopping()
      }
    }

    Menu {
      title: qsTr("Layout")

      MenuItem {
        text: qsTr("Default layout")
        shortcut: ctrlKey + "+B"
        onTriggered: setDefaultLayout()
      }
    }
  }

  Connections {
    target: nanairoManager
    onFinished: isRenderingMode = false
  }

  NSceneSettingView {
    id: scene_setting_view
    anchors.fill: parent
    enabled: !isRenderingMode
    color: main_window.color
  }

  NRenderedImageWindow {
    id: rendered_image_window
    color: main_window.color
  }

  NFileDialog {
    id: loading_scene_dialog
    title: "Load scene file"
    nameFilters: [qsTr("Scene setting file") + " (*." + nanairoFileFormat + ")"]
    selectExisting: true 
    onAccepted: loadSceneData(fileUrl)
  }

  NFileDialog {
    id: saving_scene_dialog
    title: "Save scene file"
    nameFilters: [qsTr("Scene setting file") + " (*." + nanairoFileFormat + ")"]
    selectExisting: false
    onAccepted: saveSceneData(fileUrl)
  }

  Component.onCompleted: {
    setDefaultLayout()
    if (!nanairoManager.isDebugMode()) {
      loadPresetSceneData(Nanairo.defaultScene)
    }
  }

  function invokeRendering() {
    // Make output dir
    var sceneName = scene_setting_view.getSceneName();
    var outputDir = sceneName + "_" + getCurrentTimeText()
    nanairoManager.makeDir(outputDir);
    // Backup the scene data
    var sceneFilePath = outputDir + "/" + Nanairo.sceneBackupFileName;
    saveSceneData(nanairoManager.toAbsoluteFileUrl(sceneFilePath));
    // Initialize rendered image dialog
    rendered_image_window.title = scene_setting_view.getSceneName();
    var imageResolution = scene_setting_view.getImageResolution();
    rendered_image_window.imageWidth = imageResolution[0];
    rendered_image_window.imageHeight = imageResolution[1];
    // 
    rendered_image_window.isPreviewMode = false;
    rendered_image_window.show();
    nanairoManager.invokeRendering(outputDir);
  }

  function invokePreviewing() {
    // Make output dir
    var outputDir = Nanairo.previewDir;
    nanairoManager.makeDir(outputDir);
    // Backup the scene data
    var sceneFilePath = outputDir + "/" + Nanairo.sceneBackupFileName;
    saveSceneData(nanairoManager.toAbsoluteFileUrl(sceneFilePath));
    // Initialize rendered image dialog
    rendered_image_window.title = scene_setting_view.getSceneName();
    var imageResolution = scene_setting_view.getImageResolution();
    rendered_image_window.imageWidth = imageResolution[0];
    rendered_image_window.imageHeight = imageResolution[1];
    // 
    rendered_image_window.isPreviewMode = true;
    rendered_image_window.show();
    nanairoManager.invokePreviewing(outputDir);
  }

  function getCurrentTimeText() {
    var timeText = new Date().toLocaleString(Qt.locale(), "yyyy_MM_dd_HH_mm_ss");
    return timeText;
  }

  function getSceneData() {
    var sceneData = scene_setting_view.getSceneData();
    return sceneData;
  }

  function setSceneData(sceneData) {
    scene_setting_view.setSceneData(sceneData);
  }

  function loadSceneData(filePath) {
    var sceneData = nanairoManager.loadSceneData(filePath);
    setSceneData(sceneData);
  }

  function loadPresetSceneData(sceneName) {
    var filePath = Qt.resolvedUrl("scene/" + sceneName + "." + nanairoFileFormat);
    loadSceneData(filePath);
  }

  function saveSceneData(filePath) {
    var sceneData = getSceneData();
    nanairoManager.saveSceneData(filePath, sceneData);
  }

  function setDefaultLayout() {
    main_window.width = Nanairo.mainWindowWidth
    main_window.height = Nanairo.mainWindowHeight 
  }
}
