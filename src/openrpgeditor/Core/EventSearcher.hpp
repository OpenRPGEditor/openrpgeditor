#pragma once

#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Animations.hpp"
#include "Database/Armors.hpp"
#include "Database/Audio.hpp"
#include "Database/Class.hpp"
#include "Database/CommonEvent.hpp"
#include "Database/Enemies.hpp"
#include "Database/Event.hpp"
#include "Database/Items.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"
#include "Database/Weapons.hpp"
#include "ReferenceSearch.hpp"
#include "imgui.h"

#include <vector>

struct MainWindow;
class EventSearcher {
public:
  explicit EventSearcher(MainWindow* parent)
  : m_parent(parent) {}

  bool getListing();

  void draw();
  void drawTable(std::string label, int mapId, int eventId, std::string eventName, int x, int y, int pageNo, int step);
  void drawTable(int commonId, int tableIndex, int step);
  void drawTable(std::string text, int tableIndex, int step, int eventId, int mapId, int pageId, int pos);
  void drawStringCommand(const SearchResult& result, SearchType type, int tableIndex, int step);

  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

private:
  ReferenceSearch reference;
  // std::vector<std::optional<Event>> m_events;
  std::optional<VariableSwitchPicker> picker;
  MainWindow* m_parent;
  int m_selectedVersion = -1;
  int m_pickedSearchType{0};

  int m_selectedSearchType{0};
  int m_selectedResult{0}; // For table selection
  int m_currentPage{0};
  int m_maxPage{1};
  int totalEntries{0};
  bool m_isOpen = false;
  const int TOTAL_ENTRIES = 29;

  // This is required so we don't update the data unless we ask to search
  int m_selectedData{1}; // Data set from picked data
  int m_pickedData{1};   // Data picked from selectors
  std::string m_searchString;
  std::string m_pickedString;

  bool m_valueSelected{false};

  SearchType type{SearchType::Variable};
  SearchType pickedType{SearchType::Variable};

  std::optional<ObjectPicker<Animation>> m_animationPicker;
  std::optional<ObjectPicker<Class>> m_classPicker;
  std::optional<ObjectPicker<State>> m_statePicker;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> m_commonPicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Weapon>> m_weaponPicker;
  std::optional<ObjectPicker<Armor>> m_armorPicker;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  std::optional<ObjectPicker<Enemy>> m_enemyPicker;
  std::optional<ObjectPicker<Skill>> m_skillPicker;
};
