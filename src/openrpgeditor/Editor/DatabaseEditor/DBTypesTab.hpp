#pragma once
#include "DBCommonEventsTab.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"
#include "imgui.h"

#include <string>
#include <vector>

class System;
class DBTypesTab final : public IDBCoreEditorTab<DBTypesTab> {
public:
  void draw() override;

  [[nodiscard]] std::string tabName() const override { return tr("Types"); }

  bool isReady() const override { return !!Database::instance()->system; }

  bool isInitialized() const override { return m_system; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_system = &Database::instance()->system.value();
  }

private:
  int m_categoryStart{};
  int m_categoryEnd{};
  std::vector<int> m_headers;
  enum class EditType {
    Element,
    Skill,
    Weapon,
    Armor,
    Equipment,
  };

  void setInitialBuffer(const std::vector<std::string>& list, std::string& buffer, int selected);
  void drawTypeTable(std::string_view id, std::string_view listId, std::string_view label, const std::vector<std::string>& list, int& selected, EditType editType, std::string& buffer);

  System* m_system{};
  int m_selectedElement{1};
  int m_selectedSkill{1};
  int m_selectedWeapon{1};
  int m_selectedArmor{1};
  int m_selectedEquipment{1};
  std::string m_elementBuffer;
  std::string m_skillBuffer;
  std::string m_weaponBuffer;
  std::string m_armorBuffer;
  std::string m_equipmentBuffer;
  int m_editMax{0};
  EditType m_maxEditType{EditType::Element};
  bool m_confirmationRequested{false};
  bool m_confirmationClosed{false};
  bool m_changeMaxRequested{false};
};
