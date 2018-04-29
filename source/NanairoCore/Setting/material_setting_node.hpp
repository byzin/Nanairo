/*!
  \file material_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_MATERIAL_SETTING_NODE_HPP
#define NANAIRO_MATERIAL_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
template <SettingNodeType kMaterialType>
class MaterialSettingNode : public SettingNodeBase
{
 public:
  //! Create a material settings
  MaterialSettingNode(const SettingNodeBase* parent) noexcept;


  //! Make a material and add it to the material list
  SettingNodeBase* addMaterial() noexcept;

  //! Initialize a material setting node
  void initialize() noexcept override;

  //! Return the material list
  zisc::pmr::vector<SettingNodeBase*>& materialList() noexcept;

  //! Return the material list
  const zisc::pmr::vector<SettingNodeBase*>& materialList() const noexcept;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Return the num of materials
  uint numOfMaterials() const noexcept;

  //! Read the material setting from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the material setting to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::pmr::vector<SettingNodeBase*> material_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<SettingNodeBase>> material_body_list_;
};

using TextureModelSettingNode = MaterialSettingNode<SettingNodeType::kTextureModel>;
using SurfaceModelSettingNode = MaterialSettingNode<SettingNodeType::kSurfaceModel>;
using EmitterModelSettingNode = MaterialSettingNode<SettingNodeType::kEmitterModel>;

//! \} Core

} // namespace nanairo

#include "material_setting_node-inl.hpp"

#endif // NANAIRO_MATERIAL_SETTING_NODE_HPP
