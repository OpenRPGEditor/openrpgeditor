#include "Core/DatabaseEditor/DBArmorsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBArmorsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Armors");
  ImGui::EndChild();
}