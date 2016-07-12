/*!
  \file xorshift_engine.hpp
  \author Sho Ikeda
  
  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_XORSHIFT_ENGINE_HPP
#define ZISC_XORSHIFT_ENGINE_HPP

// Zisc
#include "pseudo_random_number_engine.hpp"
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
 \brief Xorshift random number generator
 \details
 No detailed.
 */
template <typename Type>
class XorshiftEngine : 
    public PseudoRandomNumberEngine<XorshiftEngine<Type>, Type, Type>
{
 public:
  using SeedType = Type;
  using ResultType = Type;
  

  //! Initialize generator using default random seed
  XorshiftEngine() noexcept;

  //! Initialize generator
  XorshiftEngine(const SeedType seed) noexcept;


  //! Generate a random number
  ResultType generate() noexcept;

  //! Generate a bounded random number x satisfying lower <= x < upper
  template <typename Arithmetic>
  Arithmetic generate(const Arithmetic lower, const Arithmetic upper) noexcept;

  //! Return the largest possible value in the output range
  static constexpr ResultType max() noexcept;

  //! Return the smallest possible value in the output range
  static constexpr ResultType min() noexcept;

  //! Set random seed
  void setSeed(const SeedType seed) noexcept;

 private:
  //! Return the bounded number
  template <typename Float>
  Float bound(const ResultType random, 
              const Float lower, 
              const Float upper,
              EnableIfFloat<Float> = kEnabler) const noexcept;

  
  static constexpr uint N = 16 / sizeof(SeedType);


  SeedType seed_[N];
};

using Xorshift32 = XorshiftEngine<uint32>;
using Xorshift128Plus = XorshiftEngine<uint64>;

} // namespace zisc

#include "xorshift_engine-inl.hpp"

#endif // ZISC_XORSHIFT_ENGINE_HPP
