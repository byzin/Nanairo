/*!
  \file pseudo_random_number_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP_
#define _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP_

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
auto PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::operator()()
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
    const Arithmetic lower, const Arithmetic upper)
{
  return generate(lower, upper);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
auto PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::generate()
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
    const Arithmetic lower, const Arithmetic upper)
{
  static_assert(std::is_arithmetic<Arithmetic>::value,
                "## Arguments must be arithmetic type.");
  return cast<GeneratorClass*>(this)->generate(lower, upper);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
void PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::setSeed(
    const SeedType seed)
{
  cast<GeneratorClass*>(this)->setSeed(seed);
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::PseudoRandomNumberEngine()
{
}

/*!
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result> inline
PseudoRandomNumberEngine<GeneratorClass, Seed, Result>::PseudoRandomNumberEngine(
    const PseudoRandomNumberEngine&)
{
}

} // namespace zisc

#endif // _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_INL_HPP_
