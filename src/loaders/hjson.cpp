#include "./hjson.hpp"
#include <fstream>
#include "util/string.hpp"
#include <regex>

namespace aur::hjson {

template <>
std::string Value::get<string>() const {
  return std::any_cast<std::string>(data);
}

template <>
float Value::get<number>() const {
  return std::any_cast<float>(data);
}

template <>
std::vector<Value*> Value::get<array>() const {
  return std::any_cast<std::vector<Value*>>(data);
}

template <>
std::map<std::string, Value*> Value::get<object>() const {
  return std::any_cast<std::map<std::string, Value*>>(data);
}

Value::~Value() {
  if (type == array) for (auto child : get<array>()) delete child;
  if (type == object) for (auto [_, child] : get<object>()) delete child;
}

std::string Value::typeName() const {
  switch (type) {
    case string:  return "string";
    case number:  return "number";
    case object:  return "object";
    case array:   return "array";
    default:      return "unknown";
  }
}

bool Value::isContainer() const {
  return type == object || type == array;
}

static std::regex numReg { "^-?[0-9]+(\\.[0-9]+)?(e[0-9]+)?$" };

bool isNum(const std::string& value) {
  return std::regex_match(value, numReg);
}

void insert(Value* container, const std::string& key, Value* value) {
  assert(container && container->isContainer());
  if (container->type == array) {
    auto arr = container->get<array>();
    arr.push_back(value);
    container->data = arr;
  } else {
    auto obj = container->get<object>();
    obj.insert({ key, value });
    container->data = obj;
  }
}

void commitValue(std::string token, Value* node, const std::string& key) {
  token = str::trim(token);
  if (!token.size()) return;
  auto value = new Value();
  if (isNum(token)) {
    value->type = number;
    value->data = std::stof(token);
  } else {
    value->type = string;
    if (token.size() > 1 && token[0] == '"' && token[token.size() - 1] == '"') {
      token.erase(0, 1);
      token.pop_back();
    }
    value->data = token;
  }
  insert(node, key, value);
}

std::unique_ptr<Value> load(const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) 
    throw std::runtime_error(std::string("couldn't open hjson file: ") + path);
  
  Value* node = nullptr;
  std::string token;
  std::string key;
  bool isStr = false;
  bool isCmt = false;
  
  for (char ch; file.get(ch);) {
    if (isCmt) {
      if (ch == '\n') isCmt = false;
      else continue;
    }
    
    if (ch == '{' || ch == '[') {
      auto container = ch == '{'
        ? new Value{ object, std::map<std::string, Value*>{} }
        : new Value{ array, std::vector<Value*>{} };

      if (node) {
        insert(node, key, container);
        container->parent = node;
      }

      node = container;
    }
    else if (ch == '}' || ch == ']') {
      commitValue(token, node, key);
      if (!node->parent) return std::unique_ptr<Value>(node);
      node = node->parent;
      token = "";
    }
    else if (ch == ':' && !isStr) {
      key = str::trim(token);
      token = "";
    }
    else if ((ch == '\n' || ch == ',') && !isStr) {
      commitValue(token, node, key);
      token = "";
    }
    else if (ch == '#') {
      isCmt = true;
    }
    else {
      token += ch;
    }
  }
  assert(false);
}

}
