#include "./string.hpp"
#include <sstream>
#include <cctype>

namespace aur::str {

std::vector<std::string> split(const std::string& value, char separator) {
  std::stringstream sstr(value);
  std::vector<std::string> segments;
  for (std::string seg; std::getline(sstr, seg, separator);) segments.push_back(seg);
  return segments;
}

std::string trimFront(const std::string& value) {
  std::string trimmed;
  for (auto c : value) {
    if (trimmed.size() == 0 && isspace(c)) continue;
    trimmed.push_back(c);
  }
  return trimmed;
}

std::string trimBack(const std::string& value) {
  std::string trimmed = value;
  while (trimmed.size() > 0 && isspace(trimmed.back())) trimmed.pop_back();
  return trimmed;
}

std::string trim(const std::string value) {
  return trimBack(trimFront(value));
}

}
