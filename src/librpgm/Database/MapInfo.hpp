#pragma once

#include "Database/Map.hpp"
#include "nlohmann/json.hpp"
#include <string>

class ISerializable;
class MapInfo final : public IModifiable {
  friend class MapInfosSerializer;
  friend void to_json(nlohmann::ordered_json& json, const MapInfo& mapinfo);
  friend void from_json(const nlohmann::ordered_json& json, MapInfo& mapinfo);
  friend class MapInfos; // TODO: Remove this

public:
  MapInfo() = default;
  MapInfo(const MapInfo& other)
  : IModifiable(other)
  , m_expanded(other.m_expanded)
  , m_id(other.m_id)
  , m_name(other.m_name)
  , m_order(other.m_order)
  , m_parentId(other.m_parentId)
  , m_scrollX(other.m_scrollX)
  , m_scrollY(other.m_scrollY)
  , m_children(other.m_children) {
    if (other.m_map) {
      m_map = std::make_unique<Map>(other.m_map->clone());
    }
  };
  MapInfo& operator=(const MapInfo& other) {
    IModifiable::operator=(other);
    m_expanded = other.m_expanded;
    m_id = other.m_id;
    m_name = other.m_name;
    m_order = other.m_order;
    m_parentId = other.m_parentId;
    m_scrollX = other.m_scrollX;
    m_scrollY = other.m_scrollY;
    if (other.m_map) {
      m_map = std::make_unique<Map>(other.m_map->clone());
    }
    return *this;
  }
  MapInfo(MapInfo&& other) noexcept
  : IModifiable(other)
  , m_expanded(other.m_expanded)
  , m_id(other.m_id)
  , m_name(std::move(other.m_name))
  , m_order(other.m_order)
  , m_parentId(other.m_parentId)
  , m_scrollX(other.m_scrollX)
  , m_scrollY(other.m_scrollY)
  , m_children(std::move(other.m_children)) {
    if (other.m_map) {
      m_map = std::make_unique<Map>(other.m_map->clone());
    }
  };
  MapInfo& operator=(MapInfo&& other) noexcept {
    IModifiable::operator=(std::move(other));
    m_expanded = other.m_expanded;
    m_id = other.m_id;
    m_name = std::move(other.m_name);
    m_order = other.m_order;
    m_parentId = other.m_parentId;
    m_scrollX = other.m_scrollX;
    m_scrollY = other.m_scrollY;
    if (other.m_map) {
      m_map = std::make_unique<Map>(other.m_map->clone());
    }
    return *this;
  }

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

  nlohmann::ordered_json serializeOldValues() const override { return clone(); }

  std::vector<MapInfo*>& children() { return m_children; }
  [[nodiscard]] const std::vector<MapInfo*>& children() const { return m_children; }

  Map* map();
  [[nodiscard]] const Map* map() const;

  Event* event(const int id) { return m_map ? m_map->event(id) : nullptr; }
  [[nodiscard]] const Event* event(const int id) const { return m_map ? m_map->event(id) : nullptr; }

  MapInfo clone() const { return MapInfo(*this, 1); }

  bool isModified() const override { return IModifiable::isModified() | (m_map ? m_map->isModified() : 0); }

private:
  MapInfo(const MapInfo& other, int)
  : IModifiable(other)
  , m_expanded(other.m_expanded)
  , m_id(other.m_id)
  , m_name(other.m_name)
  , m_order(other.m_order)
  , m_parentId(other.m_parentId)
  , m_scrollX(other.m_scrollX)
  , m_scrollY(other.m_scrollY) {};
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