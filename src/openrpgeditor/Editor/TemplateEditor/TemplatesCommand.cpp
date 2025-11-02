#include "Editor/TemplateEditor/TemplatesCommand.hpp"
#include "Database/Templates.hpp"
#include "Editor/Application.hpp"
#include "Editor/DatabaseEditor/IDBEditorTab.hpp"

#include "imgui.h"

TemplatesCommand::TemplatesCommand(Template* templ)
: m_currentTemplate(templ) {}

void TemplatesCommand::draw() {};