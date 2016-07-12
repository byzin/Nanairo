/*
  \file csv.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_CSV_HPP
#define ZISC_CSV_HPP

// Standard C++ library
#include <istream>
#include <list>
#include <regex>
#include <string>
#include <tuple>
// Zisc
#include "non_copyable.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
 \brief Manipulate CSV file
 \details
 No detailed.
 */
template <typename Type, typename ...Types>
class Csv : public NonCopyable
{
 public:
  using RecordType = std::tuple<Type, Types...>;
  template <uint index>
  using FieldType = typename std::tuple_element<index, RecordType>::type;


  //! Initialize CSV
  Csv() noexcept;


  //! Add values
  void append(const std::string& csv,
              std::list<std::string>* message_list = nullptr) noexcept;

  //! Add values
  void append(std::istream& csv, 
              std::list<std::string>* message_list = nullptr) noexcept;

  //! Clear all csv data
  void clear() noexcept;

  //! Return the column size
  static constexpr uint columnSize() noexcept;

  //! Return the record by the row
  const RecordType& record(const uint row) const noexcept;

  //! Return the row size
  uint rowSize() const noexcept;

  //! Get value
  template <uint column>
  const FieldType<column>& get(const uint row) const noexcept;

 private:
  //! Parse csv line
  RecordType parseCsvLine(const std::string& line,
                          std::list<std::string>* message_list) const noexcept;


  std::list<RecordType> data_;
  std::regex csv_pattern_;
};

} // namespace zisc

#include "csv-inl.hpp"

#endif // ZISC_CSV_HPP
