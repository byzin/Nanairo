/*!
  \file file.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_FILE_HPP
#define NANAIRO_FILE_HPP

// Standard C++ library
#include <cstddef>

// Forward declaration
class QByteArray;

namespace nanairo {

//! Write the data to QByteArray  
template <typename Type>
std::size_t write(QByteArray* array, const Type& value) noexcept;

} // namespace nanairo

#include "file-inl.hpp"

#endif // NANAIRO_FILE_HPP
