/*!
  \file NSurfaceModelTabItem.qml
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
  id: surface_model_tab_item

  property var textureModel: null

  materialName: qsTr("Surface")
  materialTypeList: [Nanairo.smoothDiffuseSurface,
                     Nanairo.smoothDielectricSurface,
                     Nanairo.smoothConductorSurface,
                     Nanairo.roughDielectricSurface,
                     Nanairo.roughConductorSurface,
                     Nanairo.layeredDiffuseSurface,
                     Nanairo.clothSurface]
  materialComponent: setting_view_component
  addMaterialButtonImage:"../image/add_surface_model.png"
  addMaterialButtonDisabledImage: "../image/add_surface_model_disabled.png"
  deleteMaterialButtonImage: "../image/delete_surface_model.png"
  deleteMaterialButtonDisabledImage: "../image/delete_surface_model_disabled.png"

  Component {
    id: setting_view_component
    NSurfaceModelSettingView {
      id: surface_item_setting_view
      color: surface_model_tab_item.color
      currentTypeIndex: surface_model_tab_item.currentMaterialTypeIndex
      textureModel: surface_model_tab_item.textureModel
    }
  }
}
