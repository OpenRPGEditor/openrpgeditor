#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"

#include "Database/EventPage.hpp"

struct EventEditor;
struct Actor;
struct Item;
class EVPage final : IPageEditor {
public:
  /* Returns true when closed, closing is the equivalent of deleting */
  std::tuple<bool, bool> draw(bool canDelete, int index) override;

  void clearPage() const;

  void setPage(EventPage* page) {
    m_page = page;
    m_commandEditor.setCommands(&m_page->list());
  }

  void setParent(EventEditor* parent) { m_parent = parent; }

protected:
  friend IPageEditor* IPageEditor::create(EventPage* page);
  explicit EVPage(EventPage* page);
  void pagePointerInvalidated() override;

private:
  enum VariableSwitchSelection {
    Variable,
    Switch1,
    Switch2,
  };
  EventEditor* m_parent;
  char m_pageNameBuf[4096]{};
  EventCommandEditor m_commandEditor;
  CharacterSheet m_characterSheet;
  VariableSwitchSelection m_variableSwitchSelection{Variable};
  std::optional<VariableSwitchPicker> m_variableSwitchPicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::PatternAndDirection};
  std::optional<CheckeredCompositeTexture> m_actorButton;
};
