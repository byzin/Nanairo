/*!
  \file transformation_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <tuple>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/vector.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"

TEST(TransformationTest, DefaultTangentTest)
{
  using nanairo::Transformation;
  using nanairo::uint;

  const nanairo::Vector3 normal{0.0, 0.0, 1.0};
  const auto tangents = Transformation::calcDefaultTangent(normal);

  const auto& tangent = std::get<0>(tangents);
  const auto& bitangent = std::get<1>(tangents);

  ASSERT_TRUE(nanairo::isUnitVector(tangent))
      << "The tangent vector isn't unit vector.";
  ASSERT_TRUE(nanairo::isUnitVector(bitangent))
      << "The bitangent vector isn't unit vector.";

  const auto n = zisc::cross(tangent, bitangent);
  for (uint i = 0; i < 3; ++i)
    ASSERT_DOUBLE_EQ(normal[i], n[i]) << "The calculation of tangents are wrong.";
}
