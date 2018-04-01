/*!
  \file NMainWindow.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import Qt.labs.platform 1.0
import "Items"
import "definitions.js" as Definitions

ApplicationWindow {
  id: mainWindow

  property bool isRenderMode: false

  width: Definitions.mainWindowWidth
  height: Definitions.mainWindowHeight
  visible: true
  maximumWidth: width
  maximumHeight: height 
  minimumWidth: width
  minimumHeight: height
  title: Definitions.projectName

  header: NMainMenu {
    id: mainMenu

    isRenderMode: mainWindow.isRenderMode

    onFileMenuSelected: {
      sceneFileDialog.sceneDialogMode = menu;
      switch (menu) {
        case mainMenu.load: {
          sceneFileDialog.title = "Load scene.";
          sceneFileDialog.fileMode = FileDialog.OpenFile
          break;
        }
        case mainMenu.save: {
          sceneFileDialog.title = "Save scene.";
          sceneFileDialog.fileMode = FileDialog.SaveFile
          break;
        }
      }
      sceneFileDialog.open();
    }

    onSceneMenuSelected: {
      loadPresetScene(menu);
    }

    onRenderMenuSelected: {
      switch (menu) {
        case mainMenu.render: {
          mainWindow.isRenderMode = true;
          mainWindow.invokeRendering(false);
          break;
        }
        case mainMenu.preview: {
          mainWindow.isRenderMode = true;
          mainWindow.invokeRendering(true);
          break;
        }
        case mainMenu.stop: {
          renderWindow.close();
          break;
        }
      }
    }
  }

  NSceneSettingView {
    id: settingView

    enabled: !mainWindow.isRenderMode
    anchors.fill: parent
    currentIndex: settingTabBar.currentIndex
  }

  footer: NTabBar {
    id: settingTabBar

    enabled: !mainWindow.isRenderMode
    height: Definitions.defaultTabHeight
    Repeater {
      model: [qsTr("Tag"),
              qsTr("System"),
              qsTr("Color"),
              qsTr("Method"),
              qsTr("Texture"),
              qsTr("Surface"),
              qsTr("Emitter"),
              qsTr("Object"),
              qsTr("BVH")
              ]
      NTabButton {
        text: modelData
      }
    }
  }

  NRenderWindow {
    id: renderWindow
  }

  FileDialog {
    id: sceneFileDialog

    property int sceneDialogMode
    nameFilters: ["Nanairo scene file (*.nana)"]

    onAccepted: {
      switch (sceneDialogMode) {
        case mainMenu.load: {
          loadScene(file);
          break;
        }
        case mainMenu.save: {
          saveScene(file);
          break;
        }
      }
    }
  }

  Connections {
    target: nanairoManager
    onStarted: renderWindow.show()
    onFinished: isRenderMode = false
  }

  Component.onCompleted: loadPresetScene(Definitions.defaultScene)

  function invokeRendering(isPreviewing) {
    // Initialize the rendered image dialog
    renderWindow.title = settingView.getSceneName();
    if (isPreviewing)
      renderWindow.title = renderWindow.title + " (Preview)";
    var imageResolution = settingView.getImageResolution();
    renderWindow.initForRendering(imageResolution, isPreviewing);
    // Start rendering
    var sceneData = settingView.getSceneData();
    nanairoManager.invokeRendering(sceneData, isPreviewing);
  }

  function loadPresetScene(sceneName) {
    var sceneFile = "qrc:/NanairoGui/scene/" + sceneName + "." + Definitions.nanairoFileFormat;
    sceneFile = Qt.resolvedUrl(sceneFile);
    loadScene(sceneFile);
  }

  function loadScene(sceneFile) {
    var sceneData = nanairoManager.loadSceneData(sceneFile);
    settingView.setSceneData(sceneData);
  }

  function saveScene(sceneFile) {
    var sceneData = settingView.getSceneData();
    nanairoManager.saveSceneData(sceneFile, sceneData);
  }
}
