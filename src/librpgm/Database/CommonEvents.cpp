
#include "CommonEvents.hpp"

#include <fstream>
#include <iostream>

#include <regex>
#include <string>
#include <vector>

#define MAGIC_ENUM_RANGE_MIN -4096
#define MAGIC_ENUM_RANGE_MAX 4096
#include <magic_enum.hpp>

std::string ExpandEnumName(std::string_view str) {
  std::regex reg("([A-Z]?[a-z]+|[0-9]+|[A-Z]+(?![a-z]))");
  std::smatch match;
  std::string result = "";
  std::string search = str.data();

  while (std::regex_search(search, match, reg)) {
    if (!result.empty() && !match[0].str().empty()) {
      result += " " + match[0].str();
    } else if (!match[0].str().empty()) {
      result += match[0].str();
    }
    search = match.suffix().str();
  }

  return result;
}

using json = nlohmann::json;

CommonEvents::CommonEvents() {
}
