/*!
  \file intersection_test_result.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_INTERSECTION_TEST_RESULT_HPP
#define NANAIRO_INTERSECTION_TEST_RESULT_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
class IntersectionTestResult
{
 public:
  //! Create a failure result
  IntersectionTestResult() noexcept;

  //! Create a success result with the distance
  IntersectionTestResult(const Float d) noexcept;


  //! Check if the result is success
  explicit operator bool() const noexcept;


  //! Return the failure distance
  static constexpr Float failureDistance() noexcept;

  //! Check if the result is success
  bool isSuccess() const noexcept;

  //! Return the distance of the intersection point
  Float rayDistance() const noexcept;

  //! Set the distance of the intersection point
  void setRayDistance(const Float d) noexcept;

 private:
  Float distance_;
};

} // namespace nanairo

#include "intersection_test_result-inl.hpp"

#endif // NANAIRO_INTERSECTION_TEST_RESULT_HPP
