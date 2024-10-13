#pragma once
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CharacterSheet.hpp"
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"

#include "Database/EventPage.hpp"

struct EventEditor;
struct EventPage;
struct Actor;
struct Item;
struct EVPage {
  EVPage(EventEditor* parent, EventPage* page);

  /* Returns true when closed, closing is the equivelant of deleting */
  std::tuple<bool, bool> draw(bool canDelete, int index);

  void clearPage();

  void setPage(EventPage* page) {
    m_page = page;
    m_commandEditor.setCommands(&m_page->list);
  }

  void setParent(EventEditor* parent) { m_parent = parent; }

private:
  enum VariableSwitchSelection {
    Variable,
    Switch1,
    Switch2,
  };
  EventEditor* m_parent;
  EventPage* m_page;
  char m_pageNameBuf[4096];
  EventCommandEditor m_commandEditor;
  CharacterSheet m_characterSheet;
  CheckerboardTexture m_buttonBack{80, 102, CellSizes::_64, 255, 200};
  VariableSwitchSelection m_variableSwitchSelection{Variable};
  std::optional<VariableSwitchPicker> m_variableSwitchPicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::PatternAndDirection};
};
