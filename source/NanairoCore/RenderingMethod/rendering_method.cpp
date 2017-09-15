/*!
  \file rendering_method.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendering_method.hpp"
// Standard C++ library
#include <functional>
#include <limits>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
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
#include "NanairoCore/Utility/unique_pointer.hpp"

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
  \details
  No detailed.
  */
UniquePointer<RenderingMethod> RenderingMethod::makeMethod(
    System& system,
    const SettingNodeBase* settings,
    const Scene& scene) noexcept
{
  ZISC_ASSERT(settings != nullptr, "The setting node is null.");
  ZISC_ASSERT(settings->type() == SettingNodeType::kRenderingMethod,
              "Wrong setting node is specified.");
  const auto method_settings = zisc::cast<const RenderingMethodSettingNode*>(settings);

  RenderingMethod* method = nullptr;
  switch (method_settings->methodType()) {
   case RenderingMethodType::kPathTracing: {
    method = new PathTracing{system, settings, scene};
    break;
   }
   case RenderingMethodType::kLightTracing: {
    method = new LightTracing{system, settings, scene};
    break;
   }
   case RenderingMethodType::kProbabilisticPpm:
    method = new ProbabilisticPpm{system, settings, scene};
    break;
   default: {
    zisc::raiseError("RenderingMethodError: Unsupported type is speficied.");
    break;
   }
  }
  return UniquePointer<RenderingMethod>{method};
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
