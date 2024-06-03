#include "Items.hpp"


#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
Items Items::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Items items;
  items.m_items.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Item& animation = items.m_items.emplace_back();
    value.get_to(animation);
  }
  return items;
}

void Items::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data{nullptr};

  for (const Item& animation : m_items) {
    data.push_back(animation);
  }

  file << data;
}

