/*!
  \file json_object-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_OBJECT_INL_HPP
#define ZISC_JSON_OBJECT_INL_HPP

#include "json_object.hpp"
// Standard C++ library
#include <map>
#include <string>
#include <utility>
// Zisc
#include "error.hpp"
#include "json_value.hpp"
#include "utility.hpp"

namespace zisc {

/*!
  */
inline
JsonObject::JsonObject() noexcept
{
}

/*!
  */
inline
JsonObject::JsonObject(JsonObject&& other) noexcept :
    members_{std::move(other.members_)}
{
}

/*!
  */
inline
JsonObject& JsonObject::operator=(JsonObject&& other) noexcept
{
  members_ = std::move(other.members_);
  return *this;
}

/*!
  */
inline
auto JsonObject::begin() noexcept -> iterator
{
  return members_.begin();
}

/*!
  */
inline
auto JsonObject::begin() const noexcept -> const_iterator
{
  return members_.begin();
}

/*!
  */
inline
auto JsonObject::cbegin() const noexcept -> const_iterator
{
  return members_.cbegin();
}

/*!
  */
inline
auto JsonObject::end() noexcept -> iterator
{
  return members_.end();
}

/*!
  */
inline
auto JsonObject::end() const noexcept -> const_iterator
{
  return members_.end();
}

/*!
  */
inline
auto JsonObject::cend() const noexcept -> const_iterator
{
  return members_.cend();
}

/*!
  */
inline
void JsonObject::append(const std::string& key, JsonValue&& value) noexcept
{
  append(std::string{key}, std::move(value));
}

/*!
  */
inline
void JsonObject::append(std::string&& key, JsonValue&& value) noexcept
{
  ZISC_ASSERT(!hasKey(key), "The key already exists.");
  members_.emplace(std::move(key), std::move(value));
}

/*!
  */
inline
bool JsonObject::hasKey(const std::string& key) const noexcept
{
  auto position = members_.find(key);
  return (position != members_.end());
}

/*!
  */
inline
uint JsonObject::size() const noexcept
{
  return cast<uint>(members_.size());
}

/*!
  */
inline
JsonValue& JsonObject::value(const std::string& key) noexcept
{
  ZISC_ASSERT(hasKey(key), "The key doesn't exist.");
  return members_.at(key);
}

/*!
  */
inline
const JsonValue& JsonObject::value(const std::string& key) const noexcept
{
  ZISC_ASSERT(hasKey(key), "The key doesn't exist.");
  return members_.at(key);
}

/*!
  */
inline
void JsonValue::setAsObject(JsonObject&& value) noexcept
{
  deleteValue();
  auto v = new JsonObject{std::move(value)};
  value_ = treatAs<void*>(v);
  type_ = ValueType::kObject;
}

} // namespace zisc

#endif // ZISC_JSON_OBJECT_INL_HPP
