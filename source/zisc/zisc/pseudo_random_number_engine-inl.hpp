/*!
  \file pseudo_random_number_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP
#define ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP

#include "pseudo_random_number_engine.hpp"
// Standard C++ library
#include <cstdint>
#include <type_traits>
// Zisc
#include "utility.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
auto PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::operator()() noexcept
    -> ResultType
{
  return generate();
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result>
template <typename Arithmetic> inline
Arithmetic PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::operator()(
    const Arithmetic lower, 
    const Arithmetic upper) noexcept
{
  return generate(lower, upper);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
auto PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::generate() noexcept
    -> ResultType
{
  return cast<GeneratorClass*>(this)->generate();
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result>
template <typename Arithmetic> inline
Arithmetic PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::generate(
    const Arithmetic lower, 
    const Arithmetic upper) noexcept
{
  static_assert(std::is_arithmetic<Arithmetic>::value,
                "Arithmetic isn't arithmetic type.");
  return cast<GeneratorClass*>(this)->generate(lower, upper);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
void PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::setSeed(
    const SeedType seed) noexcept
{
  cast<GeneratorClass*>(this)->setSeed(seed);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::PseudoRandomNumberEngine()
    noexcept
{
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::PseudoRandomNumberEngine(
    const PseudoRandomNumberEngine&) noexcept
{
}

} // namespace zisc

#endif // ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP
