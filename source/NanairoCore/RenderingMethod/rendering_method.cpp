/*!
  \file rendering_method.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "rendering_method.hpp"
// Standard C++ library
#include <functional>
#include <limits>
#include <tuple>
#include <utility>
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/math.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
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
                                 const QJsonObject& settings) noexcept :
    clear_function_{},
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
    const QJsonObject& settings) noexcept
{
  using zisc::toHash32;

  RenderingMethod* method = nullptr;
  const auto type = SceneValue::toString(settings, keyword::type);
  switch (keyword::toHash32(type)) {
   case toHash32(keyword::pathTracing): {
    method = new PathTracing{system, settings};
    break;
   }
   case toHash32(keyword::lightTracing): {
    method = new LightTracing{system, settings};
    break;
   }
   case toHash32(keyword::probabilisticPpm):
    method = new ProbabilisticPpm{system, settings};
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
void RenderingMethod::initialize(const QJsonObject& settings) noexcept
{
  // Ray cast epsilon
  ray_cast_epsilon_ = SceneValue::toFloat<Float>(settings, keyword::rayCastEpsilon);
  ZISC_ASSERT(0.0 < ray_cast_epsilon_, "Ray cast epsilon is negative.");
}

} // namespace nanairo
