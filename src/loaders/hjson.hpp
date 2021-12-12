#pragma once

#include <string>
#include <vector>
#include <map>
#include <any>
#include <type_traits>
#include <memory>

namespace aur::hjson {

enum class ValueType {
  number,
  string,
  object,
  array,
};

inline constexpr auto number = ValueType::number;
inline constexpr auto string = ValueType::string;
inline constexpr auto object = ValueType::object;
inline constexpr auto array = ValueType::array;

template <ValueType T>
struct Type {};

struct Value;

template <>
struct Type<number> { typedef float type; };

template <>
struct Type<string> { typedef std::string type; };

template <>
struct Type<array> { typedef std::vector<Value*> type; };

template <>
struct Type<object> { typedef std::map<std::string, Value*> type; };

struct Value {
  ValueType type;
  std::any data;
  Value* parent = nullptr;

  template <ValueType T>
  typename Type<T>::type get() const;
  bool isContainer() const;
};

std::unique_ptr<Value> load(const std::string& path);

}
