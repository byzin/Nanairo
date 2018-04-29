/*!
  \file rendering_method.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendering_method.hpp"
// Standard C++ library
#include <functional>
#include <limits>
#include <memory>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "path_tracing.hpp"
#include "light_tracing.hpp"
#include "probabilistic_ppm.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/world.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/bvh.hpp"
#include "NanairoCore/Sampling/russian_roulette.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"
#include "NanairoCore/Sampling/sampler.hpp"
#include "NanairoCore/Setting/rendering_method_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class Scene;
class System;

/*!
  \details
  No detailed.
  */
RenderingMethod::RenderingMethod(const System& /* system */,
                                 const SettingNodeBase* settings) noexcept :
    russian_roulette_{settings},
    ray_cast_epsilon_{0.0}
{
  initialize(settings);
}

/*!
  */
void RenderingMethod::initMethod() noexcept
{
}

/*!
  \details
  No detailed.
  */
zisc::UniqueMemoryPointer<RenderingMethod> RenderingMethod::makeMethod(
    System& system,
    const SettingNodeBase* settings,
    const Scene& scene) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);

  zisc::UniqueMemoryPointer<RenderingMethod> method;
  auto data_resource = &system.dataMemoryManager();
  switch (method_settings->methodType()) {
   case RenderingMethodType::kPathTracing: {
    method = zisc::UniqueMemoryPointer<PathTracing>::make(data_resource,
                                                          system,
                                                          settings,
                                                          scene);
    break;
   }
   case RenderingMethodType::kLightTracing: {
    method = zisc::UniqueMemoryPointer<LightTracing>::make(data_resource,
                                                           system,
                                                           settings,
                                                           scene);
    break;
   }
   case RenderingMethodType::kProbabilisticPpm:
    method = zisc::UniqueMemoryPointer<ProbabilisticPpm>::make(data_resource,
                                                               system,
                                                               settings, scene);
    break;
   default: {
    zisc::raiseError("RenderingMethodError: Unsupported type is speficied.");
    break;
   }
  }
  return method;
}

/*!
  \details
  No detailed.
  */
void RenderingMethod::initialize(const SettingNodeBase* settings) noexcept
{
  const auto method_settings = castNode<RenderingMethodSettingNode>(settings);

  {
    ray_cast_epsilon_ = zisc::cast<Float>(method_settings->rayCastEpsilon());
    ZISC_ASSERT(0.0 < ray_cast_epsilon_, "Ray cast epsilon is negative.");
  }
}

} // namespace nanairo
