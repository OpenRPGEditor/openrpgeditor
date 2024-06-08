#include "Core/DatabaseEditor/DBClassesTab.hpp"
#include <imgui.h>

void DBClassesTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Classes");
  ImGui::EndChild();
}
