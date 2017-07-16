/*!
  \file NObjectModel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../nanairo.js" as Nanairo

QtObject {
  id: object_model

  property var model: ListModel {
  }

  function clear() {
    var count = model.count;
    for (var i = 0; i < count; ++i) {
      model.remove(0, 1);
    }
  }

  function createGeneralElement() {
    var element = {};
    element.groupLevel = 0;
    element.enabled = true;
    element.expanded = false;
    element.visible = true;
    return element;
  }

  function addElement(element) {
    model.append(element);
  }

  function createCameraElement() {
    var element = createGeneralElement();
    element.name = "Camera";
    element.type = Nanairo.cameraObject;
    return element;
  }

  function createObjectElement() {
    var element = createGeneralElement();
    element.name = "Object";
    element.type = Nanairo.singleObject;
    return element;
  }

  function createGroupElement() {
    var element = createGeneralElement();
    element.name = "Group";
    element.type = Nanairo.groupObject;
    return element;
  }

  function isValidIndex(index) {
    var isValid = (0 <= index) && (index < model.count);
    return isValid;
  }

  function getObject(index) {
    console.assert(isValidIndex(index), "Object index is out of range: ", index);
    return model.get(index);
  }

  function getNumOfDescendants(index) {
    var count = 0;
    if (isValidIndex(index)) {
      var parent = getObject(index);
      var groupLevel = parent.groupLevel;
      for (var i = index + 1; i < model.count; ++i) {
        var isNotChild = getObject(i).groupLevel <= groupLevel;
        if (isNotChild)
          break;
        ++count;
      }
    }
    return count;
  }

  function cloneModelItem(item) {
    var copyItem = {};
    for (var key in item) {
      copyItem[key] = item[key];
    }
    return copyItem;
  }

  function moveObjects(source, dest, num) {
    // Make a move list
    var moveList = [];
    var diffLevel = getObject(dest).groupLevel - getObject(source).groupLevel;
    for (var i = source; i < (source + num); ++i) {
      var moveItem = getObject(i);
      moveItem.groupLevel += (diffLevel + 1);
      moveList.push(cloneModelItem(moveItem));
    }
    model.remove(source, num);
    // Move items
    var destination = (source < dest) ? dest - num
                                      : dest;
    destination = destination + getNumOfDescendants(destination) + 1;
    for (var i = moveList.length - 1; 0 <= i; --i) {
      model.insert(destination, moveList[i]);
    }
  }

  function updateItemVisible() {
    var parentExpandList = [];
    var previousGroupLevel = 0;
    for (var i = 0; i < model.count; ++i) {
      var item = getObject(i);
      if (previousGroupLevel < item.groupLevel) {
        var parent = getObject(i - 1);
        parentExpandList.push(parent.expanded);
      }
      else if (item.groupLevel < previousGroupLevel) {
        for (var j = 0; j < (previousGroupLevel - item.groupLevel); ++j) {
          parentExpandList.pop();
        }
      }
      var itemVisible = true;
      for (var j = 0; j < parentExpandList.length; ++j) {
        itemVisible = itemVisible && parentExpandList[j];
      }
      item.visible = itemVisible;
      previousGroupLevel = item.groupLevel;
    }
  }
}
