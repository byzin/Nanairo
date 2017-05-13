/*!
  \file transformation_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "transformation_setting_node.hpp"
// Standard C++ library
#include <array>
#include <istream>
#include <ostream>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"

namespace nanairo {

/*!
  */
void TransformationSettingNode::initialize() noexcept
{
  setTransformationType(TransformationType::kTranslation);
}

/*!
  */
bool TransformationSettingNode::isEnabled() const noexcept
{
  return is_enabled_ == kTrue;
}

/*!
  */
void TransformationSettingNode::readData(std::istream* data_stream) noexcept
{
  zisc::read(&type_, data_stream);
  constexpr uint size = sizeof(value_[0]);
  zisc::read(&value_, data_stream, size * 3);
  zisc::read(&is_enabled_, data_stream);
}

/*!
  */
void TransformationSettingNode::setEnabled(const bool is_enabled) noexcept
{
  is_enabled_ = is_enabled ? kTrue : kFalse;
}

/*!
  */
void TransformationSettingNode::setTransformationType(const TransformationType type)
    noexcept
{
  type_ = type;
  switch (type) {
   case TransformationType::kTranslation:
    setValue(0.0, 0.0, 0.0);
    break;
   case TransformationType::kScaling:
    setValue(1.0, 1.0, 1.0);
    break;
   case TransformationType::kRotation:
    setValue(0.0, 0.0, 0.0);
    break;
   default:
    zisc::raiseError("Invalid transformation is specified.");
    break;
  }
}

/*!
  */
void TransformationSettingNode::setValue(const double x,
                                         const double y,
                                         const double z) noexcept
{
  value_[0] = x;
  value_[1] = y;
  value_[2] = z;
}

/*!
  */
TransformationType TransformationSettingNode::transformationType() const noexcept
{
  return type_;
}

/*!
  */
SettingNodeType TransformationSettingNode::type() const noexcept
{
  return SettingNodeType::kTransformation;
}

/*!
  */
const std::array<double, 3>& TransformationSettingNode::value() const noexcept
{
  return value_;
}

/*!
  */
void TransformationSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  // Write properties
  zisc::write(&type_, data_stream);
  constexpr uint size = sizeof(value_[0]);
  zisc::write(&value_, data_stream, size * 3);
  zisc::write(&is_enabled_, data_stream);
}

/*!
  */
double TransformationSettingNode::x() const noexcept
{
  const auto& v = value();
  return v[0];
}

/*!
  */
double TransformationSettingNode::y() const noexcept
{
  const auto& v = value();
  return v[1];
}

/*!
  */
double TransformationSettingNode::z() const noexcept
{
  const auto& v = value();
  return v[2];
}

} // namespace nanairo
