/*!
  \file test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "test.hpp"
// Standard C++ library
#include <memory>
#include <vector>
// Qt
#include <QString>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/algorithm.hpp"
// Nanairo
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/LinearAlgebra/vector.hpp"
#include "NanairoCore/Utility/scene_settings.hpp"

/*!
  \details
  No detailed.
  */
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<nanairo::System> makeTestSystem(const int image_width,
                                                const int image_height,
                                                const bool is_rgb_rendering,
                                                nanairo::SceneSettings& settings)
{
  using namespace nanairo;

  // Initialize
  // System
  QString prefix{keyword::system};
  auto key = prefix + "/" + keyword::numOfThreads;
  settings.setIntValue(key, 1);
  key = prefix + "/" + keyword::randomSeed;
  settings.setIntValue(key, zisc::toHash32("UnitTest"));
  key = prefix + "/" + keyword::imageWidthResolution;
  settings.setIntValue(key, image_width);
  key = prefix + "/" + keyword::imageHeightResolution;
  settings.setIntValue(key, image_height);
  // Color
  prefix = keyword::color;
  key = prefix + "/" + keyword::rgbRendering;
  settings.setBooleanValue(key, is_rgb_rendering);
  key = prefix + "/" + keyword::gamma;
  settings.setRealValue(key, 2.2);
  key = prefix + "/" + keyword::colorSpace;
  settings.setStringValue(key, QString{keyword::sRgbD65});
  key = prefix + "/" + keyword::standardObserver;
  settings.setStringValue(key, QString{keyword::cie2Deg});
  key = prefix + "/" + keyword::standardIllumination;
  settings.setStringValue(key, QString{keyword::cieD65});

  return std::unique_ptr<System>{new System{settings}};
}
