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

class DBActorsTab final : public IDBCoreEditorTab<DBActorsTab> {
public:
  void draw() override;

  std::string getName(const int index) const override { return m_actors->actor(index)->name(); }
  int getCount() const override { return m_actors->count(); }

  [[nodiscard]] std::string tabName() const override { return tr("Actors"); }

  bool isReady() const override { return Database::instance()->actors && Database::instance()->system && Database::instance()->classes; }
  void initialize() override {
    if (!isReady()) {
      return;
    }
    m_actors = &Database::instance()->actors.value();

    m_selectedActor = m_actors->actor(1);
    if (m_selectedActor) {
      m_traitsEditor.setTraits(&m_selectedActor->traits());
    }
  }

  bool isInitialized() const override { return m_actors; }

private:
  Actors* m_actors = nullptr;
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
