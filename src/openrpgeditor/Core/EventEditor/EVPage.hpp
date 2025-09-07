#pragma once
#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/CommonUI/EventCommandEditor.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventEditor.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"

#include "Database/EventPage.hpp"

struct EventEditor;
struct Actor;
struct Item;
struct ImGuiTabItem;

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
  ImGuiTabItem* tabItem() const { return m_tabItem; }
  int layoutIndex() const { return m_layoutIndex; }

protected:
  friend IPageEditor* IPageEditor::create(EventPage* page);

  explicit EVPage(EventPage* page);

  void pagePointerInvalidated() override;

private:
  friend class EventEditor;

  enum VariableSwitchSelection {
    Variable,
    Switch1,
    Switch2,
  };

  EventEditor* m_parent = nullptr;
  char m_pageNameBuf[4096]{};
  EventCommandEditor m_commandEditor;
  CharacterSheet m_characterSheet;
  std::optional<Dialog_SetMovementRoute> m_routeDialog;
  VariableSwitchSelection m_variableSwitchSelection{Variable};
  std::optional<VariableSwitchPicker> m_variableSwitchPicker;
  std::optional<ActorPicker> m_actorPicker;
  std::optional<ItemPicker> m_itemPicker;
  CharacterPicker m_characterPicker{CharacterPicker::PickerMode::PatternAndDirection, true};
  std::optional<CheckeredCompositeTexture> m_actorButton;
  ImGuiTabItem* m_tabItem = nullptr;
  int m_layoutIndex = 0;

  int m_uid;
  static int mNextID;
};
