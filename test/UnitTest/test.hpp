/*!
  \file test.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_TEST_HPP
#define NANAIRO_TEST_HPP

// Standard C++ library
#include <memory>
#include <vector>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"


//! Make a system
std::unique_ptr<nanairo::System> makeTestSystem(const int image_width,
                                                const int image_height,
                                                const bool is_rgb_rendering,
                                                nanairo::SceneSettings& settings);

#endif // NANAIRO_TEST_HPP
