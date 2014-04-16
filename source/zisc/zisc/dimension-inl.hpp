/*!
  \file dimension-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_DIMENSION_INL_HPP_
#define _ZISC_DIMENSION_INL_HPP_

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
Dimension<Arithmetic, kN>::Dimension()
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
Dimension<Arithmetic, kN>::Dimension(const Types ...values) :
    elements_{values...}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Dimension<Arithmetic, kN>::Dimension(const ArrayType& array) :
    elements_{array}
{
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic& Dimension<Arithmetic, kN>::operator[](const uint index)
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
const Arithmetic& Dimension<Arithmetic, kN>::operator[](const uint index) const
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Dimension<Arithmetic, kN>::data() -> ArrayType&
{
  return elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
auto Dimension<Arithmetic, kN>::data() const -> const ArrayType&
{
  return elements_;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
Arithmetic Dimension<Arithmetic, kN>::get(const uint index) const
{
  return elements_[index];
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
constexpr uint Dimension<Arithmetic, kN>::size()
{
  return kN;
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> inline
void Dimension<Arithmetic, kN>::set(const uint index, const Arithmetic value)
{
  elements_.set(index, value);
}

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN> template <typename ...Types> inline
void Dimension<Arithmetic, kN>::setElements(const Types ...values)
{
  elements_.setElements(values...);
}

} // namespace zisc

#endif // _ZISC_DIMENSION_INL_HPP_
