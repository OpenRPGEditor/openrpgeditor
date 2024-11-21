#include "Database/Items.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

void to_json(nlohmann::ordered_json& json, const Item& item) {
  json = {
      {"id", item.id},
      {"animationId", item.animationId},
      {"consumable", item.consumable},
      {"damage", item.damage},
      {"description", item.description},
      {"effects", item.effects},
      {"hitType", item.hitType},
      {"iconIndex", item.iconIndex},
      {"itypeId", item.itypeId},
      {"name", item.name},
      {"note", item.note},
      {"occasion", item.occasion},
      {"price", item.price},
      {"repeats", item.repeats},
      {"scope", item.scope},
      {"speed", item.speed},
      {"successRate", item.successRate},
      {"tpGain", item.tpGain},
  };
}
void from_json(const nlohmann::ordered_json& json, Item& item) {
  item.id = json.value("id", item.id);
  item.animationId = json.value("animationId", item.animationId);
  item.consumable = json.value("consumable", item.consumable);
  item.damage = json.value("damage", item.damage);
  item.description = json.value("description", item.description);
  item.effects = json.value("effects", item.effects);
  item.hitType = json.value("hitType", item.hitType);
  item.iconIndex = json.value("iconIndex", item.iconIndex);
  item.itypeId = json.value("itypeId", item.itypeId);
  item.name = json.value("name", item.name);
  item.note = json.value("note", item.note);
  item.occasion = json.value("occasion", item.occasion);
  item.price = json.value("price", item.price);
  item.repeats = json.value("repeats", item.repeats);
  item.scope = json.value("scope", item.scope);
  item.speed = json.value("speed", item.speed);
  item.successRate = json.value("successRate", item.successRate);
  item.tpGain = json.value("tpGain", item.tpGain);
}

using json = nlohmann::ordered_json;
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
