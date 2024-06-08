#include "Core/DatabaseEditor/DBSkillsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBSkillsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Skills");
  ImGui::EndChild();
}
