/*!
  \file NObjectModel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import "../../definitions.js" as Definitions

QtObject {
  id: objectModel

  property var model: ListModel {
    dynamicRoles: true
    function isValidIndex(index) {
      return Definitions.isInBounds(index, 0, count);
    }
  }

  function addObject(element) {
    model.append(element);
  }

  function clear() {
    model.clear();
  }

  function cloneObject(item) {
    var newItem = {};
    for (var key in item) {
      if (key == Definitions.modelTransformationKey) {
        var tList = item[key];
        var newTList = [];
        for (var i = 0; i < tList.count; ++i) {
          var transformationItem = tList.get(i);
          newTList.push(Definitions.copyObject(transformationItem));
        }
        newItem[key] = newTList;
      }
      else {
        newItem[key] = item[key]
      }
    }
    return newItem;
  }

  function deleteObject(index) {
    console.assert(isValidIndex(index),
                   "The object index is out of range: ", index);
    var num = getNumOfDescendants(index) + 1;
    model.remove(index, num);
  }

  function getObject(index) {
    console.assert(isValidIndex(index),
                   "The object index is out of range: ", index);
    return model.get(index);
  }

  function getSiblingIndex(index, direction)
  {
    console.assert(isValidIndex(index),
                   "The object index is out of range: ", index);
    var sourceObject = getObject(index);
    var sourceGroupLevel = Definitions.getProperty(sourceObject, Definitions.modelGroupLevelKey);
    var siblingIndex = -1;
    for (var i = index + direction; isValidIndex(i); i = i + direction) {
      var item = getObject(i);
      var itemGroupLevel = Definitions.getProperty(item, Definitions.modelGroupLevelKey);
      if (itemGroupLevel == sourceGroupLevel) {
        siblingIndex = i;
        break;
      }
      else if (itemGroupLevel < sourceGroupLevel) {
        break;
      }
    }
    return siblingIndex;
  }

  function getPrevSiblingIndex(index) {
    return getSiblingIndex(index, -1);
  }

  function getNextSiblingIndex(index) {
    return getSiblingIndex(index, 1);
  }

  function getNumOfDescendants(index) {
    console.assert(isValidIndex(index),
                   "The object index is out of range: ", index);
    var count = 0;
    if (isValidIndex(index)) {
      var parent = getObject(index);
      var groupLevel = Definitions.getProperty(parent, Definitions.modelGroupLevelKey);
      for (var i = index + 1; i < model.count; ++i) {
        const child = getObject(i);
        var isChild = groupLevel < Definitions.getProperty(child, Definitions.modelGroupLevelKey);
        if (!isChild)
          break;
        ++count;
      }
    }
    return count;
  }

  function isValidIndex(index) {
    return model.isValidIndex(index);
  }

  function makeGeneralObject() {
    var element = {};
    // Initialize the object
    element[Definitions.modelGroupLevelKey] = 0;
    element[Definitions.modelEnabledKey] = true;
    element[Definitions.modelExpandedKey] = false;
    element[Definitions.modelVisibleIndexKey] = -1;
    element[Definitions.modelTransformationKey] = [];

    return element;
  }

  function makeCameraObject() {
    var element = makeGeneralObject();
    // Initialize the object
    element[Definitions.modelNameKey] = "Camera";
    element[Definitions.modelTypeKey] = Definitions.cameraObject;

    return element;
  }

  function makeSingleObject() {
    var element = makeGeneralObject();
    // Initialize the object
    element[Definitions.modelNameKey] = "Object";
    element[Definitions.modelTypeKey] = Definitions.singleObject;

    return element;
  }

  function makeGroupObject() {
    var element = makeGeneralObject();
    // Initialize the object
    element[Definitions.modelNameKey] = "Group";
    element[Definitions.modelTypeKey] = Definitions.groupObject;

    return element;
  }

  function moveObject(source, dest) {
    console.assert(isValidIndex(source),
                   "The object index is out of range: ", source);
    console.assert(isValidIndex(dest),
                   "The object index is out of range: ", dest);
    // Make a move list
    var moveList = [];
    var num = getNumOfDescendants(source) + 1;
    console.assert(!((source <= dest) && (dest < source + num)), 
                   "The dest object is the child of source.");
    for (var i = source; i < (source + num); ++i) {
      var o = getObject(i);
      moveList.push(cloneObject(o));
    }
    model.remove(source, num);
    // Move items
    var destination = (source < dest) ? dest - num : dest;
    destination = destination + getNumOfDescendants(destination) + 1;
    for (var i = moveList.length - 1; 0 <=  i; --i)
      model.insert(destination, moveList[i]);
  }

  function toChild(source, dest) {
    console.assert(isValidIndex(source),
                   "The object index is out of range: ", source);
    console.assert(isValidIndex(dest),
                   "The object index is out of range: ", dest);
    // Calc the difference of the group level
    var sourceObject = getObject(source);
    var destObject = getObject(dest);
    var diffLevel =
        Definitions.getProperty(destObject, Definitions.modelGroupLevelKey) -
        Definitions.getProperty(sourceObject, Definitions.modelGroupLevelKey);
    var num = getNumOfDescendants(source) + 1;
    console.assert(!((source <= dest) && (dest < source + num)), 
                   "The dest object is the child of source.");
    for (var i = source; i < (source + num); ++i) {
      var moveItem = getObject(i);
      moveItem[Definitions.modelGroupLevelKey] += (diffLevel + 1);
    }
    // Move objects
    moveObject(source, dest);
  }

  function updateItemVisible() {
    var parentExpandList = [true];
    var previousGroupLevel = 0;
    var itemVisibleIndex = 0;
    for (var i = 0; i < model.count; ++i) {
      var item = getObject(i);
      var itemGroupLevel = Definitions.getProperty(item, Definitions.modelGroupLevelKey);
      var itemVisible = parentExpandList[parentExpandList.length - 1];
      if (previousGroupLevel < itemGroupLevel) {
        var parent = getObject(i - 1);
        itemVisible = itemVisible && Definitions.getProperty(parent, Definitions.modelExpandedKey);
        parentExpandList.push(itemVisible);
      }
      else if (itemGroupLevel < previousGroupLevel) {
        for (var j = 0; j < (previousGroupLevel - itemGroupLevel); ++j) {
          console.assert(0 < parentExpandList.length, "The update is failed.");
          parentExpandList.pop();
        }
        itemVisible = parentExpandList[parentExpandList.length - 1];
      }
      if (itemVisible) {
        Definitions.setProperty(item, Definitions.modelVisibleIndexKey, itemVisibleIndex);
        ++itemVisibleIndex;
      }
      else {
        Definitions.setProperty(item, Definitions.modelVisibleIndexKey, -1);
      }
      previousGroupLevel = itemGroupLevel;
    }
    return itemVisibleIndex; // The number of visible items
  }
}
