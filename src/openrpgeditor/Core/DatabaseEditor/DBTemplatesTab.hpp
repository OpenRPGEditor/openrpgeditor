#pragma once
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Core/EventEditor.hpp"
#include "Core/TemplateEditor/IDBTemplates.hpp"
#include "Core/TemplateEditor/TemplatesCommand.hpp"
#include "Core/TemplateEditor/TemplatesEventProperties.hpp"
#include "Core/TemplateEditor/TemplatesTint.hpp"
#include "Database/Templates.hpp"
#include "DBCommonEventsTab.hpp"

#include <optional>

class Templates;
class DBTemplatesTab final : public IDBEditorTab {
public:
  void draw() override;
  std::vector<Templates*>& templates() { return m_template; }

  [[nodiscard]] std::string tabName() const override { return tr("Templates"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBTemplatesTab"sv; };
  bool isExperimental() const override { return true; }

  bool isReady() const override {
    if (m_templates) {
      return true;
    }
    if (!Database::instance()->templates) {
      return false;
    }

    const_cast<DBTemplatesTab*>(this)->m_templates = &Database::instance()->templates.value();
    return m_templates != nullptr;
  }

  bool isInitialized() const override { return m_templates; }
  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_templates = &Database::instance()->templates.value();

    if (!m_templates->templates.empty()) {
      if (m_templates->templates.at(m_selection).commands().empty()) {
        m_currentCommands.emplace_back(std::make_shared<EventDummy>());
        m_currentCommands.back()->setIndent(0);
      } else {
        if (m_templates->templates.at(m_selection).type() == Template::TemplateType::Command) {
          CommandParser parser;
          const nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(m_templates->templates.at(m_selection).commands());
          m_currentCommands = parser.parse(cmdJson);
          m_commandEditor.setCommands(&m_currentCommands);
        } else {
          m_currentCommands.clear();
        }
      }
      m_templateNote = m_templates->templates.at(m_selection).note();
      m_templateName = m_templates->templates.at(m_selection).name();
      m_templateType = static_cast<int>(m_templates->templates.at(m_selection).type());
      m_currentTemplate = CreateTemplateDialog((m_templates->templates.at(m_selection).type()));
    } else {
      m_currentTemplate = CreateTemplateDialog(Template::TemplateType::Command);
      m_currentCommands.emplace_back(std::make_shared<EventDummy>());
      m_currentCommands.back()->setIndent(0);
      m_commandEditor.setCommands(&m_currentCommands);
    }
  }

private:
  Templates* m_templates = nullptr;
  std::vector<Templates*> m_template;
  std::vector<std::shared_ptr<IEventCommand>> m_currentCommands;
  int m_id{0};
  int m_selection{0};
  int m_templateType{0};
  std::string m_templateName;
  std::string m_templateNote;
  bool m_hasChanges = false;
  EventCommandEditor m_commandEditor;

  std::shared_ptr<IDBTemplates> m_currentTemplate;

  std::shared_ptr<IDBTemplates> CreateTemplateDialog(const Template::TemplateType type) const {
    if (m_templates->templates.empty()) {
      return std::make_shared<TemplatesCommand>(nullptr);
    }
    switch (type) {
    case Template::TemplateType::Command:
      return std::make_shared<TemplatesCommand>(&m_templates->templates.at(m_selection));
    case Template::TemplateType::Tint:
      return std::make_shared<TemplatesTint>(&m_templates->templates.at(m_selection));
    case Template::TemplateType::Event:
      return std::make_shared<TemplatesEventProperties>(&m_templates->templates.at(m_selection));
    default:
      return nullptr;
    }
  }
  void AddTemplate(const std::string& label, Template::TemplateType type, const std::string& commandString, const std::vector<int>& params);
  void SetTemplate();
  void SaveToFile() const;
  void SaveChanges();
};
