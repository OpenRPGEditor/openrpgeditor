#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "OREMath/Point.hpp"

#include <map>

class MapInfo;
class EventTilePicker final : public IDialogController {
public:
  explicit EventTilePicker(int mapId);

  std::tuple<bool, bool> draw() override;

  Point selectedTile() const { return m_selectedTile; }
  int selectedMap() const { return m_currentMap; }

private:
  void drawMapTreeRecursive(MapInfo& info);
  void drawMapTree();

  int m_currentMap;
  Point m_selectedTile;

  std::map<int, bool> m_expandedState; // mapId -> expanded
};
