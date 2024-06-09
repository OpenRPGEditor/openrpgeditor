#pragma once

#include "nlohmann/json.hpp"

#include <string>
struct MapInfo {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MapInfo, expanded, id, name, order, parentId, scrollX, scrollY);
  bool expanded;
  int id;
  std::string name;
  int order;
  int parentId;
  double scrollX;
  double scrollY;

  std::vector<MapInfo*>& children() { return m_children; }
  const std::vector<MapInfo*>& children() const { return m_children; }
private:

  friend void recursiveSort(MapInfo& in);
  friend class MapInfos;
  std::vector<MapInfo*> m_children;
};

class MapInfos {
public:
  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] MapInfo* map(int id) {
    for (auto& item : m_mapinfos) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const MapInfo* map(int id) const {
    for (const auto& set : m_mapinfos) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }

  bool empty() const {
    // If the vector is actually empty or we only have the dummy root node we have no maps
    // This allows for cases when a MapInfos.json only has it's root node
    // That being said the editor should *not* allow the user to delete their last map
    return m_mapinfos.empty() || m_mapinfos.size() == 1;
  }

  MapInfo& root() { return m_mapinfos[0]; }
  const MapInfo& root() const { return m_mapinfos[0]; }

  void buildTree(bool reset = false);
private:
  std::vector<MapInfo> m_mapinfos;

};
