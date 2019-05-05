/*!
  \file surface_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2019 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "surface_setting_node.hpp"
// Standard C++ library
#include <array>
#include <istream>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
// Zisc
#include "zisc/binary_data.hpp"
#include "zisc/error.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Material/SurfaceModel/surface_model.hpp"

namespace nanairo {

/*!
  */
void SmoothDiffuseParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&reflectance_index_, data_stream);
}

/*!
  */
void SmoothDiffuseParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&reflectance_index_, data_stream);
}

/*!
  */
void SmoothDielectricParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&outer_refractive_index_, data_stream);
  zisc::read(&inner_refractive_index_, data_stream);
}

/*!
  */
void SmoothDielectricParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&outer_refractive_index_, data_stream);
  zisc::write(&inner_refractive_index_, data_stream);
}

/*!
  */
void SmoothConductorParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&outer_refractive_index_, data_stream);
  zisc::read(&inner_refractive_index_, data_stream);
  zisc::read(&inner_extinction_index_, data_stream);
}

/*!
  */
void SmoothConductorParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&outer_refractive_index_, data_stream);
  zisc::write(&inner_refractive_index_, data_stream);
  zisc::write(&inner_extinction_index_, data_stream);
}

/*!
  */
void RoughDielectricParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&outer_refractive_index_, data_stream);
  zisc::read(&inner_refractive_index_, data_stream);
  zisc::read(&roughness_x_index_, data_stream);
  zisc::read(&roughness_y_index_, data_stream);
  zisc::read(&anisotropic_, data_stream);
}

/*!
  */
void RoughDielectricParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&outer_refractive_index_, data_stream);
  zisc::write(&inner_refractive_index_, data_stream);
  zisc::write(&roughness_x_index_, data_stream);
  zisc::write(&roughness_y_index_, data_stream);
  zisc::write(&anisotropic_, data_stream);
}

/*!
  */
void RoughConductorParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&outer_refractive_index_, data_stream);
  zisc::read(&inner_refractive_index_, data_stream);
  zisc::read(&inner_extinction_index_, data_stream);
  zisc::read(&roughness_x_index_, data_stream);
  zisc::read(&roughness_y_index_, data_stream);
  zisc::read(&anisotropic_, data_stream);
}

/*!
  */
void RoughConductorParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&outer_refractive_index_, data_stream);
  zisc::write(&inner_refractive_index_, data_stream);
  zisc::write(&inner_extinction_index_, data_stream);
  zisc::write(&roughness_x_index_, data_stream);
  zisc::write(&roughness_y_index_, data_stream);
  zisc::write(&anisotropic_, data_stream);
}

/*!
  */
void LayeredDiffuseParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&outer_refractive_index_, data_stream);
  zisc::read(&inner_refractive_index_, data_stream);
  zisc::read(&reflectance_index_, data_stream);
  zisc::read(&roughness_x_index_, data_stream);
  zisc::read(&roughness_y_index_, data_stream);
  zisc::read(&anisotropic_, data_stream);
}

/*!
  */
void LayeredDiffuseParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&outer_refractive_index_, data_stream);
  zisc::write(&inner_refractive_index_, data_stream);
  zisc::write(&reflectance_index_, data_stream);
  zisc::write(&roughness_x_index_, data_stream);
  zisc::write(&roughness_y_index_, data_stream);
  zisc::write(&anisotropic_, data_stream);
}

/*!
  */
void ClothParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&coefficients_[0], data_stream, sizeof(double) * coefficients_.size());
  zisc::read(&reflectance_index_, data_stream);
}

/*!
  */
void ClothParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&coefficients_[0], data_stream, sizeof(double) * coefficients_.size());
  zisc::write(&reflectance_index_, data_stream);
}

/*!
  */
SurfaceSettingNode::SurfaceSettingNode(const SettingNodeBase* parent) noexcept :
    SettingNodeBase(parent),
    name_{dataResource()}
{
}

/*!
  */
ClothParameters& SurfaceSettingNode::clothParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kCloth,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<ClothParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const ClothParameters& SurfaceSettingNode::clothParameters() const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kCloth,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const ClothParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
void SurfaceSettingNode::initialize() noexcept
{
  setName("SurfaceModel");
  setSurfaceType(SurfaceType::kSmoothDiffuse);
}

/*!
  */
LayeredDiffuseParameters& SurfaceSettingNode::layeredDiffuseParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kLayeredDiffuse,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<LayeredDiffuseParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const LayeredDiffuseParameters& SurfaceSettingNode::layeredDiffuseParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kLayeredDiffuse,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const LayeredDiffuseParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
std::string_view SurfaceSettingNode::name() const noexcept
{
  return std::string_view{name_};
}

/*!
  */
SettingNodeType SurfaceSettingNode::nodeType() noexcept
{
  return SettingNodeType::kSurface;
}

/*!
  */
void SurfaceSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    name_ = readString(data_stream);
  }
  {
    zisc::read(&surface_type_, data_stream);
    setSurfaceType(surface_type_);
  }
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
RoughConductorParameters& SurfaceSettingNode::roughConductorParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kRoughConductor,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<RoughConductorParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const RoughConductorParameters& SurfaceSettingNode::roughConductorParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kRoughConductor,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const RoughConductorParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
RoughDielectricParameters& SurfaceSettingNode::roughDielectricParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kRoughDielectric,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<RoughDielectricParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const RoughDielectricParameters& SurfaceSettingNode::roughDielectricParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kRoughDielectric,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const RoughDielectricParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
void SurfaceSettingNode::setName(const std::string_view& name) noexcept
{
  name_ = name;
}

/*!
  */
void SurfaceSettingNode::setSurfaceType(const SurfaceType type) noexcept
{
  surface_type_ = type;
  // Initialize parameters
  parameters_.reset();
  switch (surface_type_) {
   case SurfaceType::kSmoothDiffuse: {
    parameters_ =
        zisc::UniqueMemoryPointer<SmoothDiffuseParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kSmoothDielectric: {
    parameters_ =
        zisc::UniqueMemoryPointer<SmoothDielectricParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kSmoothConductor: {
    parameters_ =
        zisc::UniqueMemoryPointer<SmoothConductorParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kRoughDielectric: {
    parameters_ =
        zisc::UniqueMemoryPointer<RoughDielectricParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kRoughConductor: {
    parameters_ =
        zisc::UniqueMemoryPointer<RoughConductorParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kLayeredDiffuse: {
    parameters_ =
        zisc::UniqueMemoryPointer<LayeredDiffuseParameters>::make(dataResource());
    break;
   }
   case SurfaceType::kCloth: {
    parameters_ =
        zisc::UniqueMemoryPointer<ClothParameters>::make(dataResource());
    break;
   }
   default:
    break;
  }
}

/*!
  */
SmoothConductorParameters& SurfaceSettingNode::smoothConductorParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothConductor,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<SmoothConductorParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const SmoothConductorParameters& SurfaceSettingNode::smoothConductorParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothConductor,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const SmoothConductorParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
SmoothDielectricParameters& SurfaceSettingNode::smoothDielectricParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothDielectric,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<SmoothDielectricParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const SmoothDielectricParameters& SurfaceSettingNode::smoothDielectricParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothDielectric,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const SmoothDielectricParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
SmoothDiffuseParameters& SurfaceSettingNode::smoothDiffuseParameters() noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothDiffuse,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<SmoothDiffuseParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
const SmoothDiffuseParameters& SurfaceSettingNode::smoothDiffuseParameters()
    const noexcept
{
  ZISC_ASSERT(surfaceType() == SurfaceType::kSmoothDiffuse,
              "Invalid surface type is specified.");
  auto parameters = zisc::cast<const SmoothDiffuseParameters*>(parameters_.get());
  return *parameters;
}

/*!
  */
SurfaceType SurfaceSettingNode::surfaceType() const noexcept
{
  return surface_type_;
}

/*!
  */
SettingNodeType SurfaceSettingNode::type() const noexcept
{
  return SettingNodeType::kSurface;
}

/*!
  */
void SurfaceSettingNode::writeData(std::ostream* data_stream) const noexcept
{
  writeType(data_stream);
  writeString(name(), data_stream);
  zisc::write(&surface_type_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
