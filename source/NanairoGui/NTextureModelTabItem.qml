/*!
  \file NTextureModelTabItem.qml
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
  id: texture_model_tab_item

  materialName: qsTr("Texture")
  materialTypeList: [Nanairo.valueTexture,
                     Nanairo.unicolorTexture,
                     Nanairo.checkerboardTexture,
                     Nanairo.imageTexture]
  materialComponent: setting_view_component
  addMaterialButtonImage: "../image/add_texture_model.png"
  addMaterialButtonDisabledImage: "../image/add_texture_model_disabled.png"
  deleteMaterialButtonImage: "../image/delete_texture_model.png"
  deleteMaterialButtonDisabledImage: "../image/delete_texture_model_disabled.png"

  Component {
    id: setting_view_component
    NTextureModelSettingView {
      id: texture_item_setting_view
      color: texture_model_tab_item.color
      currentTypeIndex: texture_model_tab_item.currentMaterialTypeIndex
    }
  }
}
