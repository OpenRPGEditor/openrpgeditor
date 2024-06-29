#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBSystemTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("System");
  ImGui::EndChild();
}
