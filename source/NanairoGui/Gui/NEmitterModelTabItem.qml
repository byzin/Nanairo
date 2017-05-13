/*!
  \file NEmitterModelTabItem.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import "MaterialItems"
import "SettingBlockItems"
import "nanairo.js" as Nanairo

NMaterialTabItem {
  id: emitter_model_tab_item

  property var textureModel: null

  materialName: qsTr("Emitter")
  materialTypeList: [Nanairo.nonDirectionalEmitter]
  materialComponent: setting_view_component
  addMaterialButtonImage: "../image/add_emitter_model.png"
  addMaterialButtonDisabledImage: "../image/add_emitter_model_disabled.png"
  deleteMaterialButtonImage: "../image/delete_emitter_model.png"
  deleteMaterialButtonDisabledImage: "../image/delete_emitter_model_disabled.png"

  Component {
    id: setting_view_component
    NEmitterModelSettingView {
      id: emitter_item_setting_view
      color: emitter_model_tab_item.color
      currentTypeIndex: emitter_model_tab_item.currentMaterialTypeIndex
      textureModel: emitter_model_tab_item.textureModel
    }
  }
}
