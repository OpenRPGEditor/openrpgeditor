#include "Database/Plugins.hpp"
#include <fstream>

Plugins Plugins::load(std::string_view path) {
  std::ifstream file(path.data());
  Plugins ret;
  if (file.is_open()) {
    std::string line;
    std::string content;
    bool found = false;
    while (std::getline(file, line)) {
      if (line.compare("var $plugins =") == 0) {
        found = true;
        continue;
      }

      if (found) {
        content += "\n" + line;
      }
    }
    if (content.back() == ';') {
      content.back() = ' ';
    }
    nlohmann::json data = nlohmann::json::parse(content);
    for (auto [_, value] : data.items()) {
      auto& plugin = ret.plugins.emplace_back();
      value.get_to(plugin);
    }
  }

  return ret;
}

