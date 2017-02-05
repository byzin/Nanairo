/*!
  \file system.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "system.hpp"
// Standard C++ library
#include <vector>
// Qt
#include <QJsonObject>
#include <QtGlobal>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/thread_pool.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "Color/xyz_color_matching_function.hpp"
#include "NanairoCommon/keyword.hpp"
#include "Sampling/sampler.hpp"
#include "Utility/unique_pointer.hpp"
#include "Utility/scene_value.hpp"
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
System::System(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
System::~System() noexcept
{
}

/*!
  \details
  No detailed.
  */
void System::initialize(const QJsonObject& settings) noexcept
{
  using zisc::cast;

  // Thread pool
  auto initialize_system = [this](const QJsonObject& node)
  {
    // Threading
    const auto num_of_threads = SceneValue::toInt<uint>(node,
                                                        keyword::numOfThreads);
    thread_pool_ = new zisc::ThreadPool{num_of_threads};
    qInfo("  Threads: %d", num_of_threads);

    // Random number generator
    const auto random_seed = SceneValue::toInt<uint64>(node,
                                                       keyword::randomSeed);
    sampler_list_.reserve(num_of_threads + 1);
    for (uint i = 0; i < (num_of_threads + 1); ++i)
      sampler_list_.emplace_back(random_seed + cast<uint64>(i));

    // Memory pool
    memory_pool_list_.resize(num_of_threads + 1);

    // Image resolution
    const auto image_resolution = SceneValue::toArray(node,
                                                      keyword::imageResolution);
    image_width_resolution_ = SceneValue::toInt<uint>(image_resolution[0]);
    image_height_resolution_ = SceneValue::toInt<uint>(image_resolution[1]);
  };
  const auto system_settings = SceneValue::toObject(settings, keyword::system);
  initialize_system(system_settings);

  // Color
  auto initialize_color_system = [this](const QJsonObject& node)
  {
    // RGB rendering
    is_rgb_rendering_mode_ =
        (SceneValue::toString(node, keyword::colorMode) == keyword::rgb);
    qInfo("  RGB rendering mode: %d", is_rgb_rendering_mode_);
    // Color space
    color_space_ = keyword::toHash32(SceneValue::toString(node,
                                                          keyword::colorSpace));
    gamma_ = SceneValue::toFloat<Float>(node, keyword::gamma);
    qInfo("  Gamma: %f", zisc::cast<float>(gamma_));
    // Color matching function
    xyz_color_matching_function_ = new XyzColorMatchingFunction{node};
  };
  const auto color_settings = SceneValue::toObject(settings, keyword::color);
  initialize_color_system(color_settings);
}

} // namespace nanairo
