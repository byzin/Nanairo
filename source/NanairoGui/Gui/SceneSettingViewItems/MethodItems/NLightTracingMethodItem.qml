/*!
  \file NLightTracingMethodItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import "../../Items"
import "../../definitions.js" as Definitions

NScrollView {
  id: methodItem

  ColumnLayout {
    spacing: Definitions.defaultItemSpace

    NLabel {
      Layout.alignment: Qt.AlignLeft | Qt.AlignTop
      text: "light path light sampler"
    }

    NLightSampler {
      id: lightSampler

      Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
      Layout.preferredWidth: methodItem.width
      Layout.preferredHeight: Definitions.defaultSettingItemHeight
      isEyePathSampler: false
    }
  }

  function getSceneData() {
    var sceneData = lightSampler.getSceneData();
    return sceneData;
  }

  function initSceneData() {
    lightSampler.initSceneData();
  }

  function setSceneData(sceneData) {
    lightSampler.setSceneData(sceneData);
  }
}
