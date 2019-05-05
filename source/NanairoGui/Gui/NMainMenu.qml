/*!
  \file NMainMenu.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.11
import "Items"
import "definitions.js" as Definitions

NToolBar {
  id: mainMenu

  Material.elevation: 0

  readonly property int buttonPositionY: 0
  property bool isRenderMode: false
  // File menu values
  readonly property int load: 0
  readonly property int save: load + 1
  // Render menu values
  readonly property int render: 0
  readonly property int preview: render + 1
  readonly property int stop: preview + 1

  RowLayout {
    id: layout

    anchors.fill: parent
    spacing: Definitions.defaultItemSpace

    NToolButton {
      id: fileButton

      Layout.fillHeight: true 
      enabled: !isRenderMode
      text: qsTr("File")

      onClicked: toggleMenu(fileMenu)

      NMenu {
        id: fileMenu

        y: fileButton.height

        NMenuItem {
          id: loadSceneItem

          enabled: !isRenderMode
          text: qsTr("Load scene")
          onTriggered: mainMenu.fileMenuSelected(mainMenu.load)

          Shortcut {
            enabled: !isRenderMode
            sequence: Definitions.ctrlKey + "+O"
            context: Qt.ApplicationShortcut
            onActivated: loadSceneItem.triggered()
          }
        }

        NMenuItem {
          id: saveSceneItem

          enabled: !isRenderMode
          text: qsTr("Save scene")
          onTriggered: mainMenu.fileMenuSelected(mainMenu.save)

          Shortcut {
            enabled: !isRenderMode
            sequence: Definitions.ctrlKey + "+S"
            context: Qt.ApplicationShortcut
            onActivated: saveSceneItem.triggered()
          }
        }
      }
    }

    NToolButton {
      id: sceneButton

      Layout.fillHeight: true 
      enabled: !isRenderMode
      text: qsTr("Scene")

      onClicked: toggleMenu(sceneMenu)

      NMenu {
        id: sceneMenu

        y: sceneButton.height

        Repeater {
          model: [Definitions.defaultScene,
                  Definitions.cornellBoxScene,
                  Definitions.jensenCornellBoxScene,
                  Definitions.causticsTestScene,
                  Definitions.dispersionTestScene,
                  Definitions.veachMisTestScene,
                  Definitions.veachBdptTestScene,
                  Definitions.smoothDiffuseTestScene,
                  Definitions.smoothDielectricTestScene,
                  Definitions.smoothConductorTestScene,
                  Definitions.roughDielectricTestScene,
                  Definitions.roughConductorTestScene,
                  Definitions.layeredDiffuseTestScene,
//                  Definitions.clothTestScene,
                  Definitions.colorCheckerSpectrumScene,
                  Definitions.colorCheckerSRgbScene,
                  Definitions.raytracingCamp4Scene,
                  Definitions.privateRoomScene
                  ]
          NMenuItem {
            text: modelData
            onTriggered: mainMenu.sceneMenuSelected(text)
          }
        }
      }
    }

    NToolButton {
      id: renderButton

      Layout.fillHeight: true 
      text: qsTr("Render")

      onClicked: toggleMenu(renderMenu)

      NMenu {
        id: renderMenu

        y: renderButton.height

        NMenuItem {
          id: renderItem

          enabled: !isRenderMode
          text: qsTr("Render")
          onTriggered: mainMenu.renderMenuSelected(mainMenu.render)

          Shortcut {
            enabled: !isRenderMode
            sequence: Definitions.ctrlKey + "+R"
            context: Qt.ApplicationShortcut
            onActivated: renderItem.triggered()
          }
        }

        NMenuItem {
          id: previewItem

          enabled: !isRenderMode
          text: qsTr("Preview")
          onTriggered: mainMenu.renderMenuSelected(mainMenu.preview)

          Shortcut {
            enabled: !isRenderMode
            sequence: Definitions.ctrlKey + "+P"
            context: Qt.ApplicationShortcut
            onActivated: previewItem.triggered()
          }
        }

        NMenuItem {
          id: stopItem

          enabled: isRenderMode
          text: qsTr("Stop")
          onTriggered: mainMenu.renderMenuSelected(mainMenu.stop)

          Shortcut {
            enabled: isRenderMode
            sequence: StandardKey.Cancel
            context: Qt.ApplicationShortcut
            onActivated: stopItem.triggered()
          }
        }
      }
    }

    NLabel {
      Layout.fillWidth: true
      Layout.fillHeight: true
    }
  }

  function toggleMenu(menu) {
    if (menu.visible)
      menu.close();
    else
      menu.open();
  }

  signal fileMenuSelected(int menu)

  signal sceneMenuSelected(string menu)

  signal renderMenuSelected(int menu)
}
