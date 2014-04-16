/*!
  \file dimension.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_DIMENSION_HPP_
#define _ZISC_DIMENSION_HPP_

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
  Dimension();

  //! Initialize with values
  template <typename ...Types>
  Dimension(const Types ...values);

  //! Initialize with array
  Dimension(const ArrayType& array);


  //! Get the reference by index.
  Arithmetic& operator[](const uint index);

  //! Get the reference by index.
  const Arithmetic& operator[](const uint index) const;

  //! Get array reference
  ArrayType& data();

  //! Get array reference
  const ArrayType& data() const;

  //! Get an element by index.
  Arithmetic get(const uint index) const;

  //! Return the number of elements
  static constexpr uint size();

  //! Set value
  void set(const uint index, const Arithmetic value);

  //! Set values
  template <typename ...Types>
  void setElements(const Types ...values);

 private:
  ArrayType elements_;
};

} // namespace zisc

#include "dimension-inl.hpp"

#endif // _ZISC_DIMENSION_HPP_
