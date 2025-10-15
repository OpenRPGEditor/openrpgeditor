#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "DBCommonEventsTab.hpp"
#include "imgui.h"

#include <string>
#include <vector>

struct System;
class DBTypesTab final : public IDBEditorTab {
public:
  explicit DBTypesTab(DatabaseEditor* parent)
  : IDBEditorTab(parent) {}
  void draw() override;
  std::vector<int>& getHeaders() override { return m_headers; }
  int getHeader(const int index) override { return m_headers.at(index); }
  bool hasHeader() override { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) override {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  std::string getName(const int index) override { return ""; }
  int getCount() override { return 0; }

  [[nodiscard]] std::string tabName() const override { return tr("Types"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBTypesTab"sv; };

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
