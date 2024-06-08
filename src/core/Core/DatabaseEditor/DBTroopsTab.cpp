#include "Core/DatabaseEditor/DBTroopsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBTroopsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Troops");
  ImGui::EndChild();
}