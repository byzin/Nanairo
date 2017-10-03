/*!
  \file object_model_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_OBJECT_MODEL_SETTING_NODE_HPP
#define NANAIRO_OBJECT_MODEL_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  */
class ObjectModelSettingNode : public SettingNodeBase
{
 public:
  //! Make a transformation and add it to the transformation list
  SettingNodeBase* addTransformation(const TransformationType type,
                                     const double x,
                                     const double y,
                                     const double z) noexcept;

  //! Initialize the object model
  void initialize() noexcept override;

  //! Return the object name
  const std::string& name() const noexcept;

  //! Return the number of visible faces
  uint numOfVisibleFaces() const noexcept;

  //! Return the object model setting node
  SettingNodeBase* objectSettingNode() noexcept;

  //! Return the object model setting node
  const SettingNodeBase* objectSettingNode() const noexcept;

  //! Read the object setting data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set the object name
  void setName(const std::string& name) noexcept;

  //! Set the object name
  void setName(std::string&& name) noexcept;

  //! Create new object instance and set to this object
  SettingNodeBase* setObject(const ObjectType type) noexcept;

  //! Set the visibility of the object
  void setVisibility(const bool visibility) noexcept;

  //! Return the transformation setting node list
  std::vector<SettingNodeBase*>& transformationList() noexcept;

  //! Return the transformation setting node list
  const std::vector<SettingNodeBase*>& transformationList() const noexcept;

  //! Return the node type
  SettingNodeType type() const noexcept override;

  //! Return the visibility of the object
  bool visibility() const noexcept;

  //! Write the object setting data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  std::string name_;
  std::vector<std::unique_ptr<SettingNodeBase>> transformation_list_;
  std::unique_ptr<SettingNodeBase> object_setting_node_;
  uint8 visibility_;
};

} // namespace nanairo

#endif // NANAIRO_OBJECT_MODEL_SETTING_NODE_HPP
