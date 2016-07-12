/*!
  \file file-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILE_INL_HPP
#define NANAIRO_FILE_INL_HPP

// Standard C++ library
#include <cstddef>
// Zisc
#include "zisc/utility.hpp"
// Qt
#include <QByteArray>

namespace nanairo {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
std::size_t write(QByteArray* array, const Type& value) noexcept
{
  using zisc::cast;

  constexpr int size = cast<int>(sizeof(Type));
  const char* data = zisc::treatAs<const char*>(&value);
  array->append(data, size);

  return sizeof(Type);
}

} // namespace nanairo

#endif // NANAIRO_FILE_INL_HPP
