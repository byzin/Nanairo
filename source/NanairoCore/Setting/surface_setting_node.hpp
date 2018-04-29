/*!
  \file surface_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SURFACE_SETTING_NODE_HPP
#define NANAIRO_SURFACE_SETTING_NODE_HPP

// Standard C++ library
#include <array>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

//! SmoothDiffuse parameters
struct SmoothDiffuseParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 reflectance_index_ = 0;
};

//! SmoothDielectric parameters
struct SmoothDielectricParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 outer_refractive_index_ = 0;
  uint32 inner_refractive_index_ = 0;
};

//! SmoothConductor parameters
struct SmoothConductorParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 outer_refractive_index_ = 0;
  uint32 inner_refractive_index_ = 0;
  uint32 inner_extinction_index_ = 0;
};

//! RoughDielectric parameters
struct RoughDielectricParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 outer_refractive_index_ = 0;
  uint32 inner_refractive_index_ = 0;
  uint32 roughness_x_index_ = 0;
  uint32 roughness_y_index_ = 0;
  uint8 anisotropic_ = kFalse;
};

//! RoughConductor parameters
struct RoughConductorParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 outer_refractive_index_ = 0;
  uint32 inner_refractive_index_ = 0;
  uint32 inner_extinction_index_ = 0;
  uint32 roughness_x_index_ = 0;
  uint32 roughness_y_index_ = 0;
  uint8 anisotropic_ = kFalse;
};

//! LayeredDiffuse parameters
struct LayeredDiffuseParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  uint32 outer_refractive_index_ = 0;
  uint32 inner_refractive_index_ = 0;
  uint32 reflectance_index_ = 0;
  uint32 roughness_x_index_ = 0;
  uint32 roughness_y_index_ = 0;
  uint8 anisotropic_ = kFalse;
};

//! Cloth parameters
struct ClothParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept;

  std::array<double, 5> coefficients_; // The refractive index, isotropic coefficient, surface gaussian width, volume gaussian width, bandwidth
  uint32 reflectance_index_ = 0;
};

/*!
  */
class SurfaceSettingNode : public SettingNodeBase
{
 public:
  //! Create a surface settings
  SurfaceSettingNode(const SettingNodeBase* parent) noexcept;


  //! Return the cloth parameters
  ClothParameters& clothParameters() noexcept;

  //! Return the cloth parameters
  const ClothParameters& clothParameters() const noexcept;

  //! Initialize a surface setting
  virtual void initialize() noexcept override;

  //! Return the layered diffuse parameters
  LayeredDiffuseParameters& layeredDiffuseParameters() noexcept;

  //! Return the layered diffuse parameters
  const LayeredDiffuseParameters& layeredDiffuseParameters() const noexcept;

  //! Return the surface name
  std::string_view name() const noexcept;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Read the surface setting from the stream
  virtual void readData(std::istream* data_stream) noexcept override;

  //! Return the rough conductor parameters
  RoughConductorParameters& roughConductorParameters() noexcept;

  //! Return the rough conductor parameters
  const RoughConductorParameters& roughConductorParameters() const noexcept;

  //! Return the rough dielectric parameters
  RoughDielectricParameters& roughDielectricParameters() noexcept;

  //! Return the rough dielectric parameters
  const RoughDielectricParameters& roughDielectricParameters() const noexcept;

  //! Set the surface name
  void setName(const std::string_view& name) noexcept;

  //! Set the surface type
  void setSurfaceType(const SurfaceType type) noexcept;

  //! Return the smooth conductor parameters
  SmoothConductorParameters& smoothConductorParameters() noexcept;

  //! Return the smooth conductor parameters
  const SmoothConductorParameters& smoothConductorParameters() const noexcept;

  //! Return the smooth dielectric parameters
  SmoothDielectricParameters& smoothDielectricParameters() noexcept;

  //! Return the smooth dielectric parameters
  const SmoothDielectricParameters& smoothDielectricParameters() const noexcept;

  //! Return the smooth diffuse parameters
  SmoothDiffuseParameters& smoothDiffuseParameters() noexcept;

  //! Return the smooth diffuse parameters
  const SmoothDiffuseParameters& smoothDiffuseParameters() const noexcept;

  //! Return the surface type
  SurfaceType surfaceType() const noexcept;

  //! Return the node type
  virtual SettingNodeType type() const noexcept override;

  //! Write the surface setting to the stream
  virtual void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::UniqueMemoryPointer<NodeParameterBase> parameters_;
  zisc::pmr::string name_;
  SurfaceType surface_type_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_SURFACE_SETTING_NODE_HPP
