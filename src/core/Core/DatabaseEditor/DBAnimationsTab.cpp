#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Database/Animations.hpp"

#include "imgui.h"

void DBAnimationsTab::draw() {
  ImGui::BeginChild("#orpg_animations_tab");
  ImGui::Text("Animations");
  ImGui::EndChild();
}