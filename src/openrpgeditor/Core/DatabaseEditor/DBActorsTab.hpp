#pragma once
#include "Core/CommonUI/FacePicker.hpp"
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CharacterSheet.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/FaceSheet.hpp"
#include "Core/Graphics/SideViewBattlerSheet.hpp"
#include "DBCommonEventsTab.hpp"

#include "Database/Actors.hpp"

#include <optional>

struct DBActorsTab : IDBEditorTab {
  DBActorsTab() = delete;
  explicit DBActorsTab(Actors& actors, DatabaseEditor* parent);
  void draw() override;

  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(int start, int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }
  std::string getName(int index) override { return m_actors.actor(index)->name(); }
  int getCount() override { return m_actors.count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Actors"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBActorsTab"sv; };

private:
  int m_categoryStart;
  int m_categoryEnd;

  std::vector<int> m_headers;
  Actors& m_actors;
  Actor* m_selectedActor{};
  int m_editMaxActors{};
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  bool m_showEquipEdit = false;
  int m_selectedEquip = 0;
  int m_chosenDataId = 0;
  int m_chosenEquipId = 0;
  std::optional<FaceSheet> m_faceSheet;
  std::optional<CharacterSheet> m_characterSheet;
  std::optional<SideViewBattlerSheet> m_battlerSheet;
  std::optional<CheckeredCompositeTexture> m_faceButton;
  std::optional<CheckeredCompositeTexture> m_actorButton;
  std::optional<CharacterPicker> m_actorPicker;
  std::optional<CheckeredCompositeTexture> m_battlerButton;
  std::optional<ImagePicker> m_battlerPicker;
  std::optional<FacePicker> m_facePicker;
  TraitsEditor m_traitsEditor;
};
