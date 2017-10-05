/*!
  \file intersection_test_result-inl.hpp
  \author Sho Ikeda
  */

#ifndef NANAIRO_INTERSECTION_TEST_RESULT_INL_HPP
#define NANAIRO_INTERSECTION_TEST_RESULT_INL_HPP

#include "intersection_test_result.hpp"
// Standard C++ library
#include <limits>
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
IntersectionTestResult::IntersectionTestResult() noexcept :
    distance_{failureDistance()}
{
}

/*!
  */
inline
IntersectionTestResult::IntersectionTestResult(const Float d) noexcept :
    distance_{d}
{
}

/*!
  */
inline
IntersectionTestResult::operator bool() const noexcept
{
  return isSuccess();
}

/*!
  */
inline
constexpr Float IntersectionTestResult::failureDistance() noexcept
{
  constexpr Float d = std::numeric_limits<Float>::lowest();
  return d;
}

/*!
  */
inline
bool IntersectionTestResult::isSuccess() const noexcept
{
  const bool is_success = rayDistance() != failureDistance();
  return is_success;
}

/*!
  */
inline
Float IntersectionTestResult::rayDistance() const noexcept
{
  return distance_;
}


/*!
  */
inline
void IntersectionTestResult::setRayDistance(const Float d) noexcept
{
  distance_ = d;
}

} // namespace nanairo

#endif // NANAIRO_INTERSECTION_TEST_RESULT_INL_HPP
