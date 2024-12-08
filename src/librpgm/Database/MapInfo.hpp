#pragma once

#include "Database/Map.hpp"
#include "nlohmann/json.hpp"
#include <string>

class ISerializable;
class MapInfo {
  friend class MapInfosSerializer;
  friend void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo);
  friend void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo);
  friend class MapInfos; // TODO: Remove this

public:
  [[nodiscard]] int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  [[nodiscard]] bool expanded() const { return m_expanded; }
  void setExpanded(const bool expanded) { m_expanded = expanded; }

  [[nodiscard]] const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  [[nodiscard]] int order() const { return m_order; }
  void setOrder(const int order) { m_order = order; }

  [[nodiscard]] int parentId() const { return m_parentId; }
  void setParentId(const int parentId) { m_parentId = parentId; }

  [[nodiscard]] double scrollX() const { return m_scrollX; }
  void setScrollX(const double scrollX) { m_scrollX = scrollX; }

  [[nodiscard]] double scrollY() const { return m_scrollY; }
  void setScrollY(const double scrollY) { m_scrollY = scrollY; }

  std::vector<MapInfo*>& children() { return m_children; }
  [[nodiscard]] const std::vector<MapInfo*>& children() const { return m_children; }

  Map* map() { return m_map.get(); }
  [[nodiscard]] const Map* map() const { return m_map.get(); }

  Event* event(int id) { return m_map ? m_map->event(id) : nullptr; }
  [[nodiscard]] const Event* event(int id) const { return m_map ? m_map->event(id) : nullptr; }

private:
  friend void recursiveSort(MapInfo& in);
  friend class MapInfos;
  friend class MapInfosSerializer;

  bool m_expanded{};
  int m_id{};
  std::string m_name;
  int m_order{};
  int m_parentId{};
  double m_scrollX{};
  double m_scrollY{};

  std::vector<MapInfo*> m_children;
  std::unique_ptr<Map> m_map;
};
void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo);
void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo);