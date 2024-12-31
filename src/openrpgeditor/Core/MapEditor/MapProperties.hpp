#pragma once
#include "Core/CommonUI/IDialogController.hpp"

class Map;
class MapInfo;
class MapProperties final : public IDialogController {
public:
  MapProperties() : IDialogController("Map Properties") {}

  void setMapInfo(MapInfo* mapInfo) { m_mapInfo = mapInfo; }
  std::tuple<bool, bool> draw() override;

  [[nodiscard]] Map* map() const;

private:
  void resizeMap(int width, int height) const;
  MapInfo* m_mapInfo{nullptr};
  int m_tempWidth{0};
  int m_tempHeight{0};
};
