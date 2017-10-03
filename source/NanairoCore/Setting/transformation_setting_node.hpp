/*!
  \file transformation_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TRANSFORMATION_SETTING_NODE_HPP
#define NANAIRO_TRANSFORMATION_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <ostream>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  */
class TransformationSettingNode : public SettingNodeBase
{
 public:
  //! Initialize the value
  void initialize() noexcept override;

  //! Check if the transformation is enabled
  bool isEnabled() const noexcept;

  //! Read the transformation data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Set if the transformation is enabled
  void setEnabled(const bool is_enabled) noexcept;

  //! Set the transformation type
  void setTransformationType(const TransformationType type) noexcept;

  //! Set the value
  void setValue(const double x, const double y, const double z) noexcept;

  //! Return the transformation type
  TransformationType transformationType() const noexcept;

  //! Return the setting node type
  SettingNodeType type() const noexcept override;

  //! Return the value
  const std::array<double, 3>& value() const noexcept;

  //! Write the transformation data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  //! Return the x value
  double x() const noexcept;

  //! Return the y value
  double y() const noexcept;

  //! Return the z value
  double z() const noexcept;


 private:
  TransformationType type_;
  std::array<double, 3> value_;
  uint8 is_enabled_;
};

} // namespace nanairo

#endif // NANAIRO_TRANSFORMATION_SETTING_NODE_HPP
