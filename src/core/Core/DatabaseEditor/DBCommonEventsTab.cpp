#include "Core/DatabaseEditor/DBCommonEventsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBCommonEventsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("CommonEvents");
  ImGui::EndChild();
}
