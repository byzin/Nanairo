/*!
  \file xsadd_engine.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_XSADD_ENGINE_HPP_
#define _ZISC_XSADD_ENGINE_HPP_

// Standard C++ library
#include <cstddef>
#include <cstdint>
#include <vector>
// Zisc
#include "pseudo_random_number_engine.hpp"
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \brief Xorshift-add random number generator
  \details
  No detailed.
  */
class XsaddEngine : public PseudoRandomNumberEngine<XsaddEngine, uint32, uint32>
{
 public:
  using SeedType = uint32;
  using ResultType = uint32;


  //! Initialize generator using defualt random seed
  XsaddEngine();

  //! Initialize generator
  XsaddEngine(const uint32 seed);


  //! Generate a random number
  uint32 generate();

  //! Generate a bounded random number x satisfying lower <= x < upper 
  template <typename Float>
  Float generate(const Float lower, const Float upper,
                 EnableIfFloat<Float> = kEnabler);

  //! Generate double precision floating point number x (0.0 <= x < 1.0)
  double generateFloat();

  //! Return the largest possible value in the output range
  static constexpr uint32 max();

  //! Return the sammlest possible value in the output range
  static constexpr uint32 min();

  //! Set random seed
  void setSeed(const uint32 seed);

  //! Set random seed
  void setSeed(const std::vector<uint32>& seed_array);

 private:
  //! Return the bounded random number
  template <typename Float>
  Float bound(const double random, const Float lower, const Float upper,
              EnableIfFloat<Float> = kEnabler);

  //! Initialize
  uint32 initializeValue1(const uint32 x) const;

  //! Initialize
  uint32 initializeValue2(const uint32 x) const;

  //! Change internal state
  void nextState();

  //! Certificate period
  void periodCertification();


  static constexpr uint loop_ = 8;


  uint32 state_[4];
};

} // namespace zisc

#include "xsadd_engine-inl.hpp"

#endif // _ZISC_XSADD_ENGINE_HPP_
