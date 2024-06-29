#include "Core/DatabaseEditor/DBTilesetsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBTilesetsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Tilesets");
  ImGui::EndChild();
}