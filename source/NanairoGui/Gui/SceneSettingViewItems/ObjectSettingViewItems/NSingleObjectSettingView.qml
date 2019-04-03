/*!
  \file NSingleObjectSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import Qt.labs.platform 1.0
import "../../Items"
import "../../definitions.js" as Definitions

NScrollView {
  id: objectSettingView

  property var objectItem: null
  readonly property bool isMeshObject: shapeType == Definitions.meshObject
  property var surfaceModelList: null
  property var emitterModelList: null
  property string shapeType: ""
  property string objectFilePath: ""
  property bool smoothing: false
  property int surfaceIndex: 0
  property bool isEmissiveObject: false
  property int emitterIndex: 0

  GridLayout {
    columns: 2
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "shape type" 
      color: objectSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: shapeTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: find(objectSettingView.shapeType)
          model: [Definitions.planeObject,
                  Definitions.meshObject]

          onCurrentTextChanged: objectSettingView.shapeType = currentText
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      enabled: objectSettingView.isMeshObject
      title: "mesh data" 
      color: objectSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NButton {
          id: objectFileButton

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          font.weight: Font.Light
          font.pixelSize: 10
          text: (objectSettingView.objectFilePath != "")
              ? nanairoManager.getFileName(objectSettingView.objectFilePath)
              : "object file"

          onClicked: objectFileDialog.open()
        }

        NCheckBox {
          id: smoothingCheckBox

          Layout.alignment: Qt.AlignLeft | Qt.AlignTop
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
    //      enabled: objectSettingView.isMeshObject
          enabled: false // \todo Implement smoothing
          checked: objectSettingView.smoothing
          text: "smoothing"

          onCheckedChanged: objectSettingView.smoothing = checked
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "surface" 
      color: objectSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: surfaceIndexComboBox

          Layout.alignment: Qt.AlignHcenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: objectSettingView.surfaceIndex
          model: objectSettingView.surfaceModelList
          textRole: Definitions.modelNameKey

          onCurrentIndexChanged: objectSettingView.surfaceIndex = currentIndex
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }

    NGroupBox {
      title: "emitter" 
      color: objectSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NCheckBox {
          id: emitterCheckBox

          Layout.alignment: Qt.AlignLeft | Qt.AlignTop
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          checked: objectSettingView.isEmissiveObject
          text: "is emissive"

          onCheckedChanged: objectSettingView.isEmissiveObject = checked
        }

        NComboBox {
          id: emitterIndexComboBox

          enabled: emitterCheckBox.checked
          Layout.alignment: Qt.AlignHcenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: objectSettingView.emitterIndex
          model: objectSettingView.emitterModelList
          textRole: Definitions.modelNameKey

          onCurrentIndexChanged: objectSettingView.emitterIndex = currentIndex
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color;
        }
      }
    }
  }

  FileDialog {
    id: objectFileDialog

    title: "Open mesh file"
    nameFilters: ["Wavefront (*.obj)"]

    onAccepted: objectSettingView.objectFilePath = nanairoManager.getRelativePath(file)
  }

  onIsMeshObjectChanged: {
    if (!isMeshObject) {
      objectFilePath = "";
      smoothing = false;
    }
  }

  onShapeTypeChanged: setProperty(Definitions.shapeType, shapeType)

  onObjectFilePathChanged: setProperty(Definitions.objectFilePath, objectFilePath)

  onSmoothingChanged: setProperty(Definitions.smoothing, smoothing)

  onSurfaceIndexChanged: setProperty(Definitions.surfaceIndex, surfaceIndex)

  onIsEmissiveObjectChanged: setProperty(Definitions.isEmissiveObject, isEmissiveObject)

  onEmitterIndexChanged: setProperty(Definitions.emitterIndex, emitterIndex)

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    // Set the properties
    item[Definitions.shapeType] = Definitions.planeObject;
    item[Definitions.objectFilePath] = "";
    item[Definitions.smoothing] = false;
    item[Definitions.surfaceIndex] = 0;
    item[Definitions.isEmissiveObject] = false;
    item[Definitions.emitterIndex] = 0;
  }

  function setProperty(propertyName, value) {
    if (objectItem != null)
      Definitions.setProperty(objectItem, propertyName, value);
  }

  function setItem(item) {
    objectItem = null;
    if (item != null) {
      shapeType = Definitions.getProperty(item, Definitions.shapeType);
      objectFilePath = Definitions.getProperty(item, Definitions.objectFilePath);
      smoothing = Definitions.getProperty(item, Definitions.smoothing);
      surfaceIndex = Definitions.getProperty(item, Definitions.surfaceIndex);
      isEmissiveObject = Definitions.getProperty(item, Definitions.isEmissiveObject);
      emitterIndex = Definitions.getProperty(item, Definitions.emitterIndex);
    }
    objectItem = item;
  }

  function getSceneData(item) {
    var sceneData = {};

    var shapeType = Definitions.getProperty(item, Definitions.shapeType);
    sceneData[Definitions.shapeType] = shapeType;
    if (shapeType == Definitions.meshObject) {
      sceneData[Definitions.objectFilePath] = 
          Definitions.getProperty(item, Definitions.objectFilePath);
      sceneData[Definitions.smoothing] = 
          Definitions.getProperty(item, Definitions.smoothing);
    }
    sceneData[Definitions.surfaceIndex] = 
        Definitions.getProperty(item, Definitions.surfaceIndex);
    sceneData[Definitions.isEmissiveObject] = 
        Definitions.getProperty(item, Definitions.isEmissiveObject);
    sceneData[Definitions.emitterIndex] = 
        Definitions.getProperty(item, Definitions.emitterIndex);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var shapeType = Definitions.getProperty(sceneData, Definitions.shapeType);
    item[Definitions.shapeType] = shapeType;
    if (shapeType == Definitions.meshObject) {
      item[Definitions.objectFilePath] = 
          Definitions.getProperty(sceneData, Definitions.objectFilePath);
      item[Definitions.smoothing] = 
          Definitions.getProperty(sceneData, Definitions.smoothing);
    }
    else {
      item[Definitions.objectFilePath] = "";
      item[Definitions.smoothing] = false;
    }
    item[Definitions.surfaceIndex] = 
        Definitions.getProperty(sceneData, Definitions.surfaceIndex);
    item[Definitions.isEmissiveObject] = 
        Definitions.getProperty(sceneData, Definitions.isEmissiveObject);
    item[Definitions.emitterIndex] = 
        Definitions.getProperty(sceneData, Definitions.emitterIndex);
  }
}
