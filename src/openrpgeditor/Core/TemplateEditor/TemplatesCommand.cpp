#include "Core/TemplateEditor/TemplatesCommand.hpp"
#include "Core/Application.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/Templates.hpp"

#include "imgui.h"

TemplatesCommand::TemplatesCommand(Template* templ, DatabaseEditor* parent)
: IDBTemplates(parent)
, m_parent(parent)
, m_currentTemplate(templ) {}

void TemplatesCommand::draw() {};