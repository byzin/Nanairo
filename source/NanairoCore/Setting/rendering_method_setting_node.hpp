/*!
  \file rendering_method_setting_node.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef RENDERING_METHOD_SETTING_NODE_HPP
#define RENDERING_METHOD_SETTING_NODE_HPP

// Standard C++ library
#include <istream>
#include <memory>
#include <ostream>
// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "setting_node_base.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/LightSourceSampler/light_source_sampler.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

// PathTracing parameters
struct PathTracingParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  LightSourceSamplerType eye_path_light_sampler_type_ =
      LightSourceSamplerType::kPowerWeighted;
};

// LightTracing parameters
struct LightTracingParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  LightSourceSamplerType light_path_light_sampler_type_ =
      LightSourceSamplerType::kPowerWeighted;
};

//! ProbabilisticPPM parameters
struct ProbabilisticPpmParameters : public NodeParameterBase
{
  //! Read the parameters from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Write the parameters to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

  double photon_search_radius_ = 0.02;
  double radius_reduction_rate_ = 2.0 / 3.0;
  uint32 num_of_photons_ = 131072;
  uint32 k_nearest_neighbor_ = 8;
  LightSourceSamplerType light_path_light_sampler_type_ =
      LightSourceSamplerType::kPowerWeighted;
};

/*!
  */
class RenderingMethodSettingNode : public SettingNodeBase
{
 public:
  //! Create a rendering method settings
  RenderingMethodSettingNode(const SettingNodeBase* parent) noexcept;


  //! Initialize a rendering method
  void initialize() noexcept override;

  //! Return the LightTracing parameters
  LightTracingParameters& lightTracingParameters() noexcept;

  //! Return the LightTracing parameters
  const LightTracingParameters& lightTracingParameters() const noexcept;

  //! Return the node type
  static SettingNodeType nodeType() noexcept;

  //! Return the PathTracing parameters
  PathTracingParameters& pathTracingParameters() noexcept;

  //! Return the PathTracing parameters
  const PathTracingParameters& pathTracingParameters() const noexcept;

  //! Return the ProbabilisticPPM parameters
  ProbabilisticPpmParameters& probabilisticPpmParameters() noexcept;

  //! Return the ProbabilisticPPM parameters
  const ProbabilisticPpmParameters& probabilisticPpmParameters() const noexcept;

  //! Return the ray cast epsilon
  double rayCastEpsilon() const noexcept;

  //! Read the setting data from the stream
  void readData(std::istream* data_stream) noexcept override;

  //! Return the roulette path length
  uint32 roulettePathLength() const noexcept;

  //! Return the russian roulette type
  RouletteType rouletteType() const noexcept;

  //! Return the rendering method type
  RenderingMethodType methodType() const noexcept;

  //! Set the rendering method type
  void setMethodType(const RenderingMethodType method_type) noexcept;

  //! Set the roulette path length
  void setRoulettePathLength(const uint32 path_length) noexcept;

  //! Set the russian roulette type
  void setRouletteType(const RouletteType roulette_type) noexcept;

  //! Set the ray cast epsilon
  void setRayCastEpsilon(const double ray_cast_epsilon) noexcept;

  //! Return the setting node type
  SettingNodeType type() const noexcept override;

  //! Write the setting data to the stream
  void writeData(std::ostream* data_stream) const noexcept override;

 private:
  zisc::UniqueMemoryPointer<NodeParameterBase> parameters_;
  RenderingMethodType method_type_;
  double ray_cast_epsilon_;
  RouletteType roulette_type_;
  uint32 roulette_path_length_;
};

//! \} Core

} // namespace nanairo

#endif // RENDERING_METHOD_SETTING_NODE_HPP
