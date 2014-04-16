///*!
//  \file binary_file.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _ZISC_BINARY_FILE_HPP_
//#define _ZISC_BINARY_FILE_HPP_
//
//// Standard C++ library
//#include <fstream>
//#include <string>
//// Zisc
//#include "non_copyable.hpp"
//#include "utility.hpp"
//
//namespace zisc {
//
///*!
//  \brief File  of binary files
//  \details
//  No detailed.
//  */
//enum class BinaryFileOpenMode
//{
//  kRead,
//  kWrite,
//  kReadWrite
//};
//
///*!
//  \brief BinaryFile manipulates binary file
//  \details
//  No detailed.
//  */
//template <BinaryFileOpenMode kFileType>
//class BinaryFile : public NonCopyable
//{
// public:
//  //! Close file
//  void close();
//
//  //! Open file
//  bool open(const std::string& file_name);
//
//  //! Read a value
//  template <typename Type>
//  Type read(const std::streamsize size = cast<std::streamsize>(sizeof(Type)));
//
//  //! Write values
//  template <typename Type, typename ...Types>
//  void write(const Type& buffer, const Types& ...buffers);
//
//  //! Write a value
//  template <typename Type>
//  void write(const Type& buffer);
//
// private:
//  //! Return the open mode of binary file
//  static constexpr std::ios_base::openmode openMode();
//
//
//  std::fstream binary_file_;
//};
//
//// BinaryFile aliases
//using ReadBinary = BinaryFile<BinaryFileOpenMode::kRead>;
//using WriteBinary = BinaryFile<BinaryFileOpenMode::kWrite>;
//using ReadWriteBinary = BinaryFile<BinaryFileOpenMode::kReadWrite>;
//
//} // namespace zisc
//
//#include "binary_file-inl.hpp"
//
//#endif // _ZISC_BINARY_FILE_HPP_
