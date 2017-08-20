/*!
  \file group_object_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
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
  //! Make a object and add it to the group
  SettingNodeBase* addObject() noexcept;

  //! Initialize group object setting
  virtual void initialize() noexcept override;

  //! Read the group object setting data from the stream
  virtual void readData(std::istream* data_stream) noexcept override;

  //! Return the object list
  std::vector<SettingNodeBase*>& objectList() noexcept;

  //! Return the object list
  const std::vector<SettingNodeBase*>& objectList() const noexcept;

  //! Return the node type
  virtual SettingNodeType type() const noexcept override;

  //! Write the group object setting to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept override;

 private:
  std::vector<std::unique_ptr<SettingNodeBase>> object_list_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_GROUP_OBJECT_SETTING_NODE_HPP
