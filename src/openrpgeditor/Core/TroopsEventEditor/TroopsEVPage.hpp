#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/Graphics/CharacterSheet.hpp"

struct TroopsEVEditor;
struct TroopsEVPage {
  TroopsEVPage(TroopsEVEditor* parent, Troop::Page* page);

  /* Returns true when closed, closing is the equivelant of deleting */
  std::tuple<bool, bool> draw(bool canDelete, int index);

  void clearPage();

  void setPage(Troop::Page* page, int pageIndex) {
    m_page = page;
    m_commandEditor.setCommands(&m_page->list);
  }
  void setParent(TroopsEVEditor* parent) { m_parent = parent; }

private:
  TroopsEVEditor* m_parent;
  Troop::Page* m_page;
  char m_pageNameBuf[4096];
  EventCommandEditor m_commandEditor;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<VariableSwitchPicker> picker;
  int m_page_id;
};
