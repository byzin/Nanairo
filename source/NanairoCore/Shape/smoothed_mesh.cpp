/*!
  \file smoothed_mesh.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smoothed_mesh.hpp"
// Standard C++ library
#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <tuple>
#include <utility>
// Zisc
#include "zisc/error.hpp"
#include "zisc/math.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "flat_mesh.hpp"
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
  return 16.0;
}

/*!
  \details
  No detailed.
  */
bool SmoothedMesh::testIntersection(const Ray& ray,
                                    IntersectionInfo* intersection) const noexcept
{
  const auto coefficients = calcCurveCoefficients(ray);
  constexpr auto method = smoothingMethod();
  return
  (method == SmoothingMethod::kResultant)
      ? testIntersectionUsingResultant(ray, coefficients, intersection) :
  (method == SmoothingMethod::kPencil)
      ? testIntersectionUsingPencil(ray, coefficients, intersection)
      : testIntersectionWithoutSmoothing(ray, intersection);
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
                                     const Vector3& normal1,
                                     const Vector3& normal2,
                                     const Vector3& normal3) noexcept
{
  auto eval_edge = [](const Point3& vi, const Point3& vj)
  {
    return vj - vi;
  };
  auto eval_curvature = [eval_edge](const Point3& vi, const Point3& vj,
                                    const Vector3& ni, const Vector3& nj)
  {
    constexpr Float alpha = 0.5;
    return alpha * (zisc::dot(nj, eval_edge(vi, vj)) * nj -
                    zisc::dot(ni, eval_edge(vi, vj)) * ni);
  };

  c_[0] = -eval_curvature(vertex3, vertex1, normal3, normal1);
  c_[1] = -eval_curvature(vertex2, vertex3, normal2, normal3);
  c_[2] = Vector3{vertex3.data()};
  c_[3] = eval_curvature(vertex1, vertex2, normal1, normal2) + c_[0] + c_[1];
  c_[4] = eval_edge(vertex3, vertex1) - c_[0];
  c_[5] = eval_edge(vertex3, vertex2) - c_[1];
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
  ZISC_ASSERT(determinant != 0.0, "The determinant is zero.");
  // Calc the plane (d[0] * x + d[1] * y * d[2] * z + k = 0)
  const Vector3 d{v[2] * c[1] - v[1] * c[2],
                  v[0] * c[2] - v[2] * c[0],
                  v[1] * c[0] - v[0] * c[1]};
  const Float k = determinant;
  ZISC_ASSERT(
    zisc::isInClosedBounds(zisc::dot(d, Vector3{o.data()}) + k, -0.000001, 0.000001),
    "The calculation of plane d, k is wrong.");
  ZISC_ASSERT(zisc::isInClosedBounds(zisc::dot(d, v), -0.000001, 0.000001),
              "The calculation of plane d is wrong.");
  ZISC_ASSERT(zisc::isInClosedBounds(zisc::dot(d, c), -0.000001, 0.000001),
              "The calculation of plane d is wrong.");
  return std::make_tuple(d, k);
}

/*!
  */
std::tuple<std::array<Float, 2>, uint> SmoothedMesh::calcResultantV(
    const std::array<Float, 10>& coefficients,
    const Float u) const noexcept
{
  const Float a = coefficients[0];
  const Float b = coefficients[1];
  const Float c = coefficients[2];
  const Float d = coefficients[3];
  const Float f = coefficients[4];

  const Float t2 = b;
  const Float t1 = d * u + f;
  const Float t0 = a * u * u + c;

  std::array<Float, 2> v_list;
  uint n = 0;
  if (t2 != 0.0) {
    const auto result = zisc::solveQuadratic(t2, t1, t0);
    v_list = std::get<0>(result);
    n = std::get<1>(result);
  }
  else if (t1 != 0.0) {
    const Float v = -t0 / t1;
    v_list[0] = v;
    n = 1;
  }
  return std::make_tuple(v_list, n);
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
Float SmoothedMesh::solvePencil(
    const std::array<Float, 10>& coefficients) const noexcept
{
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
  // Calculate the coefficients of x
  const Float a0 = (a * b * c) - 0.25 * (a * f * f + c * d * d);
  const Float a1 = a * (b * n + m * c) + (l * b * c) - 0.5 * (c * d * o) +
                   0.25 * (f * (d * p - l * f) - n * d * d);
  const Float a2 = (a * m * n) + l * (b * n + m * c) - 0.5 * (n * d * o) +
                   0.25 * (p * (o * f - b * p) - c * o * o);
  const Float a3 = (l * m * n) - 0.25 * (m * p * p + n * o * o);
  // Calculate the x
  Float x = 0.0;
  if (a3 != 0.0) {
    x = zisc::solveCubicOne(a3, a2, a1, a0);
  }
  else if (a2 != 0.0) {
    const Float discriminant = zisc::power<2>(a1) - 4.0 * a2 * a0;
    ZISC_ASSERT(0.0 <= discriminant, "The discriminant is minus.");
    x = (discriminant == 0.0)
        ? -a1 / (2.0 * a2)
        : (-a1 + zisc::sqrt(discriminant)) / (2.0 * a2);
  }
  else {
    ZISC_ASSERT(a1 != 0.0, "The a1 is zero.");
    x = -a0 / a1;
  }
  return 1.0 / x;
}

/*!
  */
std::tuple<std::array<Float, 4>, uint> SmoothedMesh::solveResultant(
    const std::array<Float, 10>& coefficients) const noexcept
{
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

  const Float a0 = (a * b * o * o) + (a * a * m * m) +
                   (d * d * l * m) + (b * b * l * l) -
                   ((a * d * m * o) + (b * d * l * o) + 2.0 * (a * b * l * m));
  const Float a1 = (d * d * m * p) -
                   ((b * d * o * p) + (a * f * m * o) + (b * f * l * o)) +
                  2.0 * ((b * b * l * p) + (d * f * l * m) - (a * b * m * p));
  const Float a2 = (f * f * l * m) + (b * c * o * o) +
                   (d * d * m * n) + (b * b * p * p) -
                   ((b * f * o * p) + (b * d * n * o) + (c * d * m * o)) +
                   2.0 * ((b * b * l * n) + (a * c * m * m) + (d * f * m * p) -
                          (a * b * m * n) - (b * c * l * m));
  const Float a3 = (f * f * m * p) - ((b * f * n * o) + (c * f * m * o)) +
                   2.0 * ((b * b * n * p) + (d * f * m * n) - (b * c * m * p));
  const Float a4 = (b * b * n * n) + (f * f * m * n) + (c * c * m * m) -
                   2.0 * (b * c * m * n);
  return zisc::solveQuartic(a4, a3, a2, a1, a0);
}

/*!
  \details
  No detailed.
  */
bool SmoothedMesh::testIntersectionUsingPencil(
    const Ray& ray,
    const std::array<Float, 10>& coefficients,
    IntersectionInfo* intersection) const noexcept
{
  const Float x = solvePencil(coefficients);
  return (std::isfinite(x))
      ? testPencilLineIntersection(ray, coefficients, x, intersection)
      : false;
}

/*!
  \details
  No detailed.
  */
bool SmoothedMesh::testIntersectionUsingResultant(
    const Ray& ray,
    const std::array<Float, 10>& coefficients,
    IntersectionInfo* intersection) const noexcept
{
  const auto u_result = solveResultant(coefficients);
  const auto& u_list = std::get<0>(u_result);
  const uint u_count = std::get<1>(u_result);
  bool is_hit = false;
  for (uint i = 0; i < u_count; ++i) {
    const Float u = 1.0 / u_list[i];
    if (!zisc::isInClosedBounds(u, 0.0, 1.0))
      continue;
    const auto v_result = calcResultantV(coefficients, u);
    const auto& v_list = std::get<0>(v_result);
    const uint v_count = std::get<1>(v_result);
    for (uint j = 0; j < v_count; ++j) {
      const Float v = v_list[j];
      if (!zisc::isInClosedBounds(v, 0.0, 1.0))
        continue;
      is_hit = is_hit || testRaySurfaceIntersection(ray, u, v, intersection);
    }
  }
  return is_hit;
}

/*!
  \details
  No detailed.
  */
bool SmoothedMesh::testIntersectionWithoutSmoothing(
    const Ray& ray,
    IntersectionInfo* intersection) const noexcept
{
  const Point3 v1{vertex1().data()};
  const Point3 v2{vertex2().data()};
  const Point3 v3{vertex3().data()};
  const FlatMesh mesh{v1, v2, v3};
  return mesh.testIntersection(ray, intersection);
}

/*!
  */
bool SmoothedMesh::testPencilLineIntersection(
    const Ray& ray,
    const std::array<Float, 10>& coefficients,
    const Float x,
    IntersectionInfo* intersection) const noexcept
{
//  constexpr Float error = 0.0000001;
  constexpr Float error = 0.001;

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

  const Float m11 = a * x + l;
  const Float m22 = b * x * m;
  const Float m12 = 0.5 * (d * x + o);
  const Float m21 = m12;
  bool is_hit = (m11 * m22 - m12 * m21) < 0.0;
  if (is_hit) {
    if (zisc::abs(m22) < zisc::abs(m11)) {
      const Float inv_m11 = 1.0 / m11;
      // Beta
      const Float m12d = m12 * inv_m11;
      const Float m22d = m22 * inv_m11;
      const Float b_term = (0.0 < (zisc::power<2>(m12d) - m22d))
          ? zisc::sqrt(zisc::power<2>(m12d) - m22d)
          : 0.0;
      const Float beta1 = m12d + b_term;
      const Float beta2 = m12d - b_term;
      ZISC_ASSERT(
          zisc::isInBounds((d * x + o) / m11 - (beta1 + beta2), -error, error),
          "The calc of beta is wrong: ", "M12 = ", (d * x + o) / m11,
          ", beta1 = ", beta1, ", beta2 = ", beta2,
          ", b1+b2 = ", beta1 + beta2,
          ", diff = ", (d * x + o) / m11 - (beta1 + beta2));
      // Gamma
      const Float m13d = 0.5 * p * inv_m11;
      const Float m33d = (c * x + n) * inv_m11;
      const Float g_term = (0.0 < (zisc::power<2>(m13d) - m33d))
          ? zisc::sqrt(zisc::power<2>(m13d) - m33d)
          : 0.0;
      Float gamma1 = m13d + g_term;
      Float gamma2 = m13d - g_term;
      ZISC_ASSERT(
          zisc::isInBounds(p / m11 - (gamma1 + gamma2), -error, error),
          "The calc of gamma is wrong: ", "M13 = ", p / m11,
          ", gamma1 = ", gamma1, ", gamma2 = ", gamma2,
          ", g1+g2 = ", gamma1 + gamma2,
          ", diff = ", p / m11 - (gamma1 + gamma2));
      const Float k = (f * x) * inv_m11;
      if (zisc::abs(k - (beta1 * gamma1 + beta2 * gamma2)) <
          zisc::abs(k - (beta1 * gamma2 + beta2 * gamma1)))
        std::swap(gamma1, gamma2);
      // Test line-surface intersection
      is_hit = testPencilLineIntersection(ray, coefficients,
                                           1.0, beta1, gamma1, intersection) ||
               testPencilLineIntersection(ray, coefficients,
                                           1.0, beta2, gamma2, intersection);
    }
    else {
      const Float inv_m22 = 1.0 / m22;
      // Alpha
      const Float m12d = m12 * inv_m22;
      const Float m11d = m11 * inv_m22;
      const Float a_term = (0.0 < (zisc::power<2>(m12d) - m11d))
          ? zisc::sqrt(zisc::power<2>(m12d) - m11d)
          : 0.0;
      const Float alpha1 = m12d + a_term;
      const Float alpha2 = m12d - a_term;
      ZISC_ASSERT(alpha1 != 0.0, "The alpha1 is zero.");
      ZISC_ASSERT(alpha2 != 0.0, "The alpha2 is zero.");
      ZISC_ASSERT(
          zisc::isInBounds((d * x + o) / m22 - (alpha1 + alpha2), -error, error),
          "The calc of alpha is wrong: ", "M12 = ", (d * x + o) / m22,
          ", alpha1 = ", alpha1, ", alpha2 = ", alpha2,
          ", a1+a2 = ", alpha1 + alpha2,
          ", diff = ", (d * x + o) / m22 - (alpha1 + alpha2));
      // Gamma
      const Float m23d = 0.5 * f * x * inv_m22;
      const Float m33d = (c * x + n) * inv_m22;
      const Float g_term = (0.0 < (zisc::power<2>(m23d) - m33d))
          ? zisc::sqrt(zisc::power<2>(m23d) - m33d)
          : 0.0;
      Float gamma1 = m23d + g_term;
      Float gamma2 = m23d - g_term;
      ZISC_ASSERT(
          zisc::isInBounds((f * x) / m22 - (gamma1 + gamma2), -error, error),
          "The calc of gamma is wrong: ", "M22 = ", (f * x) / m22,
          ", gamma1 = ", gamma1, ", gamma2 = ", gamma2,
          ", g1+g2 = ", gamma1 + gamma2,
          ", diff = ", (f * x) / m22 - (gamma1 + gamma2));
      const Float k = p * inv_m22;
      if (zisc::abs(k - (alpha1 * gamma1 + alpha2 * gamma2)) <
          zisc::abs(k - (alpha1 * gamma2 + alpha2 * gamma1)))
        std::swap(gamma1, gamma2);
      // Test line-surface intersection
      is_hit = testPencilLineIntersection(ray, coefficients,
                                          alpha1, 1.0, gamma1, intersection) ||
               testPencilLineIntersection(ray, coefficients,
                                          alpha2, 1.0, gamma2, intersection);
    }
  }
  return is_hit;
}

/*!
  */
bool SmoothedMesh::testPencilLineIntersection(
    const Ray& ray,
    const std::array<Float, 10>& coefficients,
    const Float alpha,
    const Float beta,
    const Float gamma,
    IntersectionInfo* intersection) const noexcept
{
  const Float a = coefficients[0];
  const Float b = coefficients[1];
  const Float c = coefficients[2];
  const Float d = coefficients[3];
  const Float f = coefficients[4];

  const Float t2 = a * zisc::power<2>(beta) + alpha * (b * alpha - d * beta);
  const Float t1 = 2.0 * a * beta * gamma + alpha * (f * alpha - d * gamma);
  const Float t0 = a * zisc::power<2>(gamma) + c * zisc::power<2>(alpha);

  bool is_hit = false;
  if (t2 != 0.0) {
    const auto result = zisc::solveQuadratic(t2, t1, t0);
    const auto& v_list = std::get<0>(result);
    const uint n = std::get<1>(result);
    for (uint i = 0; i < n; ++i) {
      const Float v = v_list[i];
      const Float u = -(beta * v + gamma) / alpha;
      is_hit = is_hit || testRaySurfaceIntersection(ray, u, v, intersection);
    }
  }
  else if (t1 != 0.0) {
    const Float v = -t0 / t1;
    const Float u = -(beta * v + gamma) / alpha;
    is_hit = testRaySurfaceIntersection(ray, u, v, intersection);
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
