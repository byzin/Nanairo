/*!
  \file smoothed_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smoothed_mesh.hpp"
// Standard C++ library
#include <cmath>
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
  // Vertices
  const auto v1 = vertex1();
  const auto v2 = vertex2();
  const auto v3 = vertex3();
  auto min_point = zisc::minElements(v1.data(), v2.data());
  auto max_point = zisc::maxElements(v1.data(), v2.data());
  min_point = zisc::minElements(min_point, v3.data());
  max_point = zisc::maxElements(max_point, v3.data());
  // Surface extremum
  {
    const auto ng = zisc::cross(v2 - v1, v3 - v1).normalized();
    const Float k = 1.0 / (4.0 * zisc::dot(ng, c_[0]) * zisc::dot(ng, c_[1]) -
                           zisc::power<2>(zisc::dot(ng, c_[3])));
    const Float u = k * (-2.0 * zisc::dot(ng, c_[1]) * zisc::dot(ng, c_[4]) +
                         zisc::dot(ng, c_[5]) * zisc::dot(ng, c_[3]));
    const Float v = k * (-2.0 * zisc::dot(ng, c_[0]) * zisc::dot(ng, c_[5]) +
                         zisc::dot(ng, c_[4]) * zisc::dot(ng, c_[3]));
    const Float w = 1.0 - (u + v);
    std::cout << "(u, v) = (" << u << ", " << v << ")" << std::endl;
    if (zisc::isInClosedBounds(u, 0.0, 1.0) &&
        zisc::isInClosedBounds(v, 0.0, 1.0) &&
        zisc::isInClosedBounds(w, 0.0, 1.0)) {
      const auto p = point(u, v);
      min_point = zisc::minElements(min_point, p.data());
      max_point = zisc::maxElements(max_point, p.data());
    }
    else {
      if (zisc::isInClosedBounds(u, 0.0, 1.0)) {
        const auto p1 = point(u, 0.0);
        min_point = zisc::minElements(min_point, p1.data());
        max_point = zisc::maxElements(max_point, p1.data());
      }
      if (zisc::isInClosedBounds(v, 0.0, 1.0)) {
        const auto p2 = point(0.0, v);
        min_point = zisc::minElements(min_point, p2.data());
        max_point = zisc::maxElements(max_point, p2.data());
      }
    }
  }
  // Edge extremums
  {
    const auto p1 = point(0.5, 0.0);
    min_point = zisc::minElements(min_point, p1.data());
    max_point = zisc::maxElements(max_point, p1.data());
    const auto p2 = point(0.0, 0.5);
    min_point = zisc::minElements(min_point, p2.data());
    max_point = zisc::maxElements(max_point, p2.data());
    const auto p3 = point(0.5, 0.5);
    min_point = zisc::minElements(min_point, p3.data());
    max_point = zisc::maxElements(max_point, p3.data());
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
  const auto plane1 = calcRayPlane(ray, c_[0]);
  const auto& d1 = std::get<0>(plane1);
  const Float k1 = std::get<1>(plane1);
  const auto plane2 = calcRayPlane(ray, c_[1]);
  const auto& d2 = std::get<0>(plane2);
  const Float k2 = std::get<1>(plane2);

  const Float b = zisc::dot(c_[1], d1);
  const Float c = zisc::dot(c_[2], d1) + k1;
  const Float d = zisc::dot(c_[3], d1);
  const Float e = zisc::dot(c_[4], d1);
  const Float f = zisc::dot(c_[5], d1);
  const Float l = zisc::dot(c_[0], d2);
  const Float n = zisc::dot(c_[2], d2) + k2;
  const Float o = zisc::dot(c_[3], d2);
  const Float p = zisc::dot(c_[4], d2);
  const Float q = zisc::dot(c_[5], d2);
  const Float x = calcX(b, c, d, e, f, l , n, o, p, q);
  return testLineSurfaceIntersection(ray, b, c, d, e, f, l , n, o, p, q, x,
                                     intersection);
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledPoint, Vector3, Point2> SmoothedMesh::samplePoint(
    Sampler& /* sampler */) const noexcept
{
  zisc::raiseError("Bad");
  return std::make_tuple(SampledPoint{}, Vector3{}, Point2{});
}

/*!
  \details
  No detailed.
  */
void SmoothedMesh::transform(const Matrix4x4& matrix) noexcept
{
  auto v1 = Point3{vertex1().data()};
  auto v2 = Point3{vertex2().data()};
  auto v3 = Point3{vertex3().data()};
  Transformation::affineTransform(matrix, &v1);
  Transformation::affineTransform(matrix, &v2);
  Transformation::affineTransform(matrix, &v3);
  auto n1 = normal1();
  auto n2 = normal2();
  auto n3 = normal3();
  Transformation::affineTransform(matrix, &n1);
  Transformation::affineTransform(matrix, &n2);
  Transformation::affineTransform(matrix, &n3);
  calcControlPoints(v1, v2, v3, n1, n2, n3);
}

/*!
  \details
  Please see "Simplification of Meshes into Curved PN Triangles.pdf"
  */
void SmoothedMesh::calcControlPoints(const Point3& vertex1,
                                     const Point3& vertex2,
                                     const Point3& vertex3,
                                     const Vector3& n1,
                                     const Vector3& n2,
                                     const Vector3& n3) noexcept
{
  const auto& v1 = *zisc::treatAs<const Vector3*>(&vertex1);
  const auto& v2 = *zisc::treatAs<const Vector3*>(&vertex2);
  const auto& v3 = *zisc::treatAs<const Vector3*>(&vertex3);

  auto eval_w = [](const Vector3& vi, const Vector3& vj, const Vector3& n)
  {
    return zisc::dot((vj - vi), n);
  };
  const Float w12 = eval_w(v1, v2, n1);
  const Float w13 = eval_w(v1, v3, n1);
  const Float w21 = eval_w(v2, v1, n2);
  const Float w23 = eval_w(v2, v3, n2);
  const Float w31 = eval_w(v3, v1, n3);
  const Float w32 = eval_w(v3, v2, n3);
  c_[0] = 0.5 * (w13 * n1 + w31 * n3);
  c_[1] = 0.5 * (w23 * n2 + w32 * n3);
  c_[2] = v3;
  c_[3] = 0.5 * ((w13 - w12) * n1 + (w23 - w21) * n2 + (w31 + w32) * n3);
  c_[4] = (v1 - v3) - c_[0];
  c_[5] = (v2 - v3) - c_[1];
}

/*!
  */
std::tuple<Vector3, Float> SmoothedMesh::calcRayPlane(
    const Ray& ray,
    const Vector3& c) const noexcept
{
  const auto& o = ray.origin();
  const auto& v = ray.direction();
  const Matrix3x3 kernel{o[0], o[1], o[2],
                         v[0], v[1], v[2],
                         c[0], c[1], c[2]};
  const Float determinant = kernel.determinant();
  ZISC_ASSERT(determinant == 0.0, "The determinant is zero.");
  // Calc the plane (d[0] * x + d[1] * y * d[2] * z + k = 0)
  const Vector3 d{v[2] * c[1] - v[1] * c[2],
                  v[0] * c[2] - v[2] * c[0],
                  v[1] * c[0] - v[0] * c[1]};
  const Float k = determinant;
  return std::make_tuple(d, k);
}

/*!
  */
Float SmoothedMesh::calcX(const Float b,
                          const Float c,
                          const Float d,
                          const Float e,
                          const Float f,
                          const Float l,
                          const Float n,
                          const Float o,
                          const Float p,
                          const Float q) const noexcept
{
  const Float a3 = 0.25 * (d * e * f - b * e * e - c * d * d);
  const Float a2 = (l * b * c) -
                   0.5 * (b * e * p + c * d * o) +
                   0.25 * (o * e * f + d * e * q + d * p * f - l * f * f - n * d * d);
  const Float a1 = (l * b * n) -
                   0.5 * (l * f * q + n * d * o) +
                   0.25 * (d * p * q + o * e * q + o * p * f - b * p * p - c * o * o);
  const Float a0 = 0.25 * (o * p * q - l * q * q - n * o * o);
  return zisc::solveCubicOne(a3, a2, a1, a0);
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
  calcControlPoints(vertex1, vertex2, vertex3, normal1, normal2, normal3);
}

/*!
  */
bool SmoothedMesh::testLineSurfaceIntersection(
    const Ray& ray,
    const Float b,
    const Float c,
    const Float d,
    const Float e,
    const Float f,
    const Float l,
    const Float n,
    const Float o,
    const Float p,
    const Float q,
    const Float x,
    IntersectionInfo* intersection) const noexcept
{
  auto test_line_surface_intersection =
  [this, &ray, &intersection, b, c, d, e, f](const Float beta, const Float gamma)
  {
    const Float x3 = b - d * beta;
    const Float x2 = f - (d * gamma + e * beta);
    const Float x1 = c - e * gamma;
    const Float discriminant = zisc::power<2>(x2) - 4.0 * x3 * x1;
    bool is_hit = false;
    if (0.0 < discriminant) {
      const Float inv_x3 = 1.0 / (2.0 * x3);
      const Float v1 = (-x2 + zisc::sqrt(discriminant)) * inv_x3;
      const Float u1 = -(beta * v1 + gamma);
      is_hit = testRaySurfaceIntersection(ray, u1, v1, intersection);
      const Float v2 = (-x2 - zisc::sqrt(discriminant)) * inv_x3;
      const Float u2 = -(beta * v2 + gamma);
      is_hit = is_hit || testRaySurfaceIntersection(ray, u2, v2, intersection);
    }
    else if (discriminant == 0.0) {
      const Float inv_x3 = 1.0 / (2.0 * x3);
      const Float v = -x2 * inv_x3;
      const Float u = -(beta * v + gamma);
      is_hit = testRaySurfaceIntersection(ray, u, v, intersection);
    }
    return is_hit;
  };

  const Float m11 = l;
  const Float m22 = b * x;
  const Float m12 = 0.5 * (d * x + o);
  const Float m21 = m12;
  bool is_hit = (m11 * m22 - m12 * m21) < 0.0;
  if (is_hit) {
    if (m22 < m11) {
      const Float inv_m11 = 1.0 / m11;
      const Float m12d = m12 * inv_m11;
      const Float m22d = m22 * inv_m11;
      const Float m13d = 0.5 * (e * x + p) * inv_m11;
      const Float m33d = (c * x + n) * inv_m11;
      // Line1
      const Float beta1 = m12d + zisc::sqrt(zisc::power<2>(m12d) - m22d);
      const Float gamma1 = m13d + zisc::sqrt(zisc::power<2>(m13d) - m33d);
      is_hit = test_line_surface_intersection(beta1, gamma1);
      // Line2
      const Float beta2 = m12d - zisc::sqrt(zisc::power<2>(m12d) - m22d);
      const Float gamma2 = m13d - zisc::sqrt(zisc::power<2>(m13d) - m33d);
      is_hit = is_hit || test_line_surface_intersection(beta2, gamma2);
    }
    else {
      const Float inv_m22 = 1.0 / m22;
      const Float m12d = m12 * inv_m22;
      const Float m11d = m11 * inv_m22;
      const Float m23d = 0.5 * (f * x + q) * inv_m22;
      const Float m33d = (c * x + n) * inv_m22;
      // Line1
      const Float beta1 = m12d + zisc::sqrt(zisc::power<2>(m12d) - m11d);
      const Float gamma1 = m23d + zisc::sqrt(zisc::power<2>(m23d) - m33d);
      is_hit = test_line_surface_intersection(beta1, gamma1);
      // Line2
      const Float beta2 = m12d - zisc::sqrt(zisc::power<2>(m12d) - m11d);
      const Float gamma2 = m23d - zisc::sqrt(zisc::power<2>(m23d) - m33d);
      is_hit = is_hit || test_line_surface_intersection(beta2, gamma2);
    }
  }
  return is_hit;
}

/*!
  */
bool SmoothedMesh::testRaySurfaceIntersection(
    const Ray& ray,
    const Float u,
    const Float v,
    IntersectionInfo* intersection) const noexcept
{
  const Float w = 1.0 - (u + v);
  if (!(zisc::isInClosedBounds(u, 0.0, 1.0) &&
        zisc::isInClosedBounds(v, 0.0, 1.0) &&
        zisc::isInClosedBounds(w, 0.0, 1.0)))
    return false;

  const auto p = point(u, v);
  const Float t2 = (p - ray.origin()).squareNorm();
  if (zisc::power<2>(intersection->rayDistance()) < t2)
    return false;

  const auto n = normal(u, v);
  intersection->setPoint(p);
  const Float cos_theta = zisc::dot(n, ray.direction());
  intersection->setReverseFace(0.0 < cos_theta);
  intersection->setNormal(n);
  intersection->setRayDistance(zisc::sqrt(t2));
  intersection->setTextureCoordinate(textureCoordinate(u, v));

  return true;
}

} // namespace nanairo
