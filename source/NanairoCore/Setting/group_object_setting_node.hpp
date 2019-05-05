/*!
  \file group_object_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_GROUP_OBJECT_SETTING_NODE_HPP
#define NANAIRO_GROUP_OBJECT_SETTING_NODE_HPP

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

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class GroupObjectSettingNode : public SettingNodeBase
{
 public:
  //! Create a group object settings
  GroupObjectSettingNode(const SettingNodeBase* parent) noexcept;


  //! Make a object and add it to the group
  SettingNodeBase* addObject() noexcept;

  //! Initialize group object setting
  void initialize() noexcept override;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Read the group object setting data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the object list
  zisc::pmr::vector<SettingNodeBase*>& objectList() noexcept;

  //! Return the object list
  const zisc::pmr::vector<SettingNodeBase*>& objectList() const noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Write the group object setting to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::pmr::vector<SettingNodeBase*> object_list_;
  zisc::pmr::vector<zisc::UniqueMemoryPointer<SettingNodeBase>> object_body_list_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_GROUP_OBJECT_SETTING_NODE_HPP
