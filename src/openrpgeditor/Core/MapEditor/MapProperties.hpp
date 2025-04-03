#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "Database/Map.hpp"

class Map;
class MapInfo;
class MapProperties final : public IDialogController {
public:
  MapProperties() : IDialogController("Map Properties") {}

  void setMapInfo(MapInfo* mapInfo) {
    if (m_mapInfo == mapInfo) {
      return;
    }

    m_mapInfo = mapInfo;
    if (m_mapInfo && map()) {
      m_tempWidth = map()->width();
      m_tempHeight = map()->height();
    }
  }
  std::tuple<bool, bool> draw() override;

  [[nodiscard]] Map* map() const;

private:
  void resizeMap(int width, int height) const;
  MapInfo* m_mapInfo{nullptr};
  int m_tempWidth{0};
  int m_tempHeight{0};
};
