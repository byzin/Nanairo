/*!
  \file NMaterialModel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.12
import "../../definitions.js" as Definitions

QtObject {
  id: materialModel

  property var model: ListModel {
    dynamicRoles: true
    function isValidIndex(index) {
      return materialModel.isInBounds(index, 0, count);
    }
  }

  function addMaterial(element) {
    model.append(element);
  }

  function clear() {
    model.clear();
  }

  function deleteMaterial(index) {
    console.assert(isValidIndex(index),
                   "The material index is out of range: ", index)
    model.remove(index, 1);
  }

  function getMaterial(index) {
    console.assert(isValidIndex(index),
                   "The material index is out of range: ", index)
    return model.get(index);
  }

  function isInBounds(x, lower, upper) {
    var result = (lower <= x) && (x < upper);
    return result;
  }

  function isValidIndex(index) {
    return model.isValidIndex(index);
  }

  function makeMaterial() {
    var element = {};
    // Initialize the material
    element[Definitions.modelNameKey] = "Material";

    return element;
  }
}
