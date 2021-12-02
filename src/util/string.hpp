#pragma once

#include <string>
#include <vector>

namespace aur::str {

std::vector<std::string> split(const std::string& value, char separator = ' ');

std::string trim(const std::string value);
std::string trimFront(const std::string& value);
std::string trimBack(const std::string& value);

}
