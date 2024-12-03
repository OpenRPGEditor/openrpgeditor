#pragma once
#include "Core/CommonUI/TraitsEditor.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/Graphics/CheckeredCompositeTexture.hpp"
#include "Core/Graphics/IconSheet.hpp"
#include "Database/Armors.hpp"

struct DBArmorsTab : IDBEditorTab {
  DBArmorsTab() = delete;
  explicit DBArmorsTab(Armors& armors, DatabaseEditor* parent);
  void draw() override;

  [[nodiscard]] std::vector<Armor>& armors() { return m_armors.armors(); }
  [[nodiscard]] const std::vector<Armor>& armors() const { return m_armors.armors(); }
  Armor* armor(int id) { return m_armors.armor(id); }
  const Armor* armor(int id) const { return m_armors.armor(id); }

private:
  Armors& m_armors;
  Armor* m_selectedArmor{};
  int m_editMaxArmors{};
  float m_splitterWidth = 300.f;
  bool m_changeIntDialogOpen = false;
  bool m_changeConfirmDialogOpen = false;
  IconSheet* m_currentSheet = nullptr;
  std::optional<CheckeredCompositeTexture> m_itemButtonTexture;
  std::optional<IconSheet> m_itemSheet;
  std::optional<IconSheet> m_itemPicker;
  TraitsEditor m_traitsEditor;
};