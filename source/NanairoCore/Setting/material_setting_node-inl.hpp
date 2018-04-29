/*!
  \file material_setting_node-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MATERIAL_SETTING_NODE_INL_HPP
#define NANAIRO_MATERIAL_SETTING_NODE_INL_HPP

#include "material_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <vector>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "emitter_setting_node.hpp"
#include "setting_node_base.hpp"
#include "surface_setting_node.hpp"
#include "texture_setting_node.hpp"

namespace nanairo {

/*!
  */
template <SettingNodeType kMaterialType>
MaterialSettingNode<kMaterialType>::MaterialSettingNode(
    const SettingNodeBase* parent) noexcept :
        SettingNodeBase(parent),
        material_list_{dataResource()},
        material_body_list_{dataResource()}
{
}

/*!
  */
template <SettingNodeType kMaterialType>
void MaterialSettingNode<kMaterialType>::initialize() noexcept
{
  material_list_.clear();
  material_body_list_.clear();
}

/*!
  */
template <SettingNodeType kMaterialType> inline
SettingNodeBase* MaterialSettingNode<kMaterialType>::addMaterial() noexcept
{
  zisc::UniqueMemoryPointer<SettingNodeBase> material_node;
  switch (kMaterialType)
  {
   case SettingNodeType::kTextureModel: {
    material_node =
        zisc::UniqueMemoryPointer<TextureSettingNode>::make(dataResource(), this);
    break;
   }
   case SettingNodeType::kSurfaceModel: {
    material_node =
        zisc::UniqueMemoryPointer<SurfaceSettingNode>::make(dataResource(), this);
    break;
   }
   case SettingNodeType::kEmitterModel: {
    material_node =
        zisc::UniqueMemoryPointer<EmitterSettingNode>::make(dataResource(), this);
    break;
   }
   default: {
    ZISC_ASSERT(false, "Unsupported material type is specified.");
    break;
   }
  }
  material_body_list_.emplace_back(std::move(material_node));
  auto material = material_body_list_.back().get();
  material->initialize();
  material_list_.emplace_back(material);
  return material;
}

/*!
  */
template <SettingNodeType kMaterialType> inline
zisc::pmr::vector<SettingNodeBase*>& MaterialSettingNode<kMaterialType>::materialList()
    noexcept
{
  return material_list_;
}

/*!
  */
template <SettingNodeType kMaterialType> inline
const zisc::pmr::vector<SettingNodeBase*>& MaterialSettingNode<kMaterialType>::materialList()
    const noexcept
{
  return material_list_;
}

/*!
  */
template <SettingNodeType kMaterialType> inline
SettingNodeType MaterialSettingNode<kMaterialType>::nodeType() noexcept
{
  return kMaterialType;
}

/*!
  */
template <SettingNodeType kMaterialType> inline
uint MaterialSettingNode<kMaterialType>::numOfMaterials() const noexcept
{
  return zisc::cast<uint>(material_list_.size());
}

/*!
  */
template <SettingNodeType kMaterialType>
void MaterialSettingNode<kMaterialType>::readData(std::istream* data_stream)
    noexcept
{
  uint32 num_of_materials = 0;
  zisc::read(&num_of_materials, data_stream);
  ZISC_ASSERT(0 < num_of_materials, "The num of materials is zero.");
  material_list_.reserve(num_of_materials);
  material_body_list_.reserve(num_of_materials);
  for (uint i = 0; i < num_of_materials; ++i) {
    auto material = addMaterial();
    SettingNodeType material_type;
    zisc::read(&material_type, data_stream); //!< \todo Check material type
    ZISC_ASSERT(material_type ==
        ((type() == SettingNodeType::kTextureModel) ? SettingNodeType::kTexture :
         (type() == SettingNodeType::kSurfaceModel) ? SettingNodeType::kSurface
                                                    : SettingNodeType::kEmitter),
        "The stream header is wrong.");
    material->readData(data_stream);
  }
}

/*!
  */
template <SettingNodeType kMaterialType>
SettingNodeType MaterialSettingNode<kMaterialType>::type() const noexcept
{
  return kMaterialType;
}

/*!
  */
template <SettingNodeType kMaterialType>
void MaterialSettingNode<kMaterialType>::writeData(std::ostream* data_stream)
    const noexcept
{
  writeType(data_stream);

  const auto& material_list = materialList();
  const uint32 num_of_materials = zisc::cast<uint32>(material_list.size());
  zisc::write(&num_of_materials, data_stream);
  for (uint i = 0; i < num_of_materials; ++i) {
    const auto material = material_list[i];
    material->writeData(data_stream);
  }
}

} // namespace nanairo

#endif // NANAIRO_MATERIAL_SETTING_NODE_INL_HPP
