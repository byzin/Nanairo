///*!
//  \file binary_file-inl.hpp
//  \author Sho Ikeda
//
//  Copyright (c) 2015 Sho Ikeda
//  This software is released under the MIT License.
//  http://opensource.org/licenses/mit-license.php
//  */
//
//#ifndef _ZISC_BINARY_FILE_INL_HPP_H_
//#define _ZISC_BINARY_FILE_INL_HPP_H_
//
//#include "binary_file.hpp"
//// Standard C++ library
//#include <cstddef>
//#include <fstream>
//#include <ios>
//#include <string>
//#include <utility>
//// Zisc
//#include "utility.hpp"
//
//namespace zisc {
//
///*!
//  \details
//  No detailed.
//  */
//template <BinaryFileOpenMode kFileType> inline
//void BinaryFile<kFileType>::close()
//{
//  binary_file_.close();
//}
//    
///*!
//  \details
//  No detailed.
//  */
//template <BinaryFileOpenMode kFileType> inline
//bool BinaryFile<kFileType>::open(const std::string& file_name)
//{
//  binary_file_.open(file_name, openMode());
//  return !binary_file_.fail();
//}
//
//namespace inner {
//
///*!
//  \brief バイナリファイルから値を読み込みます
//  \details
//  No detailed.
//
//  \param[in] size バッファサイズ
//  \param[out] binary_file バイナリファイル
//  \return 読み込んだ値
//  */
//template <typename Type> inline
//Type read(const std::streamsize size, std::fstream* binary_file)
//{
//  Type buffer;
//  binary_file->read(reinterpret_cast<char*>(&buffer), size);
//  return buffer;
//}
//
///*!
//  \brief バイナリファイルから文字列を読み込みます
//  \details
//  No detailed.
//
//  \param[in] size 文字列サイズ
//  \param[out] binary_file バイナリファイル
//  \return 読み込んだ文字列
//  */
//template <> inline
//std::string read(const std::streamsize size, std::fstream* binary_file)
//{
//  std::string buffer;
//  buffer.resize(cast<std::size_t>(size));
//  binary_file->read(&buffer[0], size);
//  return buffer;
//}
//
///*!
//  \brief バイナリファイルに値を書き込みます
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  \param[out] binary_file バイナリファイル
//  */
//template <typename Type> inline
//void write(const Type& buffer, std::fstream* binary_file)
//{
//  binary_file->write(reinterpret_cast<const char*>(&buffer), sizeof(Type));
//}
//
///*!
//  \brief バイナリファイルに文字列を書き込みます
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  \param[out] binary_file バイナリファイル
//  */
//inline
//void write(const std::string& buffer, std::fstream* binary_file)
//{
//  (*binary_file) << buffer;
//}
//
///*!
//  \brief バイナリファイルに文字列を書き込みます
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  \param[out] binary_file バイナリファイル
//  */
//inline
//void write(const char* const& buffer, std::fstream* binary_file)
//{
//  (*binary_file) << buffer;
//}
//
///*!
//  \brief バイナリファイルに文字列を書き込みます
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  \param[out] binary_file バイナリファイル
//  */
//template <std::size_t N> inline
//void write(const char (&buffer)[N], std::fstream* binary_file)
//{
//  (*binary_file) << buffer;
//}
//
//} // namespace inner
//
///*!
// \details
// No detailed.
//
// \param[in] size バッファサイズ
// \return 読み込んだ値
// */
//template <BinaryFileOpenMode kFileType> template <typename Type> inline
//Type BinaryFile<kFileType>::read(const std::streamsize size)
//{
//  static_assert(kFileType != BinaryFileOpenMode::kWrite,"## write only file can't read.");
//  return inner::read<Type>(size, &binary_file_);
//}
//
///*!
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  \param[in] buffers 書き込む値
//  */
//template <BinaryFileOpenMode kFileType> 
//template <typename Type, typename ...Types> inline
//void BinaryFile<kFileType>::write(const Type& buffer, const Types& ...buffers)
//{
//  static_assert(kFileType != BinaryFileOpenMode::kRead, "## read only file can't write.");
//  write(buffer);
//  write(buffers...);
//}
//
///*!
//  \details
//  No detailed.
//
//  \param[in] buffer 書き込む値
//  */
//template <BinaryFileOpenMode kFileType> template <typename Type> inline
//void BinaryFile<kFileType>::write(const Type& buffer)
//{
//  static_assert(kFileType != BinaryFileOpenMode::kRead, "## read only file can't write.");
//  inner::write(buffer, &binary_file_);
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <> inline
//constexpr std::ios_base::openmode BinaryFile<BinaryFileOpenMode::kRead>::openMode()
//{
//  return std::ios_base::binary | std::ios_base::in;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <> inline
//constexpr std::ios_base::openmode BinaryFile<BinaryFileOpenMode::kWrite>::openMode()
//{
//  return std::ios_base::binary | std::ios_base::out;
//}
//
///*!
//  \details
//  No detailed.
//  */
//template <> inline
//constexpr std::ios_base::openmode BinaryFile<BinaryFileOpenMode::kReadWrite>::
//    openMode()
//{
//  return std::ios_base::binary | std::ios_base::in | std::ios_base::out;
//}
//
//} // namespace zisc
//
//#endif // _ZISC_BINARY_FILE_INL_HPP_H_
