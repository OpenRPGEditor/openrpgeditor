#pragma once

#include "nlohmann/json.hpp"

#include <string>

class MapInfos {
public:
  struct MapInfo {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MapInfo, expanded, id, name, order, parentId, scrollX, scrollY);
    bool expanded;
    int id;
    std::string name;
    int order;
    int parentId;
    double scrollX;
    double scrollY;

    std::vector<MapInfo*> m_children;
  };

  static MapInfos load(std::string_view filepath);
  bool serialize(std::string_view filename);
  std::vector<MapInfo> m_mapinfos;

  void buildTree();
};
