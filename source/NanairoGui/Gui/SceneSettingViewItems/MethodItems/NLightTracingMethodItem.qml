/*!
  \file NLightTracingMethodItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import "../../Items"
import "../../definitions.js" as Definitions

NPane {
  id: methodItem

  ColumnLayout {
    id: column1

    width: Definitions.defaultSettingItemWidth
    spacing: Definitions.defaultItemSpace

    NLabel {
      text: "light path light sampler"
    }

    NLightSampler {
      id: lightSampler

      Layout.fillWidth: true
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
