/*!
  \file camera_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_CAMERA_SETTING_NODE_HPP
#define NANAIRO_CAMERA_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/CameraModel/camera_model.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! PinholeCamera parameters
struct PinholeCameraParameters : public NodeParameters
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  double angle_of_view_ = 45.0;
};

/*!
  */
class CameraSettingNode : public SettingNodeBase
{
 public:
  //! Return the camera type
  CameraType cameraType() const noexcept;

  //! Initialize a node
  virtual void initialize() noexcept override;

  //! Return the jittering mode
  bool jittering() const noexcept;

  //! Return the pinhole camera parameters
  PinholeCameraParameters& pinholeCameraParameters() noexcept;

  //! Return the pinhole camera parameters
  const PinholeCameraParameters& pinholeCameraParameters() const noexcept;

  //! Read the setting data from the stream
  virtual void readData(std::istream* data_stream) noexcept override;

  //! Set the camera type
  void setCameraType(const CameraType type) noexcept;

  //! Set the jittering mode
  void setJittering(const bool jittering) noexcept;

  //! Return the node type
  virtual SettingNodeType type() const noexcept override;

  //! Write the setting data to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept override;

 private:
  std::unique_ptr<NodeParameters> parameters_;
  CameraType type_;
  uint8 jittering_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_CAMERA_SETTING_NODE_HPP
