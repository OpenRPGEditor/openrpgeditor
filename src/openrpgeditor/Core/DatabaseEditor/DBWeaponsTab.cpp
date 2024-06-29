#include "Core/DatabaseEditor/DBWeaponsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBWeaponsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Weapons");
  ImGui::EndChild();
}