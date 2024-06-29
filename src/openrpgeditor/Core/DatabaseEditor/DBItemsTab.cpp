#include "Core/DatabaseEditor/DBItemsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

void DBItemsTab::draw() {
  ImGui::BeginChild("#orpg_classes_tab");
  ImGui::Text("Items");
  ImGui::EndChild();
}