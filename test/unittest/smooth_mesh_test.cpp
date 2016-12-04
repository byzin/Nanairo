/*!
  \file smoothed_mesh_test.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

// Standard C++ library
#include <array>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
// GoogleTest
#include "gtest/gtest.h"
// Zisc
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/aabb.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Shape/smoothed_mesh.hpp"
// Test
#include "test.hpp"

namespace  {

using namespace nanairo;

struct TriangleData
{
  TriangleData(const Point3& v1, const Point3& v2, const Point3& v3,
               const Vector3& n1, const Vector3& n2, const Vector3& n3) :
      vertices_{{v1, v2, v3}},
      normals_{{n1, n2, n3}}
  {
  }

  std::array<Point3, 3> vertices_;
  std::array<Vector3, 3> normals_;
};

std::vector<TriangleData> getTriangleDataList()
{
  std::vector<TriangleData> triangle_data_list;
  {
    const Point3 v1{0.0, 0.96194, -0.191342};
    const Point3 v2{0.135299, 0.853553, -0.326641};
    const Point3 v3{0.0, 0.853553, -0.353553};
    const auto n1 = Vector3{0.0, 0.9152, -0.4031}.normalized();
    const auto n2 = Vector3{0.2751, 0.6952, -0.6641}.normalized();
    const auto n3 = Vector3{0.0, 0.6952, -0.7188}.normalized();
    triangle_data_list.emplace_back(v1, v2, v3, n1, n2, n3);
  }
  {
    const Point3 v1{0.326641, 0.691342, -0.326641};
    const Point3 v2{0.461940, 0.5, -0.191342};
    const Point3 v3{0.353553, 0.5, -0.353553};
    const auto n1 = Vector3{0.6557, 0.3743, -0.6557}.normalized();
    const auto n2 = Vector3{0.9239, 0.0, -0.3827}.normalized();
    const auto n3 = Vector3{0.7071, 0.0, -0.7071}.normalized();
    triangle_data_list.emplace_back(v1, v2, v3, n1, n2, n3);
  }
  {
    const Point3 v1{0.499732, 0.1, -0.01636};
    const Point3 v2{0.498877, 0.113053, -0.016332};
    const Point3 v3{0.499145, 0.113053, 0.0};
    const auto n1 = Vector3{0.9995, 0.0, -0.0327}.normalized();
    const auto n2 = Vector3{0.9909, 0.1304, -0.0324}.normalized();
    const auto n3 = Vector3{0.9915, 0.1304, 0.0}.normalized();
    triangle_data_list.emplace_back(v1, v2, v3, n1, n2, n3);
  }
  {
    const Point3 v1{0.0, 1.0, -0.5};
    const Point3 v2{0.012271, 0.0, -0.499849};
    const Point3 v3{0.0, 0.0, -0.5};
    const auto n1 = Vector3{0.0, 0.0, -1.0}.normalized();
    const auto n2 = Vector3{0.0245, 0.0, -0.9997}.normalized();
    const auto n3 = Vector3{0.0, 0.0, -1.0}.normalized();
    triangle_data_list.emplace_back(v1, v2, v3, n1, n2, n3);
  }
  {
    const Point3 v1{-0.073366, 0.005412, 0.0};
    const Point3 v2{-0.073278, 0.005412, -0.0036};
    const Point3 v3{-0.048950, 0.002408, -0.002405};
    const auto n1 = Vector3{-0.1471, -0.9891, 0.0}.normalized();
    const auto n2 = Vector3{-0.1469, -0.9891, -0.0072}.normalized();
    const auto n3 = Vector3{-0.0983, -0.9951, -0.0048}.normalized();
    triangle_data_list.emplace_back(v1, v2, v3, n1, n2, n3);
  }

  return triangle_data_list;
}

} // namespace

TEST(SmoothedMeshTest, VertexTest)
{
  const auto triangle_data_list = ::getTriangleDataList();
  const int n = static_cast<int>(triangle_data_list.size());
  for (int index = 0; index < n; ++index) {
    const auto& data = triangle_data_list[index];

    const auto& v1 = data.vertices_[0];
    const auto& v2 = data.vertices_[1];
    const auto& v3 = data.vertices_[2];
    const auto& n1 = data.normals_[0];
    const auto& n2 = data.normals_[1];
    const auto& n3 = data.normals_[2];

    auto mesh = std::make_unique<SmoothedMesh>(v1, v2, v3, n1, n2, n3);

    // Vertex1
    const auto vertex1 = mesh->vertex1();
    for (int i = 0; i < 3; ++i) {
      EXPECT_DOUBLE_EQ(v1[i], vertex1[i])
          << "The v1 of triangle[" << index << "] is wrong: "
          << "v1 = (" << v1[0] << ", " << v1[1] << ", " << v1[2] << "), "
          << "vs1 = (" << vertex1[0] << ", " << vertex1[1] << ", " << vertex1[2] << ");";
    }
    EXPECT_EQ(vertex1.data(), mesh->point(1.0, 0.0).data());
    // Vertex2
    const auto vertex2 = mesh->vertex2();
    for (int i = 0; i < 3; ++i) {
      EXPECT_DOUBLE_EQ(v2[i], vertex2[i])
          << "The v2 of triangle[" << index << "] is wrong: "
          << "v2 = (" << v2[0] << ", " << v2[1] << ", " << v2[2] << "), "
          << "vs2 = (" << vertex2[0] << ", " << vertex2[1] << ", " << vertex2[2] << ");";
    }
    EXPECT_EQ(vertex2.data(), mesh->point(0.0, 1.0).data());
    // Vertex2
    const auto vertex3 = mesh->vertex3();
    for (int i = 0; i < 3; ++i) {
      EXPECT_DOUBLE_EQ(v3[i], vertex3[i])
          << "The v3 of triangle[" << index << "] is wrong: "
          << "v3 = (" << v3[0] << ", " << v3[1] << ", " << v3[2] << "), "
          << "vs1 = (" << vertex3[0] << ", " << vertex3[1] << ", " << vertex3[2] << ");";
    }
    EXPECT_EQ(vertex3.data(), mesh->point(0.0, 0.0).data());
  }
}

TEST(SmoothedMeshTest, NormalTest)
{
  const auto triangle_data_list = ::getTriangleDataList();
  const int n = static_cast<int>(triangle_data_list.size());
  for (int index = 0; index < n; ++index) {
    const auto& data = triangle_data_list[index];

    const auto& v1 = data.vertices_[0];
    const auto& v2 = data.vertices_[1];
    const auto& v3 = data.vertices_[2];
    const auto& n1 = data.normals_[0];
    const auto& n2 = data.normals_[1];
    const auto& n3 = data.normals_[2];

    auto mesh = std::make_unique<SmoothedMesh>(v1, v2, v3, n1, n2, n3);

    constexpr double error = 0.001;
    // Normal1
    const auto normal1 = mesh->normal1();
    EXPECT_NEAR(1.0, zisc::dot(n1, normal1), error)
        << "The n1 of triangle[" << index << "] is wrong: "
        << "n1 = (" << n1[0] << ", " << n1[1] << ", " << n1[2] << "), "
        << "ns1 = (" << normal1[0] << ", " << normal1[1] << ", " << normal1[2] << ");";
    EXPECT_EQ(normal1.data(), mesh->normal(1.0, 0.0).data());
    // Normal2
    const auto normal2 = mesh->normal2();
    EXPECT_NEAR(1.0, zisc::dot(n2, normal2), error)
        << "The n2 of triangle[" << index << "] is wrong: "
        << "n2 = (" << n2[0] << ", " << n2[1] << ", " << n2[2] << "), "
        << "ns2 = (" << normal2[0] << ", " << normal2[1] << ", " << normal2[2] << ");";
    EXPECT_EQ(normal2.data(), mesh->normal(0.0, 1.0).data());
    // Normal3
    const auto normal3 = mesh->normal3();
    EXPECT_NEAR(1.0, zisc::dot(n3, normal3), error)
        << "The n3 of triangle[" << index << "] is wrong: "
        << "n3 = (" << n3[0] << ", " << n3[1] << ", " << n3[2] << "), "
        << "ns1 = (" << normal3[0] << ", " << normal3[1] << ", " << normal3[2] << ");";
    EXPECT_EQ(normal3.data(), mesh->normal(0.0, 0.0).data());
  }
}

TEST(SmoothedMeshTest, AabbTest)
{
  const auto triangle_data_list = ::getTriangleDataList();
  const int n = static_cast<int>(triangle_data_list.size());
  for (int index = 0; index < n; ++index) {
    const auto& data = triangle_data_list[index];

    const auto& v1 = data.vertices_[0];
    const auto& v2 = data.vertices_[1];
    const auto& v3 = data.vertices_[2];
    const auto& n1 = data.normals_[0];
    const auto& n2 = data.normals_[1];
    const auto& n3 = data.normals_[2];

    auto mesh = std::make_unique<SmoothedMesh>(v1, v2, v3, n1, n2, n3);
    const auto aabb = mesh->boundingBox();

    const auto min_p = aabb.minPoint();
    const auto max_p = aabb.maxPoint();

    constexpr int loop = 100'000;
    Point3 min_point = v1;
    Point3 max_point = v1;
    for (int v_index = 0; v_index < loop; ++v_index) {
      const Float v = zisc::cast<Float>(v_index) / zisc::cast<Float>(loop);
      for (int u_index = 0; u_index < loop; ++u_index) {
        const Float u = zisc::cast<Float>(u_index) / zisc::cast<Float>(loop);
        const Float w = 1.0 - (u + v);
        if (zisc::isInClosedBounds(w, 0.0, 1.0)) {
          const auto p = mesh->point(u, v);
          min_point = zisc::minElements(min_point.data(), p.data());
          max_point = zisc::maxElements(max_point.data(), p.data());
        }
      }
    }

    // Min point
    for (int i = 0; i < 3; ++i) {
      EXPECT_DOUBLE_EQ(min_p[i], min_point[i])
          << "The AABB min of triangle[" << index << "] is wrong: "
          << "min_p = (" << min_p[0] << ", " << min_p[1] << ", " << min_p[2] << "), "
          << "min_point = (" << min_point[0] << ", " << min_point[1] << ", " << min_point[2] << ");";
    }
    // Max point
    for (int i = 0; i < 3; ++i) {
      EXPECT_DOUBLE_EQ(max_p[i], max_point[i])
          << "The AABB max of triangle[" << index << "] is wrong: "
          << "max_p = (" << max_p[0] << ", " << max_p[1] << ", " << max_p[2] << "), "
          << "max_point = (" << max_point[0] << ", " << max_point[1] << ", " << max_point[2] << ");";
    }
  }
}

TEST(SmoothedMeshTest, CurveCoefficientsTest)
{
  using namespace nanairo;
  const auto triangle_data_list = ::getTriangleDataList();
  const int size= static_cast<int>(triangle_data_list.size());
  const Ray ray{Point3{1.0, -2.0, 5.0}, Vector3{2.0, -1.0, 6.0}.normalized()};
  for (int index = 0; index < size; ++index) {
    const auto& data = triangle_data_list[index];

    const auto& v1 = data.vertices_[0];
    const auto& v2 = data.vertices_[1];
    const auto& v3 = data.vertices_[2];
    const auto& n1 = data.normals_[0];
    const auto& n2 = data.normals_[1];
    const auto& n3 = data.normals_[2];

    auto mesh = std::make_unique<SmoothedMesh>(v1, v2, v3, n1, n2, n3);
    const auto coefficients = mesh->calcCurveCoefficients(ray);
    const Float a = coefficients[0];
    const Float b = coefficients[1];
    const Float c = coefficients[2];
    const Float d = coefficients[3];
    const Float f = coefficients[4];
    const Float l = coefficients[5];
    const Float m = coefficients[6];
    const Float n = coefficients[7];
    const Float o = coefficients[8];
    const Float p = coefficients[9];
    const Float x = mesh->solvePencil(coefficients);

    const auto M = Matrix3x3{a * x + l,         0.5 * (d * x + o), 0.5 * p,
                             0.5 * (d * x + o), b * x + m,         0.5 * f * x,
                             0.5 * p,           0.5 * f * x      , c * x + n};
    constexpr double error = 0.0000001;
    EXPECT_NEAR(0.0, M.determinant(), error)
          << "The |M| of triangle[" << index << "] isn't zero.";
  }
}
