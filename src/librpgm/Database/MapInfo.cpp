//
// Created by antidote on 12/7/24.
//

#include "Database/MapInfo.hpp"

void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo) {
  json = {
      {"id", mapinfo.m_id},           {"expanded", mapinfo.m_expanded}, {"name", mapinfo.m_name}, {"order", mapinfo.m_order}, {"parentId", mapinfo.m_parentId},
      {"scrollX", mapinfo.m_scrollX}, {"scrollY", mapinfo.m_scrollY},
  };
}
void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo) {
  mapinfo.m_id = json.value("id", mapinfo.m_id);
  mapinfo.m_expanded = json.value("expanded", mapinfo.m_expanded);
  mapinfo.m_name = json.value("name", mapinfo.m_name);
  mapinfo.m_order = json.value("order", mapinfo.m_order);
  mapinfo.m_parentId = json.value("parentId", mapinfo.m_parentId);
  mapinfo.m_scrollX = json.value("scrollX", mapinfo.m_scrollX);
  mapinfo.m_scrollY = json.value("scrollY", mapinfo.m_scrollY);
}