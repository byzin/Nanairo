/*!
  \file json_object.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_JSON_OBJECT_HPP
#define ZISC_JSON_OBJECT_HPP

// Standard C++ library
#include <map>
#include <string>
// Zisc
#include "json_value.hpp"

namespace zisc {

/*!
  */
class JsonObject
{
 public:
  using JsonMap = std::map<std::string, JsonValue>;
  // STL like type definitions
  using value_type = JsonMap::value_type;
  using iterator = JsonMap::iterator;
  using const_iterator = JsonMap::const_iterator;


  //!
  JsonObject() noexcept;

  //!
  JsonObject(JsonObject&& other) noexcept;

  //!
  JsonObject& operator=(JsonObject&& other) noexcept;


  //! Return an iterator to the beginning
  iterator begin() noexcept;

  //! Return an iterator to the beginning
  const_iterator begin() const noexcept;

  //! Return an iterator to the beginning
  const_iterator cbegin() const noexcept;

  //! Return an iterator to the end 
  iterator end() noexcept;

  //! Return an iterator to the end 
  const_iterator end() const noexcept;

  //! Return an iterator to the end 
  const_iterator cend() const noexcept;


  //! Append a key value pair
  void append(const std::string& key, JsonValue&& value) noexcept;

  //! Append a key value pair
  void append(std::string&& key, JsonValue&& value) noexcept;

  //! Check if a object has a key
  bool hasKey(const std::string& key) const noexcept;

  //! Return the number of elements
  uint size() const noexcept;

  //! Return a value by a key
  JsonValue& value(const std::string& key) noexcept;

  //! Return a value by a key
  const JsonValue& value(const std::string& key) const noexcept;

 private:
  // Delete copy methods
  JsonObject(const JsonObject& other) = delete;
  JsonObject& operator=(const JsonObject& other) = delete;


  JsonMap members_;
};

} // namespace zisc

#include "json_object-inl.hpp"

#endif // ZISC_JSON_OBJECT_HPP
