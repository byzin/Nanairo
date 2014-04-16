/*!
  \file string-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_STRING_INL_HPP_
#define _ZISC_STRING_INL_HPP_

#include "string.hpp"
// Standard C++ library
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <regex>
#include <string>
#include <utility>
// Zisc
#include "utility.hpp"
#include "type_traits.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr BasicString<CharType, N>::BasicString(const CharType (&string)[N])
{
  initialize(string);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr bool BasicString<CharType, N>::operator==(const CharType* other) const
{
  uint m = 0;
  while (other[m] != '\0')
    ++m;
  ++m;

  bool flag = (N == m);
  if (flag) {
    for (uint i = 0; i < N; ++i) {
      if (string_[i] != other[i]) {
        flag = false;
        break;
      }
    }
  }
  return flag;
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> template <uint M> inline
constexpr bool BasicString<CharType, N>::operator==(
    const BasicString<CharType, M>& other) const
{
  bool flag = (N == M);
  if (flag) {
    for (uint i = 0; i < N; ++i) {
      if (string_[i] != other[i]) {
        flag = false;
        break;
      }
    }
  }
  return flag;
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr bool BasicString<CharType, N>::operator!=(const CharType* other) const
{
  return !(*this == other);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> template <uint M> inline
constexpr bool BasicString<CharType, N>::operator!=(
    const BasicString<CharType, M>& other) const
{
  return !(*this == other);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr CharType& BasicString<CharType, N>::operator[](const uint index)
{
  return string_[index];
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr const CharType& BasicString<CharType, N>::operator[](const uint index) const
{
  return string_[index];
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr uint BasicString<CharType, N>::size() const
{
  return N - 1;
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
const CharType* BasicString<CharType, N>::toCString() const
{
  return string_;
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
std::string BasicString<CharType, N>::toStdString() const
{
  return std::string{string_};
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N> inline
constexpr void BasicString<CharType, N>::initialize(const CharType (&string)[N])
{
  for (uint i = 0; i < size(); ++i)
    string_[i] = string[i];
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const BasicString<CharType, N1>& string1,
    const BasicString<CharType, N2>& string2)
{
  return concatenate(string1, string2);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const char (&string1)[N1],
    const BasicString<CharType, N2>& string2)
{
  return toString(string1) + string2;
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const BasicString<CharType, N1>& string1,
    const char (&string2)[N2])
{
  return string1 + toString(string2);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const BasicString<CharType, N1>& string1,
    const BasicString<CharType, N2>& string2)
{
  constexpr uint size = N1 + N2 - 1;
  char string[size] = {'\0'};
  for (uint i = 0; i < string1.size(); ++i)
    string[i] = string1[i];
  for (uint i = 0; i < string2.size(); ++i)
    string[i + string1.size()] = string2[i];
  return BasicString<CharType, size>{string};
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const char (&string1)[N1],
    const BasicString<CharType, N2>& string2)
{
  return concatenate(toString(string1), string2);
}

/*!
  \details
  No detailed.
  */
template <typename CharType, uint N1, uint N2> inline
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const BasicString<CharType, N1>& string1,
    const char (&string2)[N2])
{
  return concatenate(string1, toString(string2));
}

/*!
  \details
  No detailed.
  */
template <uint N> inline
constexpr String<N> toString(const char (&string)[N])
{
  return String<N>{string};
}

namespace zisc_string {

/*!
  \details
  No detailed.
  */
class ValueStringPattern
{
 private:
  // Sub-patterns
  static constexpr auto digit_ = toString(R"(\d)");
  static constexpr auto digits_ = digit_ + "+";
  static constexpr auto frac_ = R"(\.)" + digits_;
  static constexpr auto e_ = toString(R"(e|E|e\+|E\+|e-|E-)");
  static constexpr auto exp_ = "(?:" + e_ + ")" + digits_;
  static constexpr auto character_ = toString(R"([^"\\[:cntrl:]])");
  static constexpr auto escaped_ = toString(R"(\\["\\/bfnrt])");
  // Patterns
  static constexpr auto boolean_ = toString("true|false");
//  static constexpr auto int_ = "-?(?:" + digit_ + "|[1-9]" + digits_ + ")";
  static constexpr auto int_ = toString(R"(-?(?:0|[1-9]\d*))");
  static constexpr auto float_ = int_ + "(?:" + frac_ + ")?" + "(?:" + exp_ + ")?";
  static constexpr auto string_ = R"("(?:)" + character_ + "|" + escaped_ + R"()*")";

 public:
  static constexpr auto boolean() -> decltype(boolean_)
  {
    constexpr auto boolean = boolean_;
    return boolean;
  }

  static constexpr auto floatingPoint() -> decltype(float_)
  {
    constexpr auto floating_point = float_;
    return floating_point;
  }

  static constexpr auto integer() -> decltype(int_)
  {
    constexpr auto integer = int_;
    return integer;
  }

  static constexpr auto string() -> decltype(string_)
  {
    constexpr auto string = string_;
    return string;
  }
};

} // namespace zisc_string

/*!
  \details
  No detailed.
  */
inline
ValueString::ValueString() : 
    float_pattern_{zisc_string::ValueStringPattern::floatingPoint().toCString(),
                   regexOption()},
    int_pattern_{zisc_string::ValueStringPattern::integer().toCString(), 
                 regexOption()},
    string_pattern_{zisc_string::ValueStringPattern::string().toCString(),
                    regexOption()}
{
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isBoolean(const char* string) const
{
  return isBooleanString(string);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isBoolean(const std::string& string) const
{
  return isBooleanString(string);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isFloat(const char* string) const
{
  return std::regex_match(string, float_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isFloat(const std::string& string) const
{
  return std::regex_match(string, float_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isInteger(const char* string) const
{
  return std::regex_match(string, int_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isInteger(const std::string& string) const
{
  return std::regex_match(string, int_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isCxxString(const char* string) const
{
  return std::regex_match(string, string_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
bool ValueString::isCxxString(const std::string& string) const
{
  return std::regex_match(string, string_pattern_);
}

/*!
  \details
  No detailed.
  */
inline
constexpr auto ValueString::regexOption() -> RegexOptionType
{
  return std::regex_constants::nosubs |
         std::regex_constants::optimize |
         std::regex_constants::ECMAScript;
}

/*!
 \details
 No detailed.
 */
inline
bool toBoolean(const char* string)
{
  return (toString("true") == string);
}

/*!
 \details
 No detailed.
 */
inline
bool toBoolean(const std::string& string)
{
  return (toString("true") == string.c_str());
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float toFloat(const char* string)
{
  static_assert(kIsFloat<Float>, "Float must be float type.");
  return cast<Float>(std::atof(string));
}

/*!
  \details
  No detailed.
  */
template <typename Float> inline
Float toFloat(const std::string& string)
{
  static_assert(kIsFloat<Float>, "Float must be float type.");
  return cast<Float>(std::stod(string));
}

/*!
  \details
  No detailed.
  */
template <> inline
float toFloat(const std::string& string)
{
  return std::stof(string);
}

/*!
  \details
  No detailed.
  */
template <> inline
long double toFloat(const std::string& string)
{
  return std::stold(string);
}

/*!
  \details
  No detailed.
  */
template <typename Integer> inline
Integer toInteger(const char* string)
{
  static_assert(kIsInteger<Integer>, "Integer must be integer type.");
  return cast<Integer>(std::atoi(string));
}

/*!
  \details
  No detailed.
  */
template <> inline
long toInteger(const char* string)
{
  return std::atol(string);
}

/*!
  \details
  No detailed.
  */
template <> inline
long long toInteger(const char* string)
{
  return std::atoll(string);
}

/*!
  \details
  No detailed.
  */
template <typename Integer> inline
Integer toInteger(const std::string& string)
{
  static_assert(kIsInteger<Integer>, "Integer must be integer type.");
  return cast<Integer>(std::stoi(string));
}

/*!
  \details
  No detailed.
  */
template <> inline
long toInteger(const std::string& string)
{
  return std::stol(string);
}

/*!
  \details
  No detailed.
  */
template <> inline
long long toInteger(const std::string& string)
{
  return std::stoll(string);
}

/*!
  \details
  No detailed.
  */
template <> inline
unsigned int toInteger(const std::string& string)
{
  return cast<unsigned int>(std::stoul(string));
}

/*!
  \details
  No detailed.
  */
template <> inline
unsigned long toInteger(const std::string& string)
{
  return std::stoul(string);
}

/*!
  \details
  No detailed.
  */
template <> inline
unsigned long long toInteger(const std::string& string)
{
  return std::stoull(string);
}

/*!
  \details
  No detailed.
  */
inline
std::string toCxxString(const char* string)
{
  return toCxxString(std::string{string});
}

/*!
  \details
  No detailed.
  */
inline
std::string toCxxString(const std::string& string)
{
  return string.substr(1, string.size() - 2);
}

namespace zisc_string {

template <typename String, typename Type> inline
Type toCxxValue(const String& string, EnableIfBoolean<Type> = kEnabler)
{
  return toBoolean(string);
}

template <typename String, typename Type> inline
Type toCxxValue(const String& string, EnableIfFloat<Type> = kEnabler)
{
  return toFloat<Type>(string);
}

template <typename String, typename Type> inline
Type toCxxValue(const String& string, EnableIfInteger<Type> = kEnabler)
{
  return toInteger<Type>(string);
}

template <typename String, typename Type> inline
Type toCxxValue(const String& string, EnableIfSame<std::string, Type> = kEnabler)
{
  return toCxxString(string);
}

} // namespace zisc_string

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type toCxxValue(const char* string)
{
  return zisc_string::toCxxValue<const char*, Type>(string);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
Type toCxxValue(const std::string& string)
{
  return zisc_string::toCxxValue<std::string, Type>(string);
}

namespace zisc_string {

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool isFloatString(const Type& string)
{
  using Pattern = ValueStringPattern;
  constexpr auto regex_option =  std::regex_constants::nosubs | 
                                 std::regex_constants::ECMAScript;
  std::regex float_pattern{Pattern::floatingPoint().toCString(), regex_option};
  return std::regex_match(string, float_pattern);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool isIntegerString(const Type& string)
{
  using Pattern = ValueStringPattern;
  constexpr auto regex_option =  std::regex_constants::nosubs | 
                                 std::regex_constants::ECMAScript;
  std::regex int_pattern{Pattern::integer().toCString(), regex_option};
  return std::regex_match(string, int_pattern);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
bool isCxxStringString(const Type& string)
{
  using Pattern = ValueStringPattern;
  constexpr auto regex_option =  std::regex_constants::nosubs | 
                                 std::regex_constants::ECMAScript;
  std::regex string_pattern{Pattern::string().toCString(), regex_option};
  return std::regex_match(string, string_pattern);
}

} // namespace zisc_string

/*!
 \details
 No detailed.
 */
inline
bool isBooleanString(const char* string)
{
  return (toString("true") == string) || 
         (toString("false") == string);
}

/*!
 \details
 No detailed.
 */
inline
bool isBooleanString(const std::string& string)
{
  return (toString("true") == string.c_str()) || 
         (toString("false") == string.c_str());
}

/*!
 \details
 No detailed.
 */
inline
bool isFloatString(const char* string)
{
  return zisc_string::isFloatString(string);
}

/*!
 \details
 No detailed.
 */
inline
bool isFloatString(const std::string& string)
{
  return zisc_string::isFloatString(string);
}

/*!
 \details
 No detailed.
 */
inline
bool isIntegerString(const char* string)
{
  return zisc_string::isIntegerString(string);
}

/*!
 \details
 No detailed.
 */
inline
bool isIntegerString(const std::string& string)
{
  return zisc_string::isIntegerString(string);
}

/*!
 \details
 No detailed.
 */
inline
bool isCxxStringString(const char* string)
{
  return zisc_string::isCxxStringString(string);
}

/*!
 \details
 No detailed.
 */
inline
bool isCxxStringString(const std::string& string)
{
  return zisc_string::isCxxStringString(string);
}

/*!
  \details
  No detailed.
  */
inline
std::string& trim(std::string* string)
{
  using std::find_if;

  auto& str = *string;
  // Erase head spaces
  auto head = 
      find_if(str.begin(), str.end(), [](char& ch) {return !isspace(ch);});
  str.erase(str.begin(), head);
  // Erase tail spaces
  auto tail = 
      find_if(str.rbegin(), str.rend(), [](char& ch) {return !isspace(ch);}).base();
  str.erase(tail, str.end());
  return str;
}

/*!
  \details
  No detailed.
  */
inline
std::string trimmed(const std::string& string)
{
  auto str = string;
  trim(&str);
  return str;
}

} // namespace zisc

#endif // _Z_STRING_INL_HPP__
