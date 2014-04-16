/*!
  \file NRenderedImageCanvas.qml
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.4
import QtQuick.Controls 1.3

Rectangle {
  id: rendered_image_canvas
  width: 320
  height: 240


  Rectangle {
    id: info_area
    height: 14
    color: rendered_image_canvas.color
    anchors.top: parent.top
    anchors.topMargin: 0
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0


    Text {
      id: fps_label
      x: 2
      y: 2
      text: qsTr("FPS: ")
      verticalAlignment: Text.AlignVCenter
      textFormat: Text.PlainText
      font.pixelSize: 9
    }

    Text {
      id: fps_value_label
      x: 23
      y: 2
      text: qsTr("0")
      verticalAlignment: Text.AlignVCenter
      textFormat: Text.PlainText
      font.pixelSize: 9
    }

    Text {
      id: cycle_label
      x: 66
      y: 2
      text: qsTr("Cycle: ")
      verticalAlignment: Text.AlignVCenter
      textFormat: Text.PlainText
      font.pixelSize: 9
    }

    Text {
      id: cycle_value_label
      x: 95
      y: 2
      text: qsTr("00000000")
      verticalAlignment: Text.AlignVCenter
      textFormat: Text.PlainText
      font.pixelSize: 9
    }

    Text {
      id: time_label
      x: 155
      y: 2
      text: qsTr("Time: ")
      textFormat: Text.PlainText
      verticalAlignment: Text.AlignVCenter
      font.pixelSize: 9
    }


    Text {
      id: time_value_label
      x: 181
      y: 2
      text: qsTr("00 h 00 m 00.000 s")
      verticalAlignment: Text.AlignVCenter
      textFormat: Text.PlainText
      font.pixelSize: 9
    }
  }

  Rectangle {
    id: canvas_area
    color: rendered_image_canvas.color
    anchors.top: parent.top
    anchors.topMargin: 14
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    anchors.right: parent.right
    anchors.rightMargin: 0
    anchors.left: parent.left
    anchors.leftMargin: 0

    onEnabledChanged: mouse_event_area.resetDragInfo()

    Image {
      id: canvas
      anchors.fill: parent
      //    fillMode: Image.PreserveAspectFit
      fillMode: Image.Pad
      cache: false

      MouseArea {
        id: mouse_event_area
        anchors.fill: parent

        property bool firstDrag: true
        property int previousX: -1
        property int previousY: -1

        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton

        drag.target: parent
        drag.axis: Drag.XAndYAxis
        drag.maximumX: parent.width
        drag.maximumY: parent.height
        drag.minimumX: 0
        drag.minimumY: 0

        onReleased: resetDragInfo()

        onPositionChanged: calcDragDifference(mouse)

        onWheel: manager.previewMouseEvent(0, 0, 0, wheel.angleDelta.y)

        function calcDragDifference(mouse) {
          if (firstDrag) {
            previousX = mouse.x
            previousY = mouse.y
            firstDrag = false
          }
          else {
            var deltaX = mouse.x - previousX
            var deltaY = mouse.y - previousY
            manager.previewMouseEvent(mouse.buttons, deltaX, deltaY, 0)
            previousX = mouse.x
            previousY = mouse.y
          }
        }

        function resetDragInfo() {
          firstDrag = true
          previousX = -1
          previousY = -1
        }
      }
    }
  }

  function enableImageCanvas(enabled) {
    canvas_area.enabled = enabled
    canvas_area.visible = enabled
  }

  function setRenderingInfo(fps, cycle, time) {
    canvas.source = "image://rendered_image/preview" + cycle
    fps_value_label.text = fps
    cycle_value_label.text = cycle
    time_value_label.text = time
  }
}
