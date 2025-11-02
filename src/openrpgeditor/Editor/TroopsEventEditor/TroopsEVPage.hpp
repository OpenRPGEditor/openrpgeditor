#pragma once
#include "Editor/CommonUI/CharacterPicker.hpp"
#include "Editor/CommonUI/EventCommandEditor.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/Graphics/CharacterSheet.hpp"

struct TroopsEVEditor;
struct TroopsEVPage {
  TroopsEVPage(TroopsEVEditor* parent, Troop::Page* page, int m_troopId);

  /* Returns true when closed, closing is the equivelant of deleting */
  std::tuple<bool, bool> draw(bool canDelete, int index);

  void clearPage();

  void setPage(Troop::Page* page, int pageIndex) {
    m_page = page;
    m_commandEditor.setCommands(&m_page->list());
  }
  void setParent(TroopsEVEditor* parent) { m_parent = parent; }

private:
  TroopsEVEditor* m_parent;
  Troop::Page* m_page;
  char m_pageNameBuf[4096];
  EventCommandEditor m_commandEditor;
  std::optional<ActorPicker> m_actorPicker;
  std::optional<VariableSwitchPicker> picker;
  int m_page_id;
};
