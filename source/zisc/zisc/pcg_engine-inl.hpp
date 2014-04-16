/*!
  \file pcg_engine-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_PCG_ENGINE_INL_HPP_
#define _ZISC_PCG_ENGINE_INL_HPP_

#include "pcg_engine.hpp"
// Standard C++ library
#include <limits>
#include <type_traits>
// Zisc
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
PcgEngine<Base, Algorithm, Seed, Result>::PcgEngine()
{
  constexpr SeedType seed = cast<SeedType>(0xcafef00dd15ea5e5ULL);
  setSeed(seed);
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
PcgEngine<Base, Algorithm, Seed, Result>::PcgEngine(const SeedType seed)
{
  setSeed(seed);
}

namespace pcg {

/*!
  \details
  No detailed.
  */
template <typename SeedType, typename ResultType> inline
ResultType rotateRight(ResultType value, const SeedType rot)
{
  constexpr SeedType bits = cast<SeedType>(sizeof(ResultType) * 8);
  constexpr SeedType mask = bits - 1;
  return (value >> rot) | (value << ((-rot) & mask));
}

template <PcgAlgorithm Algorithm, typename SeedType, typename ResultType>
struct PcgMixin;

/*!
  \details
  No detailed.
  */
template <typename SeedType, typename ResultType>
struct PcgMixin<PcgAlgorithm::XshRr, SeedType, ResultType>
{
  static ResultType output(SeedType internal)
  {
    // Constant values
    constexpr SeedType bits = cast<SeedType>(sizeof(SeedType) * 8);
    constexpr SeedType xtypebits = cast<SeedType>(sizeof(ResultType) * 8);
    constexpr SeedType sparebits = bits - xtypebits;
    constexpr SeedType wantedopbits = xtypebits >= 128 ? 7 : 
                                      xtypebits >=  64 ? 6 : 
                                      xtypebits >=  32 ? 5 : 
                                      xtypebits >=  16 ? 4 :
                                                         3;
    constexpr SeedType opbits = (sparebits >= wantedopbits) ? wantedopbits 
                                                            : sparebits;
    constexpr SeedType amplifier = wantedopbits - opbits;
    constexpr SeedType mask = (1 << opbits) - 1;
    constexpr SeedType topspare = opbits;
    constexpr SeedType bottomspare = sparebits - topspare;
    constexpr SeedType xshift = (topspare + xtypebits) / 2;

    // Calc a random number
    SeedType rot = opbits ? SeedType(internal >> (bits - opbits)) & mask : 0;
    SeedType amprot = (rot << amplifier) & mask;
    internal ^= internal >> xshift;
    ResultType result = cast<ResultType>(internal >> bottomspare);
    result = rotateRight(result, amprot);
    return result;
  }
};

template <typename Type> constexpr Type mcgMultiplier();
template <> inline constexpr uint8 mcgMultiplier() {return 217U;}
template <> inline constexpr uint16 mcgMultiplier() {return 62169U;}
template <> inline constexpr uint32 mcgMultiplier() {return 277803737U;}
template <> inline constexpr uint64 mcgMultiplier() {return 12605985483714917081ULL;}

/*!
  \details
  No detailed.
  */
template <typename SeedType, typename ResultType>
struct PcgMixin<PcgAlgorithm::RxsMXs, SeedType, ResultType>
{
  static ResultType output(SeedType internal)
  {
    // Constant values
    constexpr SeedType xtypebits = cast<SeedType>(sizeof(ResultType) * 8);
    constexpr SeedType bits = cast<SeedType>(sizeof(SeedType) * 8);
    constexpr SeedType opbits = xtypebits >= 128 ? 6 : 
                                xtypebits >=  64 ? 5 : 
                                xtypebits >=  32 ? 4 : 
                                xtypebits >=  16 ? 3 :
                                                   2;
    constexpr SeedType shift = bits - xtypebits;
    constexpr SeedType mask = (1 << opbits) - 1;

    // Calc a random number
    SeedType rshift = opbits ? cast<SeedType>(internal >> (bits - opbits)) & mask : 0;
    internal ^= internal >> (opbits + rshift);
    internal *= mcgMultiplier<ResultType>();
    ResultType result = internal >> shift;
    result ^= result >> ((2U * xtypebits + 2U) / 3U);
    return result;
  }
};

} // namespace pcg 

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
auto PcgEngine<Base, Algorithm, Seed, Result>::generate() -> ResultType
{
  const auto random = pcg::PcgMixin<Algorithm, Seed, Result>::output(state_);
  next();
  return random;
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result>
template <typename Arithmetic> inline
Arithmetic PcgEngine<Base, Algorithm, Seed, Result>::generate(const Arithmetic lower,
                                                              const Arithmetic upper)
{
  static_assert(std::is_arithmetic<Arithmetic>::value,
                "## Arithmetic must be arithmetic type.");
  const auto random = generate();
  return bound(random, lower, upper);
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
constexpr  auto PcgEngine<Base, Algorithm, Seed, Result>::max() -> ResultType
{
  return std::numeric_limits<ResultType>::max();
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
constexpr auto PcgEngine<Base, Algorithm, Seed, Result>::min() -> ResultType
{
  return std::numeric_limits<ResultType>::min();
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
void PcgEngine<Base, Algorithm, Seed, Result>::setSeed(const SeedType seed)
{
  constexpr bool is_lcg = (Base == PcgBase::Lcg);
  state_ = is_lcg ? bump(seed + increment()) : (seed | cast<SeedType>(3U));
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
auto PcgEngine<Base, Algorithm, Seed, Result>::bump(const SeedType state) const
    -> SeedType
{
  return state * multiplier() + increment();
}

namespace zisc_pcg_engine {

template <typename PcgEngine, typename ResultType, typename Integer> inline
Integer pcgBound(const ResultType random,
                 const Integer lower,
                 const Integer upper,
                 EnableIfInteger<Integer> = kEnabler)
{
  const auto range = cast<ResultType>(upper - lower);
  return cast<Integer>(cast<ResultType>(lower) + random % range);
}

template <typename PcgEngine, typename ResultType, typename Float> inline
Float pcgBound(const ResultType random,
               const Float lower,
               const Float upper,
               EnableIfFloat<Float> = kEnabler)
{
  constexpr Float epsilon = std::numeric_limits<Float>::epsilon();
  constexpr Float normalizer = (cast<Float>(1.0) - epsilon) / 
                               cast<Float>(PcgEngine::max());
  const Float u = cast<Float>(random) * normalizer;
  return lower + u * (upper - lower);
}

} // namespace zisc_pcg_engine

namespace pcg {

template <typename Type> constexpr Type increment();
template <> constexpr uint8 increment() {return 77U;}
template <> constexpr uint16 increment() {return 47989U;}
template <> constexpr uint32 increment() {return 2891336453U;}
template <> constexpr uint64 increment() {return 1442695040888963407ULL;}

template <typename Type> constexpr Type multiplier();
template <> constexpr uint8 multiplier() {return 141U;}
template <> constexpr uint16 multiplier() {return 12829U;}
template <> constexpr uint32 multiplier() {return 747796405U;}
template <> constexpr uint64 multiplier() {return 6364136223846793005ULL;}

} // namespace pcg

template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
constexpr auto PcgEngine<Base, Algorithm, Seed, Result>::increment() -> SeedType
{
  constexpr bool is_mcg = (Base == PcgBase::Mcg);
  return is_mcg ? 0 : pcg::increment<SeedType>();
}

template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
constexpr auto PcgEngine<Base, Algorithm, Seed, Result>::multiplier() -> SeedType
{
  return pcg::multiplier<SeedType>();
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result>
template <typename Type> inline
Type PcgEngine<Base, Algorithm, Seed, Result>::bound(const ResultType random,
                                                     const Type lower,
                                                     const Type upper) const
{
  return zisc_pcg_engine::pcgBound<PcgEngine>(random, lower, upper);
}

/*!
  \details
  No detailed.
  */
template <PcgBase Base, PcgAlgorithm Algorithm, typename Seed, typename Result> inline
void PcgEngine<Base, Algorithm, Seed, Result>::next()
{
  state_ = bump(state_);
}

} // namespace zisc

#endif // _ZISC_PCG_ENGINE_INL_HPP_
