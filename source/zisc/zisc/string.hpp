/*!
  \file string.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_STRING_HPP
#define ZISC_STRING_HPP

// Standard C++ library
#include <string>
#include <regex>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \brief Constexprable string class
  \details
  No detailed.
  */
template <typename CharType, uint N>
class BasicString
{
 public:
  //! Create a string
  constexpr BasicString(const CharType (&string)[N]) noexcept;


  //! Lexicographically compares two strings
  constexpr bool operator==(const CharType* other) const noexcept;

  //! Lexicographically compares two strings
  template <uint M>
  constexpr bool operator==(const BasicString<CharType, M>& other) const noexcept;

  //! Lexicographically compares two strings
  constexpr bool operator!=(const CharType* other) const noexcept;

  //! Lexicographically compares two strings
  template <uint M>
  constexpr bool operator!=(const BasicString<CharType, M>& other) const noexcept;

  //! Access specified character
  constexpr CharType& operator[](const uint index) noexcept;

  //! Access specified character
  constexpr const CharType& operator[](const uint index) const noexcept;



  //! Access specified character
  constexpr CharType& get(const uint index) noexcept;

  //! Access specified character
  constexpr const CharType& get(const uint index) const noexcept;

  //! Returns the number of CharType elements in the string
  constexpr uint size() const noexcept;

  //! Return a standard c character array
  const CharType* toCString() const noexcept;

  //! Convert to std::string
  std::string toStdString() const noexcept;

 private:
  //! Initialize a string
  constexpr void initialize(const CharType (&string)[N]) noexcept;


  CharType string_[N] = {'\0'};
};

// Constexpr string aliases
template <uint N>
using String = BasicString<char, N>;


//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const BasicString<CharType, N1>& string1, 
    const BasicString<CharType, N2>& string2) noexcept;

//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const char (&string1)[N1],
    const BasicString<CharType, N2>& string2) noexcept;

//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> operator+(
    const BasicString<CharType, N1>& string1, 
    const char (&string2)[N2]) noexcept;

//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const BasicString<CharType, N1>& string1, 
    const BasicString<CharType, N2>& string2) noexcept;

//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const char (&string1)[N1],
    const BasicString<CharType, N2>& string2) noexcept;

//! Concatenates two strings
template <typename CharType, uint N1, uint N2>
constexpr BasicString<CharType, N1 + N2 - 1> concatenate(
    const BasicString<CharType, N1>& string1, 
    const char (&string2)[N2]) noexcept;


//! Convert the string to String class
template <uint N>
constexpr String<N> toString(const char (&string)[N]) noexcept;


/*!
  \details
  No detailed.
  */
class ValueString
{
 public:
  //! Create a ValueString instance
  ValueString() noexcept;


  //! Check quickly if a string represents an boolean 
  bool isBoolean(const char* string) const noexcept;

  //! Check quickly if a string represents an boolean
  bool isBoolean(const std::string& string) const noexcept;

  //! Check quickly if a string represents a float
  bool isFloat(const char* string) const noexcept;

  //! Check quickly if a string represents a float
  bool isFloat(const std::string& string) const noexcept;

  //! Check quickly if a string represents an integer
  bool isInteger(const char* string) const noexcept;

  //! Check quickly if a string represents an integer
  bool isInteger(const std::string& string) const noexcept;

  //! Check quickly if a string represents a standard C++ string
  bool isCxxString(const char* string) const noexcept;

  //! Check quickly if a string represents a standard C++ string
  bool isCxxString(const std::string& string) const noexcept;

 private:
  using RegexOptionType = std::regex_constants::syntax_option_type;


  //! Return the regex option
  static constexpr RegexOptionType regexOption() noexcept;


  std::regex float_pattern_;
  std::regex int_pattern_;
  std::regex string_pattern_;
};

//! Convert a string to an boolean
bool toBoolean(const char* string) noexcept;

//! Convert a string to an boolean
bool toBoolean(const std::string& string) noexcept;

//! Convert a string to a floating point
template <typename Float>
Float toFloat(const char* string) noexcept;

//! Convert a string to a floating point
template <typename Float>
Float toFloat(const std::string& string) noexcept;

//! Convert a string to an integer
template <typename Integer> 
Integer toInteger(const char* string) noexcept;

//! Convert a string to an integer
template <typename Integer> 
Integer toInteger(const std::string& string) noexcept;

//! Convert a string to a c++ value
template <typename Type>
Type toCxxValue(const char* string) noexcept;

//! Convert a string to a c++ value
template <typename Type>
Type toCxxValue(const std::string& string) noexcept;

//! Convert a string to a standard C++ string
std::string toCxxString(const char* string) noexcept;

//! Convert a string to a standard C++ string
std::string toCxxString(const std::string& string) noexcept;

//! Check if a string represents an boolean
bool isBooleanString(const char* string) noexcept;

//! Check if a string represents an boolean
bool isBooleanString(const std::string& string) noexcept;

//! Check if a string represents a floating point
bool isFloatString(const char* string) noexcept;

//! Check if a string represents a floating point
bool isFloatString(const std::string& string) noexcept;

//! Check if a string represents an integer
bool isIntegerString(const char* string) noexcept;

//! Check if a string represents an integer
bool isIntegerString(const std::string& string) noexcept;

//! Check if a string represents a standard C++ string
bool isCxxStringString(const char* string) noexcept;

//! Check if a string represents a standard C++ string
bool isCxxStringString(const std::string& string) noexcept;

//! Removes all spaces from the beginning and end of the supplied string.
std::string& trim(std::string* string) noexcept;

//! Removes all spaces from the beginning and end of the supplied string.
std::string trimmed(const std::string& string) noexcept;

//! Replace CR or CR + LF to LF
void replaceLineFeedCodeToUnixStyle(std::string* text) noexcept;

//! Replace line feed code (CR or LF) to spaces
void replaceLineFeedCodeToSpaces(std::string* text) noexcept;

} // namespace zisc

#include "string-inl.hpp"

namespace zisc {

//! Return the regex pattern of boolean
constexpr auto getBooleanPattern() noexcept
{
  return zisc_string::ValueStringPattern::boolean();
}

//! Return the regex pattern of floating-point
constexpr auto getFloatPattern() noexcept 
{
  return zisc_string::ValueStringPattern::floatingPoint();
}

//! Return the regex pattern of integer
constexpr auto getIntegerPattern() noexcept
{
  return zisc_string::ValueStringPattern::integer();
}

//! Return the regex pattern of string
constexpr auto getStringPattern() noexcept
{
  return zisc_string::ValueStringPattern::string();
}

} // namespace zisc

#endif // ZISC_STRING_HPP
