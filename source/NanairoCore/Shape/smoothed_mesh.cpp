/*!
  \file smoothed_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smoothed_mesh.hpp"
// Standard C++ library
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Data/ray.hpp"
#include "NanairoCore/DataStructure/aabb.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Geometry/transformation.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"
#include "NanairoCore/Sampling/sampler.hpp"

namespace nanairo {

// Calculate curvature parameter
Vector3 calcCurvatureParameter(const Vector3& distance,
                               const Vector3& normal1,
                               const Vector3& normal2) noexcept;

// Calculate surface parameters
bool calcSurfaceParameter(const std::array<Float, 5>& a,
                          const std::array<Float, 6>& b,
                          const std::array<Float, 6>& r,
                          Float* eta,
                          Float* xi,
                          Float* t) noexcept;

/*!
  \details
  No detailed.
  */
SmoothedMesh::SmoothedMesh(const Point3& vertex1,
                           const Point3& vertex2,
                           const Point3& vertex3,
                           const Vector3& normal1,
                           const Vector3& normal2,
                           const Vector3& normal3) noexcept
{
  initialize(vertex1, vertex2, vertex3, normal1, normal2, normal3);
}

/*!
  \details
  No detailed.
  */
Aabb SmoothedMesh::boundingBox() const noexcept
{
  // Distance
  const auto d0 = c_[1] + c_[4];
  const auto d1 = c_[2] + c_[3] + c_[5];
  const auto d2 = d0 + d1;
  const Vector3* distance[] = {&d0, &d1, &d2};
  // Points
  const auto& v0 = c_[0];
  const auto v1 = v0 + d0;
  const auto v2 = v0 + d2;
//  const Vector3* vertices[] = {&v0, &v1, &v2};
  const Vector3* vertices[] = {&v0, &v1, &v0};
  // Curvature parameters
  const auto& c0 = c_[4];
  const auto& c1 = c_[5];
  const auto c2 = c_[3] + c0 + c1;
  const Vector3* curvature[] = {&c0, &c1, &c2};

  const auto calc_extremum_edge =
  [&distance, &curvature](const uint i, const uint j, Float* xi)
  {
    const auto c = (*(curvature[i]))[j];
    if (c == 0.0)
      return false;
    const auto d = (*(distance)[i])[j];
    const auto x = (c - d) / (2.0 * c);
    const bool result = zisc::isInClosedBounds(x, 0.0, 1.0);
    if (result)
      *xi = x;
    return result;
  };

  const auto edge_point =
  [&vertices, &distance, &curvature](const uint i, const Float xi)
  {
    return *(vertices[i]) + (*(distance[i]) + (xi - 1.0) * *(curvature[i])) * xi;
  };

  const auto calc_extremum_surface =
  [this](const uint j, Float* eta, Float* xi)
  {
    const zisc::Matrix<Float, 2, 2> matrix{2.0 * c_[4][j], c_[3][j],
                                           c_[3][j], 2.0 * c_[5][j]};
    if (matrix.determinant() == 0.0)
      return false;
    const auto inverse_matrix = matrix.inverseMatrix();
    const zisc::Matrix<Float, 2, 1> c{c_[1][j], c_[2][j]};
    const auto result = inverse_matrix * c;
    const Float e = result(0, 0);
    const Float x = result(1, 0);
    const bool is_in = zisc::isInClosedBounds(x, 0.0, e) && (e <= 1.0);
    if (is_in) {
      *eta = e;
      *xi = x;
    }
    return is_in;
  };

  // Calc a bounding box
  auto min_point = zisc::minElements(v0.data(), v1.data());
  auto max_point = zisc::maxElements(v0.data(), v1.data());
  min_point = zisc::minElements(min_point, v2.data());
  max_point = zisc::maxElements(max_point, v2.data());
  Float eta,
         xi;
  for (uint j = 0; j < 3; ++j) {
    for (uint i = 0; i < 3; ++i) {
      if (calc_extremum_edge(i, j, &xi)) {
        const auto point = edge_point(i, xi);
        min_point = zisc::minElements(min_point, point.data());
        max_point = zisc::maxElements(max_point, point.data());
      }
    }
    if (calc_extremum_surface(j, &eta, &xi)) {
      const auto point = SmoothedMesh::point(eta, xi);
      min_point = zisc::minElements(min_point, point.data());
      max_point = zisc::maxElements(max_point, point.data());
    }
  }

  return Aabb{Point3{min_point}, Point3{max_point}};
}

/*!
  \details
  No detailed.
  */
Float SmoothedMesh::getTraversalCost() const noexcept
{
  return 8.0;
}

/*!
  \details
  No detailed.
  */
bool SmoothedMesh::testIntersection(const Ray& ray, 
                                    IntersectionInfo* intersection) const noexcept
{
  const auto& e3 = ray.direction();
  auto e1 = zisc::cross(e3, c_[5]);
  const auto k = e1.squareNorm();
  if (k == 0.0)
    return false;
  e1 = e1 * zisc::invert(zisc::sqrt(k));
  const auto e2 = zisc::cross(e3, e1);

  const std::array<Float, 5> a = {{
      zisc::dot(e1, c_[0] - *treatAs<const Vector3*>(&ray.origin())), // a00
      zisc::dot(e1, c_[1]), // a10
      zisc::dot(e1, c_[2]), // a01
      zisc::dot(e1, c_[3]), // a11
      zisc::dot(e1, c_[4])}}; // a20
  const std::array<Float, 6> b = {{
      zisc::dot(e2, c_[0] - *treatAs<const Vector3*>(&ray.origin())), // b00
      zisc::dot(e2, c_[1]), // b10
      zisc::dot(e2, c_[2]), // b01
      zisc::dot(e2, c_[3]), // b11
      zisc::dot(e2, c_[4]), // b20
      zisc::dot(e2, c_[5])}}; // b02
  const std::array<Float, 6> r = {{
      zisc::dot(e3, c_[0] - *treatAs<const Vector3*>(&ray.origin())), // r00
      zisc::dot(e3, c_[1]), // r10
      zisc::dot(e3, c_[2]), // r01
      zisc::dot(e3, c_[3]), // r11
      zisc::dot(e3, c_[4]), // r20
      zisc::dot(e3, c_[5])}}; // r02

  Float eta,
        xi,
        t = intersection->rayDistance();
  const bool is_hit = calcSurfaceParameter(a, b, r, &eta, &xi, &t);
  if (is_hit) {
    // Set the intersection info
    const auto point = ray.origin() + t * ray.direction();
    const auto normal = SmoothedMesh::normal(eta, xi);
    intersection->setPoint(point);
    const Float cos_theta = zisc::dot(normal, ray.direction());
    intersection->setReverseFace(0.0 < cos_theta);
    intersection->setNormal(normal);
    intersection->setRayDistance(t);
    const Float u = eta - xi,
                v = xi;
    intersection->setTextureCoordinate(textureCoordinate(u, v));
  }
  return is_hit;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledPoint, Vector3, Point2> SmoothedMesh::samplePoint(
    Sampler& sampler) const noexcept
{
  Float u = sampler.sample();
  Float v = sampler.sample();
  if (1.0 < (u + v)) {
    u = 1.0 - u;
    v = 1.0 - v;
  }
  const Float eta = u + v;
  const Float xi = v;
  //! \todo Calculate the surface area of the smoothed mesh
  zisc::raiseError("Todo: calculate the surface area.");
  return std::make_tuple(SampledPoint{point(eta, xi), 0.0},
                         normal(eta, xi),
                         textureCoordinate(u, v));
}

/*!
  \details
  No detailed.
  */
void SmoothedMesh::transform(const Matrix4x4& matrix) noexcept
{
  Transformation::affineTransform(matrix, treatAs<Point3*>(&c_[0]));
  for (uint i = 1; i < 6; ++i)
    Transformation::affineTransform(matrix, &c_[i]);
}

/*!
  \details
  No detailed.
  */
void SmoothedMesh::initialize(const Point3& vertex1,
                              const Point3& vertex2,
                              const Point3& vertex3,
                              const Vector3& normal1,
                              const Vector3& normal2,
                              const Vector3& normal3) noexcept
{
  const Vector3 distance[3] = {vertex2 - vertex1,
                               vertex3 - vertex2,
                               vertex3 - vertex1};
  const Vector3 curvature[3] = {
      calcCurvatureParameter(distance[0], normal1, normal2),
      calcCurvatureParameter(distance[1], normal2, normal3),
      calcCurvatureParameter(distance[2], normal1, normal3)};

  c_[0] = *treatAs<const Vector3*>(&vertex1); // c00
  c_[1] = distance[0] - curvature[0]; // c10
  c_[2] = distance[1] + curvature[0] - curvature[2]; // c01
  c_[3] = curvature[2] - curvature[0] - curvature[1]; // c11
  c_[4] = curvature[0]; // c20
  c_[5] = curvature[1]; // c02
}

/*!
  \details
  No detailed.
  */
Vector3 calcCurvatureParameter(const Vector3& distance,
                               const Vector3& normal0,
                               const Vector3& normal1) noexcept
{
  const auto c = zisc::dot(normal0, normal1);
//  ZISC_ASSERT((0.0 <= c) && (c <= 1.0), "The c must be [0, 1]: ", c);
  if (std::abs(c) == 1.0)
    return Vector3{};
  const auto delta_c = (1.0 - c) * 0.5;

  const auto v = (normal0 + normal1) * 0.5;
  const auto delta_v = (normal0 - normal1) * 0.5;
  const auto d = zisc::dot(distance, v);
  const auto delta_d = zisc::dot(distance, delta_v);

  constexpr Float epsilon = 0.01;
  const auto parameter =
      (delta_c < epsilon)         ? (delta_d / (1.0 - delta_c)) * v :
      ((1.0 - epsilon) < delta_c) ? (d / delta_c) * delta_v
                                  : (delta_d / (1.0 - delta_c)) * v +
                                    (d / delta_c) * delta_v;
  return parameter;
}

/*!
  \details
  No detailed.
  */
bool calcSurfaceParameter(const std::array<Float, 5>& a,
                          const std::array<Float, 6>& b,
                          const std::array<Float, 6>& r,
                          Float* eta,
                          Float* xi,
                          Float* t) noexcept
{
  bool is_hit = false;
  // Calc eta
  if (a[2] != 0.0 || a[3] != 0.0) {
    const std::array<Float, 4> u = {{
        a[3] * b[4] - a[4] * b[3],
        a[3] * b[1] - a[1] * b[3] + a[2] * b[4] - a[4] * b[2],
        a[2] * b[1] - a[1] * b[2] + a[3] * b[0] - a[0] * b[3],
        a[2] * b[0] - a[0] * b[2]}};
    const Float m = a[3] * u[0] + a[4] * a[4] * b[5],
                n = a[2] * u[0] + a[3] * u[1] + 2.0 * a[1] * a[4] * b[5],
                o = a[2] * u[1] + a[3] * u[2] + 
                    b[5] * (a[1] * a[1] + 2.0 * a[0] * a[4]),
                p = a[2] * u[2] + a[3] * u[3] + 2.0 * a[0] * a[1] * b[5],
                q = a[2] * u[3] + a[0] * a[0] * b[5];
    const auto result = zisc::solveQuartic(q, p, o, n, m);
    const auto& inverse_eta_list = std::get<0>(result);
    const auto n_eta = std::get<1>(result);
    for (uint i = 0; i < n_eta; ++i) {
      if (inverse_eta_list[i] < 1.0)
        continue;
      const auto e = zisc::invert(inverse_eta_list[i]);
      const Float x = -(a[0] + (a[1] + a[4] * e) * e) / (a[2] + a[3] * e);
      if (x < 0.0 || e < x)
        continue;
      const Float lambda = r[0] + (r[1] + r[4] * e) * e +
                           (r[2] + r[3] * e + r[5] * x) * x;
      if (zisc::isInOpenBounds(lambda, 0.0, *t)) {
        *eta = e;
        *xi = x;
        *t = lambda;
        is_hit = true;
      }
    }
  }
  else {
    const auto eta_result = zisc::solveQuadratic(a[4], a[1], a[0]);
    const auto& eta_list = std::get<0>(eta_result);
    const auto n_eta = std::get<1>(eta_result);
    for (uint i = 0; i < n_eta; ++i) {
      const auto e = eta_list[i];
      if (e < 0.0 || 1.0 < e)
        continue;
      const auto& xi_result = 
          zisc::solveQuadratic(b[5], b[2] + b[3] * e, b[0] + (b[1] + b[4] * e) * e);
      const auto& xi_list = std::get<0>(xi_result);
      const auto n_xi = std::get<1>(xi_result);
      for (uint j = 0; j < n_xi; ++j) {
        const auto x = xi_list[j];
        if (x < 0.0 || e < x)
          continue;
        const Float lambda = r[0] + (r[1] + r[4] * e) * e + 
                             (r[2] + r[3] * e + r[5] * x) * x;
        if (zisc::isInOpenBounds(lambda, 0.0, *t)) {
          *eta = e;
          *xi = x;
          *t = lambda;
          is_hit = true;
          ZISC_ASSERT(zisc::isInClosedBounds(x, 0.0, e),
                      "The xi is out of the range [0, eta].");
          ZISC_ASSERT(zisc::isInClosedBounds(e, x, 1.0),
                      "The eta is out of the range [xi, 1].");
        }
      }
    }
  }
  return is_hit;
}

} // namespace nanairo
