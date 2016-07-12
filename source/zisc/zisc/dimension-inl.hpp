/*!
  \file dimension-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_DIMENSION_INL_HPP
#define ZISC_DIMENSION_INL_HPP

#include "dimension.hpp"
// Zisc
#include "arithmetic_array.hpp"
#include "math.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Dimension<Arithmetic, kN>::Dimension() noexcept
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Dimension<Arithmetic, kN>::Dimension(const Types ...values) noexcept :
    elements_{values...}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Dimension<Arithmetic, kN>::Dimension(const ArrayType& array) noexcept :
    elements_{array}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& Dimension<Arithmetic, kN>::operator[](const uint index) noexcept
{
  return get(index);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& Dimension<Arithmetic, kN>::operator[](const uint index) const noexcept
{
  return get(index);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Dimension<Arithmetic, kN>::data() noexcept -> ArrayType&
{
  return elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Dimension<Arithmetic, kN>::data() const noexcept -> const ArrayType&
{
  return elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& Dimension<Arithmetic, kN>::get(const uint index) noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& Dimension<Arithmetic, kN>::get(const uint index) const noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
constexpr uint Dimension<Arithmetic, kN>::size() noexcept
{
  return kN;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void Dimension<Arithmetic, kN>::set(const uint index, const Arithmetic value) noexcept
{
  ZISC_ASSERT(index < size(), "The index is out of range.");
  elements_.set(index, value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
void Dimension<Arithmetic, kN>::setElements(const Types ...values) noexcept
{
  elements_.setElements(values...);
}

} // namespace zisc

#endif // ZISC_DIMENSION_INL_HPP
