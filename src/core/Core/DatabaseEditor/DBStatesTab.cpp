#include "Core/DatabaseEditor/DBStatesTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBStatesTab::draw() {
  ImGui::BeginChild("#orpg_states_tab");
  ImGui::Text("States");
  ImGui::EndChild();
}
