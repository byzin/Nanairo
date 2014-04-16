/*!
  \file csv-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_CSV_INL_HPP_
#define _ZISC_CSV_INL_HPP_

#include "csv.hpp"
// Standard C++ library
#include <istream>
#include <list>
#include <regex>
#include <string>
#include <sstream>
#include <tuple>
#include <utility>
// Zisc
#include "error.hpp"
#include "string.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

namespace zisc_csv {

template <typename Type> inline
constexpr auto getValuePattern(EnableIfBoolean<Type> = kEnabler)
{
  return getBooleanPattern();
}

template <typename Type> inline
constexpr auto getValuePattern(EnableIfFloat<Type> = kEnabler)
{
  return getFloatPattern();
}

template <typename Type> inline
constexpr auto getValuePattern(EnableIfInteger<Type> = kEnabler)
{
  return getIntegerPattern();
}

template <typename Type> inline
constexpr auto getValuePattern(EnableIfSame<std::string, Type> = kEnabler)
{
  return getStringPattern();
}

template <typename Type> inline
constexpr auto getCsvPattern()
{
  return R"(\s*()" + getValuePattern<Type>() + R"()\s*)";
}

template <typename Type1, typename Type2, typename ...Types> inline
constexpr auto getCsvPattern()
{
  return getCsvPattern<Type1>() + "," + getCsvPattern<Type2, Types...>();
}

} // namespace zisc_csv 

// public member function

/*!
  \details
  No detailed.
  */
template <typename Type, typename ...Types> inline
Csv<Type, Types...>::Csv() : 
    csv_pattern_{zisc_csv::getCsvPattern<Type, Types...>().toCString(), 
                 std::regex_constants::optimize | std::regex_constants::ECMAScript}
{
}

/*!
 \details
 No detailed.
 */
template <typename Type, typename ...Types> inline
void Csv<Type, Types...>::append(const std::string& csv)
{
  std::istringstream csv_text{csv};
  append(csv_text);
}

/*!
 \details
 No detailed.
 */
template <typename Type, typename ...Types> inline
void Csv<Type, Types...>::append(std::istream& csv,
                                 std::list<std::string>* message_list)
{
  for (std::string line; std::getline(csv, line);) {
    auto&& record = parseCsvLine(line, message_list);
    data_.emplace_back(std::move(record));
  }
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename ...Types> inline
void Csv<Type, Types...>::clear()
{
  data_.clear();
}

/*!
  \details
  No detailed.
  */
template <typename Type, typename ...Types> inline
constexpr uint Csv<Type, Types...>::columnSize()
{
  constexpr uint size = 1 + sizeof...(Types);
  return size;
}

/*!
 \details
 No detailed.
 */
template <typename Type, typename ...Types> inline
auto Csv<Type, Types...>::record(const uint row) const -> const RecordType&
{
  using DiffType = typename std::list<RecordType>::difference_type;
  auto&& record = std::next(data_.begin(), cast<DiffType>(row));
  return *record;
}

/*!
 \details
 No detailed.
 */
template <typename Type, typename ...Types> inline
uint Csv<Type, Types...>::rowSize() const
{
  return data_.size();
}

/*!
 \details
 No detailed.
 */
template <typename Type, typename ...Types> template <uint column> inline
auto Csv<Type, Types...>::get(const uint row) const -> const FieldType<column>&
{
  const auto& record = this->record(row);
  return std::get<column>(record);
}

namespace zisc_csv {

template <uint index, typename RecordType>
struct CsvConverter
{
  using FieldType = typename std::tuple_element<index - 1, RecordType>::type;
  template <typename ...Types>
  static RecordType toCxx(const std::smatch& results, Types&&... record)
  {
    using Converter = CsvConverter<index - 1, RecordType>;
    FieldType field = toCxxValue<FieldType>(results[index]);
    return Converter::toCxx(results, 
                            std::move(field), 
                            std::forward<Types>(record)...);
  }
};

template <typename RecordType>
struct CsvConverter<1, RecordType>
{
  using FieldType = typename std::tuple_element<0, RecordType>::type;
  template <typename ...Types>
  static RecordType toCxx(const std::smatch& results, Types&&... record)
  {
    FieldType field = toCxxValue<FieldType>(results[1]);
    return std::make_tuple(std::move(field), std::forward<Types>(record)...);
  }
};

} // namespace zisc_csv 

/*!
  \details
  No detailed.
  */
template <typename Type, typename ...Types> inline
auto Csv<Type, Types...>::parseCsvLine(const std::string& line,
                                       std::list<std::string>* message_list) const
    -> RecordType
{
  std::smatch results;
  const auto success = std::regex_match(line, results, csv_pattern_);
  if ((message_list != nullptr) && !success) {
    std::ostringstream error;
    error << R"(Parsing ")" << line << R"(" failed.)" << std::endl;
    message_list->emplace_back(error.str());
  }
  if (success) {
    constexpr uint size = columnSize();
    return zisc_csv::CsvConverter<size, RecordType>::toCxx(results);
  }
  else {
    return RecordType{};
  }
}

} // namespace zisc

#endif // _ZISC_CSV_INL_HPP_
