/*!
  \file pseudo_random_number_engine.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_HPP_
#define _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_HPP_

// Standard C++ library
#include <cstdint>

namespace zisc {

/*!
  \brief Base class of pseudo random number algorithm
  \details
  No detailed.
  */
template <typename GeneratorClass, typename Seed, typename Result>
class PseudoRandomNumberEngine
{
 public:
  using GeneratorType = GeneratorClass;
  using SeedType = Seed;
  using ResultType = Result;


  //! Generate a random number
  ResultType operator()();

  //! Generate a random number x satisfying lower <= x < upper 
  template <typename Arithmetic>
  Arithmetic operator()(const Arithmetic lower, const Arithmetic upper);


  //! Generate a random number
  ResultType generate();

  //! Generate a random number x satisfying lower <= x < upper
  template <typename Arithmetic>
  Arithmetic generate(const Arithmetic lower, const Arithmetic upper);

  //! Set random seed
  void setSeed(const SeedType seed);

 protected:
  //! Initialize the PRN
  PseudoRandomNumberEngine();

  //! Copy a PRN
  PseudoRandomNumberEngine(const PseudoRandomNumberEngine&);

  PseudoRandomNumberEngine& operator=(const PseudoRandomNumberEngine&) = delete;
};

template <typename GeneratorClass>
using RandomNumberEngine = PseudoRandomNumberEngine<GeneratorClass,
                                                    typename GeneratorClass::SeedType,
                                                    typename GeneratorClass::ResultType>;

} // namespace zisc

#include "pseudo_random_number_engine-inl.hpp"

#endif // _ZISC_PSEUDO_RANDOM_NUMBER_ENGINE_HPP_
