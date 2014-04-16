/*
  \file csv.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_CSV_HPP_
#define _ZISC_CSV_HPP_

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
  Csv();


  //! Add values
  void append(const std::string& csv);

  //! Add values
  void append(std::istream& csv, std::list<std::string>* message_list = nullptr);

  //! Clear all csv data
  void clear();

  //! Return the column size
  static constexpr uint columnSize();

  //! Return the record by the row
  const RecordType& record(const uint row) const;

  //! Return the row size
  uint rowSize() const;

  //! Get value
  template <uint column>
  const FieldType<column>& get(const uint row) const;

 private:
  //! Parse csv line
  RecordType parseCsvLine(const std::string& line,
                          std::list<std::string>* message_list) const;


  std::list<RecordType> data_;
  std::regex csv_pattern_;
};

} // namespace zisc

#include "csv-inl.hpp"

#endif // _ZISC_CSV_HPP_
