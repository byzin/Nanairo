/*!
  \file NFloatSpinBox.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import "../definitions.js" as Definitions

NSpinBox {
  id: spinBox 

  readonly property int maxDecimals: 7
  readonly property real toIntScale: Math.pow(10.0, decimals)
  readonly property real realMax: Definitions.intMax / toIntScale
  property real floatFrom: 0.0
  property real floatTo: 1.0
  property int decimals: 4
  property real floatValue: 0.0

  from: toIntScale * floatFrom
  to: toIntScale * floatTo
  stepSize: 1
  value: toIntScale * floatValue

  validator: DoubleValidator {
    bottom: Math.min(spinBox.floatFrom, spinBox.floatTo);
    top: Math.max(spinBox.floatFrom, spinBox.floatTo);
  }

  textFromValue: function(value, locale) {
    var v = Number(spinBox.floatValue).toLocaleString(locale, 'f', spinBox.decimals);
    return v;
  }

  valueFromText: function(text, locale) {
    var v = spinBox.toIntScale * Number.fromLocaleString(locale, text);
    return v;
  }

  onValueChanged: floatValue = value / toIntScale

  Component.onCompleted: {
    down.indicator = null;
    up.indicator = null;

    console.assert((0 <= decimals) && (decimals <= maxDecimals),
                   "The decimals is out of range.");
    contentItem.horizontalAlignment = TextInput.AlignRight;
  }
}
