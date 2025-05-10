#pragma once
#include "Core/CommonUI/IDialogController.hpp"
#include "OREMath/Point.hpp"

#include <map>

class MapInfo;
class EventTilePicker final : public IDialogController {
public:
  explicit EventTilePicker(int mapId, bool showTree = true);

  std::tuple<bool, bool> draw() override;

  Point selectedTile() const { return m_selectedTile; }
  int selectedMap() const { return m_currentMap; }

private:
  void drawMapTreeRecursive(MapInfo& info);

  int m_currentMap;
  Point m_selectedTile;
  bool m_showTree{true};

  std::map<int, bool> m_expandedState; // mapId -> expanded
};
