/*!
  \file camera_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "camera_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"

namespace nanairo {

/*!
  */
void PinholeCameraParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&angle_of_view_, data_stream);
}

/*!
  */
void PinholeCameraParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&angle_of_view_, data_stream);
}

/*!
  */
CameraType CameraSettingNode::cameraType() const noexcept
{
  return type_;
}

/*!
  */
void CameraSettingNode::initialize() noexcept
{
  setCameraType(CameraType::kPinhole);
  setJittering(true);
}

/*!
  */
bool CameraSettingNode::jittering() const noexcept
{
  return jittering_ == kTrue;
}

/*!
  */
PinholeCameraParameters& CameraSettingNode::pinholeCameraParameters() noexcept
{
  ZISC_ASSERT(cameraType() == CameraType::kPinhole,
              "Invalid camera type is specified.");
  auto parameter = zisc::cast<PinholeCameraParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
const PinholeCameraParameters& CameraSettingNode::pinholeCameraParameters()
    const noexcept
{
  ZISC_ASSERT(cameraType() == CameraType::kPinhole,
              "Invalid camera type is specified.");
  auto parameter = zisc::cast<const PinholeCameraParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
void CameraSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    zisc::read(&type_, data_stream);
    setCameraType(type_);
  }
  zisc::read(&jittering_, data_stream);
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
void CameraSettingNode::setCameraType(const CameraType type) noexcept
{
  type_ = type;
  // Initialize parameters
  parameters_.reset(nullptr);
  switch (type_) {
   case CameraType::kPinhole: {
    parameters_ = std::make_unique<PinholeCameraParameters>();
    break;
   }
   case CameraType::kThinLens:
   default:
    break;
  }
}

/*!
  */
void CameraSettingNode::setJittering(const bool jittering) noexcept
{
  jittering_ = jittering ? kTrue : kFalse;
}

/*!
  */
SettingNodeType CameraSettingNode::type() const noexcept
{
  return SettingNodeType::kCameraObject;
}

/*!
  */
void CameraSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);

  // Write properties
  zisc::write(&type_, data_stream);
  zisc::write(&jittering_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
