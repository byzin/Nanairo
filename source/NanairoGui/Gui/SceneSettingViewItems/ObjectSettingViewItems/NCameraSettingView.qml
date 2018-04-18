/*!
  \file NCameraSettingView.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "CameraObjectItems"
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: cameraSettingView

  property var objectItem: null
  property string cameraType: ""
  property bool jittering: false

  RowLayout {
    id: row1

    spacing: Definitions.defaultSettingItemColumnInterval

    ColumnLayout {
      id: column1

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: Definitions.defaultItemSpace

      NLabel {
        text: "type"
      }

      NComboBox {
        id: typeComboBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        currentIndex: find(cameraSettingView.cameraType)
        model: [Definitions.pinholeCamera]

        onCurrentTextChanged: cameraSettingView.cameraType = currentText
      }
    }

    ColumnLayout {
      id: column2

      Layout.preferredWidth: Definitions.defaultSettingItemWidth
      spacing: Definitions.defaultItemSpace

      NLabel {
        text: "jittering"
      }

      NCheckBox {
        id: jitteringCheckBox

        Layout.fillWidth: true
        Layout.preferredHeight: Definitions.defaultSettingItemHeight
        checked: cameraSettingView.jittering
        text: "jittering"

        onCheckedChanged: cameraSettingView.jittering = checked
      }
    }
  }

  StackLayout {
    id: cameraItemLayout

    anchors.fill: parent
    anchors.topMargin: (row1.y + row1.height) + Definitions.defaultBlockSize
    currentIndex: -1

    NPinholeCameraItem {
      id: pinholeCameraItem
      onAngleOfViewChanged: cameraSettingView.setProperty(Definitions.angleOfView, angleOfView)
    }
  }

  onCameraTypeChanged: {
    var viewIndex = typeComboBox.find(cameraType);
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
      var viewIndex = typeComboBox.find(cameraType);
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

    var viewIndex = typeComboBox.find(cameraType);
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

    var viewIndex = typeComboBox.find(cameraType);
    var cameraView = cameraItemLayout.children[viewIndex];
    cameraView.setSceneData(sceneData, item);
  }
}
