/*!
  \file NTransformationListModel.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import "../../definitions.js" as Definitions

QtObject {
  id: transformationModel

  function makeTransformationItem() {
    var item = {};
    // Initialize the properties
    item[Definitions.modelEnabledKey] = true;
    item[Definitions.modelXAxisKey] = 0.0;
    item[Definitions.modelYAxisKey] = 0.0;
    item[Definitions.modelZAxisKey] = 0.0;
    item[Definitions.modelAxisKey] = "";
    item[Definitions.modelAngleKey] = 0.0;
    item[Definitions.modelUnitKey] = "";

    return item;
  }

  function makeTranslationItem() {
    var item = makeTransformationItem();
    // Initialize the properties
    item[Definitions.modelTypeKey] = Definitions.translation;
    item[Definitions.modelXAxisKey] = 0.0;
    item[Definitions.modelYAxisKey] = 0.0;
    item[Definitions.modelZAxisKey] = 0.0;

    return item;
  }

  function makeScalingItem() {
    var item = makeTransformationItem();
    // Initialize the properties
    item[Definitions.modelTypeKey] = Definitions.scaling;
    item[Definitions.modelXAxisKey] = 1.0;
    item[Definitions.modelYAxisKey] = 1.0;
    item[Definitions.modelZAxisKey] = 1.0;

    return item;
  }

  function makeRotationItem() {
    var item = makeTransformationItem();
    // Initialize the properties
    item[Definitions.modelTypeKey] = Definitions.rotation;
    item[Definitions.modelAxisKey] = Definitions.xAxis;
    item[Definitions.modelAngleKey] = 0.0;
    item[Definitions.modelUnitKey] = Definitions.degreeUnit;

    return item;
  }
}
