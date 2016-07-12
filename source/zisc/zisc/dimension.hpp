/*!
  \file dimension.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_DIMENSION_HPP
#define ZISC_DIMENSION_HPP

// Zisc
#include "arithmetic_array.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename Arithmetic, uint kN>
class Dimension
{
 public:
  using ArrayType = ArithmeticArray<Arithmetic, kN>;
  using ValueType = Arithmetic;


  //! Initialize with 0
  Dimension() noexcept;

  //! Initialize with values
  template <typename ...Types>
  Dimension(const Types ...values) noexcept;

  //! Initialize with array
  Dimension(const ArrayType& array) noexcept;


  //! Return the reference by index.
  Arithmetic& operator[](const uint index) noexcept;

  //! Return the reference by index.
  const Arithmetic& operator[](const uint index) const noexcept;

  //! Return array reference
  ArrayType& data() noexcept;

  //! Return array reference
  const ArrayType& data() const noexcept;

  //! Return the reference by index.
  Arithmetic& get(const uint index) noexcept;

  //! Return the reference by index.
  const Arithmetic& get(const uint index) const noexcept;

  //! Return the number of elements
  static constexpr uint size() noexcept;

  //! Set value
  void set(const uint index, const Arithmetic value) noexcept;

  //! Set values
  template <typename ...Types>
  void setElements(const Types ...values) noexcept;

 private:
  ArrayType elements_;
};

} // namespace zisc

#include "dimension-inl.hpp"

#endif // ZISC_DIMENSION_HPP
