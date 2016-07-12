/*!
  \file json_value.hpp
  \author Sho Ikeda
  
  nullCopyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_VALUE_HPP
#define ZISC_JSON_VALUE_HPP

// Standard C++ library
#include <string>
#include <vector>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

// Forward declaration
class JsonObject;

/*!
  */
class JsonValue
{
 public:
  enum class ValueType : uint
  {
    kArray = 0,
    kBoolean,
    kNull,
    kNumber,
    kObject,
    kString,
  };


  //!
  JsonValue() noexcept;

  //!
  JsonValue(JsonValue&& other) noexcept;

  //! Delete value
  ~JsonValue() noexcept;

  //!
  JsonValue& operator=(JsonValue&& other) noexcept;
  

  //! Treat a value as a array 
  std::vector<JsonValue>& asArray() noexcept;

  //! Treat a value as a array 
  const std::vector<JsonValue>& asArray() const noexcept;

  //! Treat a value as a boolean
  bool& asBoolean() noexcept;

  //! Treat a value as a boolean
  const bool& asBoolean() const noexcept;

  //! Treat a value as a number
  double& asNumber() noexcept;

  //! Treat a value as a number 
  const double& asNumber() const noexcept;

  //! Treat a value as a 64bit integer
  JsonObject& asObject() noexcept;

  //! Treat a value as a 64bit integer
  const JsonObject& asObject() const noexcept;

  //! Treat a value as a string
  std::string& asString() noexcept;

  //! Treat a value as a string
  const std::string& asString() const noexcept;


  //! Check if a value is a array value
  bool isArray() const noexcept;

  //! Check if a value is a boolean value
  bool isBoolean() const noexcept;

  //! Check if a value is a number value
  bool isNumber() const noexcept;

  //! Check if a value is a null value
  bool isNull() const noexcept;

  //! Check if a value is a object value
  bool isObject() const noexcept;

  //! Check if a value is a string value
  bool isString() const noexcept;

  //! Set value as a array
  void setAsArray(std::vector<JsonValue>&& value) noexcept;

  //! Set value as a boolean
  void setAsBoolean(const bool value) noexcept;

  //! Set value as a number
  void setAsNumber(const double value) noexcept;

  //! Set value as a null
  void setAsNull() noexcept;

  //! Set value as a object
  void setAsObject(JsonObject&& value) noexcept;

  //! Set value as a string
  void setAsString(const char* value) noexcept;

  //! Set value as a string
  void setAsString(const std::string& value) noexcept;

  //! Set value as a string
  void setAsString(std::string&& value) noexcept;

  //! Swap the contents
  void swap(JsonValue& other) noexcept;

  //! Return a json type
  ValueType type() const noexcept; 

 private:
  // Delete default copy methods
  JsonValue(const JsonValue& other) = delete;
  JsonValue& operator=(const JsonValue& other) = delete;


  //! Delete previous value and set value as NULL
  void deleteValue() noexcept;


  void* value_;
  ValueType type_;
};

} // namespace zisc

#include "json_value-inl.hpp"

#endif // ZISC_JSON_VALUE_HPP
