/*!
  \file NTranslationItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "../../SettingBlockItems"
import "../../nanairo.js" as Nanairo

NTransformationItem {
  id: translation_item

  labelText: Nanairo.translation

  NSpinBox {
    id: x_spinbox

    anchors.horizontalCenter: undefined
    x: translation_item.itemOffset
    y: translation_item.labelHeight + translation_item.itemOffset
    enabled: itemEnabled

    maximumValue: Nanairo.transformationMaxValue
    minimumValue: -maximumValue
    suffix: " x"
    onValueChanged: model.x = value
  }

  NSpinBox {
    id: y_spinbox

    anchors.horizontalCenter: undefined
    x: (x_spinbox.x + x_spinbox.width) + translation_item.itemOffset
    y: x_spinbox.y
    enabled: itemEnabled

    maximumValue: Nanairo.transformationMaxValue
    minimumValue: -maximumValue
    suffix: " y"
    onValueChanged: model.y = value
  }

  NSpinBox {
    id: z_spinbox

    anchors.horizontalCenter: undefined
    x: (y_spinbox.x + y_spinbox.width) + translation_item.itemOffset
    y: y_spinbox.y
    enabled: itemEnabled

    maximumValue: Nanairo.transformationMaxValue
    minimumValue: -maximumValue
    suffix: " z"
    onValueChanged: model.z = value
  }

  Component.onCompleted: {
    x_spinbox.value = model.x;
    y_spinbox.value = model.y;
    z_spinbox.value = model.z;
  }
}
