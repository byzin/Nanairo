/*!
  \file NCameraSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.11
import "CameraObjectItems"
import "../../Items"
import "../../definitions.js" as Definitions

NScrollView {
  id: cameraSettingView

  property var objectItem: null
  property string cameraType: ""
  property bool jittering: false

  GridLayout {
    columns: 2
    columnSpacing: Definitions.defaultItemSpace
    rowSpacing: Definitions.defaultItemSpace

    NGroupBox {
      id: group
      title: "camera type"
      color: cameraSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NComboBox {
          id: cameraTypeComboBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          currentIndex: find(cameraSettingView.cameraType)
          model: [Definitions.pinholeCamera]

          onCurrentTextChanged: cameraSettingView.cameraType = currentText
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color
        }
      }
    }

    NGroupBox {
      title: "camera parameters"
      color: cameraSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      StackLayout {
        id: cameraItemLayout

        anchors.fill: parent
        currentIndex: -1

        NPinholeCameraItem {
          id: pinholeCameraItem
          onAngleOfViewChanged: cameraSettingView.setProperty(Definitions.angleOfView, angleOfView)
        }
      }

      Component.onCompleted: {
        for (var i = 0; i < cameraItemLayout.children.length; ++i) {
          var cameraView = cameraItemLayout.children[i];
          cameraView.background.color = background.color;
        }
      }
    }

    NGroupBox {
      title: "jittering"
      color: cameraSettingView.background.color
      Layout.preferredWidth: Definitions.defaultSettingGroupWidth
      Layout.preferredHeight: Definitions.defaultSettingGroupHeight

      ColumnLayout {
        anchors.fill: parent

        NCheckBox {
          id: jitteringCheckBox

          Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
          Layout.fillWidth: true
          Layout.preferredHeight: Definitions.defaultSettingItemHeight
          checked: cameraSettingView.jittering
          text: "jittering"

          onCheckedChanged: cameraSettingView.jittering = checked
        }

        NPane {
          Layout.fillWidth: true
          Layout.fillHeight: true
          Component.onCompleted: background.color = group.background.color
        }
      }
    }
  }

  onCameraTypeChanged: {
    var viewIndex = cameraTypeComboBox.find(cameraType);
    if ((objectItem != null) && (viewIndex != -1)) {
      // Set base camera properties
      setProperty(Definitions.cameraType, cameraType);
      setProperty(Definitions.jittering, jittering);
      // Set other camera properties
      var settingView = cameraItemLayout.children[viewIndex];
      settingView.initItem(objectItem);
      // Change the camera setting view
      cameraItemLayout.currentIndex = viewIndex;
      // Set the camera properties to the view
      settingView.setValue(objectItem);
    }
  }

  onJitteringChanged: setProperty(Definitions.jittering, jittering)

  function initItem(item) {
    console.assert(item != null, "The item is null.");
    // Set the properties of pinhole camera
    item[Definitions.cameraType] = Definitions.pinholeCamera;
    item[Definitions.jittering] = true;
    pinholeCameraItem.initItem(item);
  }

  function setProperty(propertyName, value) {
    if (objectItem != null)
      Definitions.setProperty(objectItem, propertyName, value);
  }

  function setItem(item) {
    objectItem = null;
    if (item != null) {
      // Set camera type to the view
      cameraType = Definitions.getProperty(item, Definitions.cameraType);
      jittering = Definitions.getProperty(item, Definitions.jittering);
      // Change the camera setting view
      var viewIndex = cameraTypeComboBox.find(cameraType);
      console.assert(viewIndex != -1, "The item has invalid camera type.");
      cameraItemLayout.currentIndex = viewIndex;
      // Set the item properties to the view
      var settingView = cameraItemLayout.children[viewIndex];
      settingView.setValue(item);
    }
    objectItem = item;
  }

  function getSceneData(item) {
    var cameraType = Definitions.getProperty(item, Definitions.cameraType);

    var viewIndex = cameraTypeComboBox.find(cameraType);
    var cameraView = cameraItemLayout.children[viewIndex];

    var sceneData = cameraView.getSceneData(item);
    sceneData[Definitions.cameraType] = cameraType;
    sceneData[Definitions.jittering] =
        Definitions.getProperty(item, Definitions.jittering);

    return sceneData;
  }

  function setSceneData(sceneData, item) {
    var cameraType = Definitions.getProperty(sceneData, Definitions.cameraType);
    item[Definitions.cameraType] = cameraType;
    item[Definitions.jittering] =
        Definitions.getProperty(sceneData, Definitions.jittering);

    var viewIndex = cameraTypeComboBox.find(cameraType);
    var cameraView = cameraItemLayout.children[viewIndex];
    cameraView.setSceneData(sceneData, item);
  }
}
