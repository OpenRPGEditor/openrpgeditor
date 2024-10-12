#include "Items.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
Items Items::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Items items;
  items.m_items.reserve(data.size());

  int i = 0;
  for (const auto& [_, value] : data.items()) {
    Item& item = items.m_items.emplace_back();
    item.m_isValid = value != nullptr;
    if (item.m_isValid) {
      value.get_to(item);
    } else {
      item.id = i;
    }
    ++i;
  }
  return items;
}

void Items::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const Item& item : m_items) {
    if (item.m_isValid) {
      data.push_back(item);
    } else {
      data.push_back(nullptr);
    }
  }

  file << data.dump(4);
}
