/*!
  \file shape_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// GoogleTest
#include "gtest/gtest.h"
// Standard C++ library
#include <array>
#include <cmath>
#include <memory>
// Zisc
#include "zisc/unit.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Shape/plane.hpp"
#include "NanairoCore/Shape/flat_triangle.hpp"

TEST(ShapeTest, PlaneTransformationTest)
{
  using nanairo::uint;
  using nanairo::Float;
  using nanairo::Transformation;
  using nanairo::Vector3;
  using nanairo::Point3;

  const Point3 default_p{-0.5, -0.5, 0.0};
  const std::array<Vector3, 2> default_e{{Vector3{1.0, 0.0, 0.0},
                                          Vector3{0.0, 1.0, 0.0}}};
  const Vector3 default_n{0.0, 0.0, 1.0};
  const Float default_area = 1.0;

  // Translation
  {
    auto shape = std::make_unique<nanairo::Plane>();

    const Vector3 v{0.5, 1.0, -1.0};
    const auto transformation = Transformation::makeTranslation(v[0], v[1], v[2]);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_p[i] + v[i], p[i])
            << "The translation of the shape is failed.";
      }
    }
    {
      const auto& e = shape->edge();
      for (uint axis = 0; axis < 2; ++axis) {
        for (uint i = 0; i < 3; ++i) {
          ASSERT_DOUBLE_EQ(default_e[axis][i], e[axis][i])
              << "The translation of the plane is failed.";
        }
      }
    }
    {
      const auto& n = shape->normal();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_n[i], n[i])
            << "The translation of the plane is failed.";
      }
    }
    ASSERT_DOUBLE_EQ(default_area, shape->surfaceArea())
        << "The translation of the plane is failed.";
  }
  // Scaling
  {
    auto shape = std::make_unique<nanairo::Plane>();

    const Vector3 v{0.5, 0.1, 1.0};
    const auto transformation = Transformation::makeScaling(v[0], v[1], v[2]);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_p[i] * v[i], p[i])
          << "The scaling of the plane is failed.";
      }
    }
    {
      const auto& e = shape->edge();
      for (uint axis = 0; axis < 2; ++axis) {
        for (uint i = 0; i < 3; ++i) {
          ASSERT_DOUBLE_EQ(default_e[axis][i] * v[i], e[axis][i])
              << "The scaling of the plane is failed.";
        }
      }
    }
    {
      const auto& n = shape->normal();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_n[i], n[i])
            << "The scaling of the plane is failed.";
      }
    }
    ASSERT_DOUBLE_EQ(default_area * v[0] * v[1] * v[2], shape->surfaceArea())
        << "The scaling of the plane is failed.";
  }
  // Rotation
  {
    auto shape = std::make_unique<nanairo::Plane>();

    const Float theta = zisc::toRadian(30.0);
    const auto transformation = Transformation::makeXAxisRotation(theta);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      ASSERT_DOUBLE_EQ(default_p[0], p[0])
            << "The rotation of the plane is failed.";
      ASSERT_DOUBLE_EQ(default_p[1] * std::cos(theta), p[1])
            << "The rotation of the plane is failed.";
      ASSERT_DOUBLE_EQ(default_p[1] * std::sin(theta), p[2])
            << "The rotation of the plane is failed.";
    }
    {
      const auto& n = shape->normal();
      ASSERT_DOUBLE_EQ(default_n[0], n[0])
          << "The rotation of the plane is failed.";
      ASSERT_DOUBLE_EQ(-default_n[2] * std::sin(theta), n[1])
          << "The rotation of the plane is failed.";
      ASSERT_DOUBLE_EQ(default_n[2] * std::cos(theta), n[2])
          << "The rotation of the plane is failed.";
    }
    ASSERT_DOUBLE_EQ(default_area, shape->surfaceArea())
        << "The translation of the plane is failed.";
  }
}

TEST(ShapeTest, FlatTriangleTransformationTest)
{
  using nanairo::uint;
  using nanairo::Float;
  using nanairo::Transformation;
  using nanairo::Vector3;
  using nanairo::Point3;

  const Point3 default_p{-0.5, -0.5, 0.0};
  const std::array<Vector3, 2> default_e{{Vector3{1.0, 0.0, 0.0},
                                          Vector3{0.0, 1.0, 0.0}}};
  const Vector3 default_n{0.0, 0.0, 1.0};
  const Float default_area = 0.5;

  auto make_triangle = [&default_p, &default_e]()
  {
    return std::make_unique<nanairo::FlatTriangle>(default_p,
                                                   default_p + default_e[0],
                                                   default_p + default_e[1]);
  };

  // Translation
  {
    auto shape = make_triangle();

    const Vector3 v{0.5, 1.0, -1.0};
    const auto transformation = Transformation::makeTranslation(v[0], v[1], v[2]);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_p[i] + v[i], p[i])
            << "The translation of the triangle is failed.";
      }
    }
    {
      const auto& e = shape->edge();
      for (uint axis = 0; axis < 2; ++axis) {
        for (uint i = 0; i < 3; ++i) {
          ASSERT_DOUBLE_EQ(default_e[axis][i], e[axis][i])
              << "The translation of the triangle is failed.";
        }
      }
    }
    {
      const auto& n = shape->normal();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_n[i], n[i])
            << "The translation of the triangle is failed.";
      }
    }
    ASSERT_DOUBLE_EQ(default_area, shape->surfaceArea())
        << "The translation of the triangle is failed.";
  }
  // Scaling
  {
    auto shape = make_triangle();

    const Vector3 v{0.5, 0.1, 1.0};
    const auto transformation = Transformation::makeScaling(v[0], v[1], v[2]);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_p[i] * v[i], p[i])
          << "The scaling of the triangle is failed.";
      }
    }
    {
      const auto& e = shape->edge();
      for (uint axis = 0; axis < 2; ++axis) {
        for (uint i = 0; i < 3; ++i) {
          ASSERT_DOUBLE_EQ(default_e[axis][i] * v[i], e[axis][i])
              << "The scaling of the triangle is failed.";
        }
      }
    }
    {
      const auto& n = shape->normal();
      for (uint i = 0; i < 3; ++i) {
        ASSERT_DOUBLE_EQ(default_n[i], n[i])
            << "The scaling of the triangle is failed.";
      }
    }
    ASSERT_DOUBLE_EQ(default_area * v[0] * v[1] * v[2], shape->surfaceArea())
        << "The scaling of the triangle is failed.";
  }
  // Rotation
  {
    auto shape = make_triangle();

    const Float theta = zisc::toRadian(30.0);
    const auto transformation = Transformation::makeXAxisRotation(theta);

    shape->transform(transformation);
    {
      const auto& p = shape->vertex0();
      ASSERT_DOUBLE_EQ(default_p[0], p[0])
            << "The rotation of the triangle is failed.";
      ASSERT_DOUBLE_EQ(default_p[1] * std::cos(theta), p[1])
            << "The rotation of the triangle is failed.";
      ASSERT_DOUBLE_EQ(default_p[1] * std::sin(theta), p[2])
            << "The rotation of the triangle is failed.";
    }
    {
      const auto& n = shape->normal();
      ASSERT_DOUBLE_EQ(default_n[0], n[0])
          << "The rotation of the triangle is failed.";
      ASSERT_DOUBLE_EQ(-default_n[2] * std::sin(theta), n[1])
          << "The rotation of the triangle is failed.";
      ASSERT_DOUBLE_EQ(default_n[2] * std::cos(theta), n[2])
          << "The rotation of the triangle is failed.";
    }
    ASSERT_DOUBLE_EQ(default_area, shape->surfaceArea())
        << "The translation of the triangle is failed.";
  }
}
