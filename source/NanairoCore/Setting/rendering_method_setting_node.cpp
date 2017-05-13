/*!
  \file rendering_method_setting_node.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendering_method_setting_node.hpp"
// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Zisc
#include "zisc/error.hpp"
#include "zisc/binary_data.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"

namespace nanairo {

/*!
  */
void ProbabilisticPpmParameters::readData(std::istream* data_stream) noexcept
{
  zisc::read(&photon_search_radius_, data_stream);
  zisc::read(&radius_reduction_rate_, data_stream);
  zisc::read(&num_of_photons_, data_stream);
  zisc::read(&k_nearest_neighbor_, data_stream);
}

/*!
  */
void ProbabilisticPpmParameters::writeData(std::ostream* data_stream) const noexcept
{
  zisc::write(&photon_search_radius_, data_stream);
  zisc::write(&radius_reduction_rate_, data_stream);
  zisc::write(&num_of_photons_, data_stream);
  zisc::write(&k_nearest_neighbor_, data_stream);
}

/*!
  */
void RenderingMethodSettingNode::initialize() noexcept
{
  setMethodType(RenderingMethodType::kPathTracing);
  setRayCastEpsilon(0.0000001);
  setRouletteType(RouletteType::kMaxWeight);
  setRoulettePathLength(3);
}

/*!
  */
void RenderingMethodSettingNode::log() const noexcept
{
  using zisc::cast;
  zisc::logMessage("Rendering method: ", cast<uint>(methodType()));
  zisc::logMessage("Ray cast epsilon: ", rayCastEpsilon());
  zisc::logMessage("Russian roulette: ", cast<uint>(rouletteType()));
}

/*!
  */
ProbabilisticPpmParameters&
RenderingMethodSettingNode::probabilisticPpmParameters() noexcept
{
  ZISC_ASSERT(methodType() == RenderingMethodType::kProbabilisticPpm,
              "Invalid method type is specified.");
  auto parameter = zisc::cast<ProbabilisticPpmParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
const ProbabilisticPpmParameters&
RenderingMethodSettingNode::probabilisticPpmParameters() const noexcept
{
  ZISC_ASSERT(methodType() == RenderingMethodType::kProbabilisticPpm,
              "Invalid method type is specified.");
  auto parameter = zisc::cast<const ProbabilisticPpmParameters*>(parameters_.get());
  return *parameter;
}

/*!
  */
double RenderingMethodSettingNode::rayCastEpsilon() const noexcept
{
  return ray_cast_epsilon_;
}

/*!
  */
void RenderingMethodSettingNode::readData(std::istream* data_stream) noexcept
{
  {
    zisc::read(&method_type_, data_stream);
    setMethodType(method_type_);
  }
  zisc::read(&ray_cast_epsilon_, data_stream);
  zisc::read(&roulette_type_, data_stream);
  zisc::read(&roulette_path_length_, data_stream);
  if (parameters_)
    parameters_->readData(data_stream);
}

/*!
  */
uint32 RenderingMethodSettingNode::roulettePathLength() const noexcept
{
  return roulette_path_length_;
}

/*!
  */
RouletteType RenderingMethodSettingNode::rouletteType() const noexcept
{
  return roulette_type_;
}

/*!
  */
RenderingMethodType RenderingMethodSettingNode::methodType() const noexcept
{
  return method_type_;
}

/*!
  */
void RenderingMethodSettingNode::setMethodType(const RenderingMethodType method_type)
    noexcept
{
  method_type_ = method_type;
  // Initialize parameters
  parameters_.reset(nullptr);
  switch (method_type_) {
   case RenderingMethodType::kProbabilisticPpm: {
    parameters_ = std::make_unique<ProbabilisticPpmParameters>();
    break;
   }
   case RenderingMethodType::kPathTracing:
   case RenderingMethodType::kLightTracing:
   default:
    break;
  }
}

/*!
  */
void RenderingMethodSettingNode::setRoulettePathLength(const uint32 path_length) noexcept
{
  ZISC_ASSERT(path_length != 0, "The roulette path length is zero.");
  roulette_path_length_ = path_length;
}

/*!
  */
void RenderingMethodSettingNode::setRouletteType(const RouletteType roulette_type)
    noexcept
{
  roulette_type_ = roulette_type;
}

/*!
  */
void RenderingMethodSettingNode::setRayCastEpsilon(const double ray_cast_epsilon)
    noexcept
{
  ray_cast_epsilon_ = ray_cast_epsilon;
}

/*!
  */
SettingNodeType RenderingMethodSettingNode::type() const noexcept
{
  return SettingNodeType::kRenderingMethod;
}

/*!
  */
void RenderingMethodSettingNode::writeData(std::ostream* data_stream)
    const noexcept
{
  writeType(data_stream);
  // Write properties
  zisc::write(&method_type_, data_stream);
  zisc::write(&ray_cast_epsilon_, data_stream);
  zisc::write(&roulette_type_, data_stream);
  zisc::write(&roulette_path_length_, data_stream);
  if (parameters_)
    parameters_->writeData(data_stream);
}

} // namespace nanairo
