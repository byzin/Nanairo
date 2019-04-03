/*!
  \file NPreviewEventArea.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import QtQuick.Controls 2.12
import "Items"
import "definitions.js" as Definitions

MouseArea {
  id: eventArea

  // Axis event type
  readonly property int xAxisEvent: 0
  readonly property int yAxisEvent: 1
  // Transformation event type
  readonly property int horizontalTranslationEvent: 0
  readonly property int verticalTranslationEvent: 1
  readonly property int rotationEvent:2
  //
  readonly property int diffThreshold: 32
  property bool eventEnabled: false
  property int eventButton: 0
  property int previousX: 0
  property int previousY: 0
  property int diffX: 0
  property int diffY: 0

  acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton
  hoverEnabled: false

  onPressed: {
    if (mouse.buttons == mouse.button)
      eventButton = mouse.button;
  }

  onReleased: {
    if (mouse.button == eventButton) {
      eventButton = 0;
      eventEnabled = false;
    }
  }

  onPressAndHold: {
    if (eventButton == (mouse.buttons & eventButton)) {
      eventEnabled = true;
      previousX = mouse.x;
      previousY = mouse.y;
    }
  }

  onPositionChanged: {
    if (eventEnabled) {
      var transformationEventType = getTransformationEventType(eventButton);
      // X
      var deltaX = mouse.x - previousX;
      diffX = diffX + deltaX;
      if (diffThreshold < Math.abs(diffX)) {
        nanairoManager.previewEvent(transformationEventType, xAxisEvent, diffX);
        previousX = mouse.x;
        diffX = 0;
      }
      // Y
      var deltaY = mouse.y - previousY;
      diffY = diffY + deltaY;
      if (diffThreshold < Math.abs(diffY)) {
        nanairoManager.previewEvent(transformationEventType, yAxisEvent, diffY);
        previousY = mouse.y;
        diffY = 0;
      }
    }
  }

  function getTransformationEventType(mouseButton) {
    var eventType = (mouseButton == Qt.LeftButton)  ? rotationEvent
                  : (mouseButton == Qt.RightButton) ? verticalTranslationEvent
                  :                                   horizontalTranslationEvent;
    return eventType;
  }
}
