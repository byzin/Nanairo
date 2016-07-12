/*!
  \file json_value-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_VALUE_INL_HPP
#define ZISC_JSON_VALUE_INL_HPP

#include "json_value.hpp"
// Standard C++ library
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
// Zisc
#include "error.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  */
inline
JsonValue::JsonValue() noexcept : 
    value_{nullptr},
    type_{ValueType::kNull}
{
}

/*!
  */
inline
JsonValue::JsonValue(JsonValue&& other) noexcept :
    value_{nullptr},
    type_{ValueType::kNull}
{
  swap(other);
}

/*!
  */
inline
JsonValue::~JsonValue() noexcept
{
  deleteValue();
}

/*!
  */
inline
JsonValue& JsonValue::operator=(JsonValue&& other) noexcept
{
  swap(other);
  return *this;
}

/*!
  */
inline
std::vector<JsonValue>& JsonValue::asArray() noexcept
{
  ZISC_ASSERT(isArray(), "The value isn't array.");
  return *treatAs<std::vector<JsonValue>*>(value_);
}

/*!
  */
inline
const std::vector<JsonValue>& JsonValue::asArray() const noexcept
{
  ZISC_ASSERT(isArray(), "The value isn't array.");
  return *treatAs<const std::vector<JsonValue>*>(value_);
}

/*!
  */
inline
bool& JsonValue::asBoolean() noexcept
{
  ZISC_ASSERT(isBoolean(), "The value isn't boolean.");
  return *treatAs<bool*>(value_);
}

/*!
  */
inline
const bool& JsonValue::asBoolean() const noexcept
{
  ZISC_ASSERT(isBoolean(), "The value isn't boolean.");
  return *treatAs<const bool*>(value_);
}

/*!
  */
inline
double& JsonValue::asNumber() noexcept
{
  ZISC_ASSERT(isNumber(), "The value isn't a number.");
  return *treatAs<double*>(value_);
}

/*!
  */
inline
const double& JsonValue::asNumber() const noexcept
{
  ZISC_ASSERT(isNumber(), "The value isn't a number.");
  return *treatAs<const double*>(value_);
}

/*!
  */
inline
JsonObject& JsonValue::asObject() noexcept
{
  ZISC_ASSERT(isObject(), "The value isn't object.");
  return *treatAs<JsonObject*>(value_);
}

/*!
  */
inline
const JsonObject& JsonValue::asObject() const noexcept
{
  ZISC_ASSERT(isObject(), "The value isn't object.");
  return *treatAs<const JsonObject*>(value_);
}

/*!
  */
inline
std::string& JsonValue::asString() noexcept
{
  ZISC_ASSERT(isString(), "The value isn't string.");
  return *treatAs<std::string*>(value_);
}

/*!
  */
inline
const std::string& JsonValue::asString() const noexcept
{
  ZISC_ASSERT(isString(), "The value isn't string.");
  return *treatAs<const std::string*>(value_);
}

/*!
  */
inline
bool JsonValue::isArray() const noexcept
{
  return (type_ == ValueType::kArray);
}

/*!
  */
inline
bool JsonValue::isBoolean() const noexcept
{
  return (type_ == ValueType::kBoolean);
}

/*!
  */
inline
bool JsonValue::isNumber() const noexcept
{
  return (type_ == ValueType::kNumber);
}

/*!
  */
inline
bool JsonValue::isNull() const noexcept
{
  return (type_ == ValueType::kNull);
}

/*!
  */
inline
bool JsonValue::isObject() const noexcept
{
  return (type_ == ValueType::kObject);
}

/*!
  */
inline
bool JsonValue::isString() const noexcept
{
  return (type_ == ValueType::kString);
}

/*!
  */
inline
void JsonValue::setAsArray(std::vector<JsonValue>&& value) noexcept
{
  deleteValue();
  auto v = new std::vector<JsonValue>{std::move(value)};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kArray;
}

/*!
  */
inline
void JsonValue::setAsBoolean(const bool value) noexcept
{
  deleteValue();
  auto v = new bool{value};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kBoolean;
}

/*!
  */
inline
void JsonValue::setAsNumber(const double value) noexcept
{
  deleteValue();
  auto v = new double{value};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kNumber;
}

/*!
  */
inline
void JsonValue::setAsNull() noexcept
{
  deleteValue();
}

/*!
  */
inline
void JsonValue::setAsString(const char* value) noexcept
{
  deleteValue();
  auto v = new std::string{value};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kString;
}

/*!
  */
inline
void JsonValue::setAsString(const std::string& value) noexcept
{
  setAsString(std::string{value});
}

/*!
  */
inline
void JsonValue::setAsString(std::string&& value) noexcept
{
  deleteValue();
  auto v = new std::string{std::move(value)};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kString;
}

/*!
  */
inline
void JsonValue::swap(JsonValue& other) noexcept
{
  auto tmp_value = other.value_;
  auto tmp_type = other.type_;
  other.value_ = value_;
  other.type_ = type_;
  value_ = tmp_value;
  type_ = tmp_type;
}

/*!
  */
inline
auto JsonValue::type() const noexcept -> ValueType
{
  return type_;
}

/*!
  */
inline
void JsonValue::deleteValue() noexcept
{
  auto delete_value = [](auto value)
  {
    ZISC_ASSERT(value != nullptr, "The value is NULL.");
    using Type = std::remove_pointer_t<decltype(value)>;
    std::unique_ptr<Type> v{value};
  };

  switch (type_) {
   case ValueType::kArray:
    delete_value(treatAs<std::vector<JsonValue>*>(value_));
    break;
   case ValueType::kBoolean:
    delete_value(treatAs<bool*>(value_));
    break;
   case ValueType::kNumber:
    delete_value(treatAs<double*>(value_));
    break;
   case ValueType::kNull:
    break;
   case ValueType::kObject:
    delete_value(treatAs<JsonObject*>(value_));
    break;
   case ValueType::kString:
    delete_value(treatAs<std::string*>(value_));
    break;
   default:
    ZISC_ASSERT(false, "The type is invalid.");
    break;
  }
  value_ = nullptr;
  type_ = ValueType::kNull;
}

} // namespace zisc

#endif // ZISC_JSON_VALUE_INL_HPP
