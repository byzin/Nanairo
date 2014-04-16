/*!
  \file file.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _NANAIRO_FILE_HPP_
#define _NANAIRO_FILE_HPP_

// Standard C++ library
#include <cstddef>

// Forward declaration
class QByteArray;

namespace nanairo {

//! Write the data to QByteArray  
template <typename Type>
std::size_t write(QByteArray* array, const Type& value);

} // namespace nanairo

#include "file-inl.hpp"

#endif // _NANAIRO_FILE_HPP_
