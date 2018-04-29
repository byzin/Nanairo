/*!
  \file bvh_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_BVH_SETTING_NODE_HPP
#define NANAIRO_BVH_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

// BinaryRadixTree has no parameter

//! AgglomerativeTreeletRestructuring parameters
struct AgglomerativeTreeletRestructuringParameters : public NodeParameterBase
{
  //! Read the parameters from the setting
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the setting
  void writeData(std::ostream* data_stream) const noexcept override;

  uint32 treelet_size_ = 9;
  uint32 optimization_loop_ = 2;
};

/*!
  */
class BvhSettingNode : public SettingNodeBase
{
 public:
  //! Create a BVH settings
  BvhSettingNode(const SettingNodeBase* parent) noexcept;


  //! Return the AgglomerativeTreeletRestructuring parameters
  AgglomerativeTreeletRestructuringParameters&
  agglomerativeTreeletRestructuringParameters() noexcept;

  //! Return the AgglomerativeTreeletRestructuring parameters
  const AgglomerativeTreeletRestructuringParameters&
  agglomerativeTreeletRestructuringParameters() const noexcept;

  //! Return the bvh type
  BvhType bvhType() const noexcept;

  //! Initialize a bvh setting
  void initialize() noexcept override;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Read the bvh setting data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set the bvh type
  void setBvhType(const BvhType type) noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the bvh setting data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::UniqueMemoryPointer<NodeParameterBase> parameters_;
  BvhType bvh_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_BVH_SETTING_NODE_HPP
