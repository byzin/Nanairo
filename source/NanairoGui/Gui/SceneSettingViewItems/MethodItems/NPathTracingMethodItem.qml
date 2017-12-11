/*!
  \file NPathTracingMethodItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.9
import QtQuick.Controls 2.2
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
      text: "eye path light sampler"
    }

    NLightSampler {
      id: lightSampler

      Layout.fillWidth: true
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
