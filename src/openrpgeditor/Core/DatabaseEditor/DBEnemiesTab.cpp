#include "Core/DatabaseEditor/DBEnemiesTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBEnemiesTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Enemies");
  ImGui::EndChild();
}
