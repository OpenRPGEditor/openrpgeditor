#include "Editor/SettingsDialog/UISettingsTab.hpp"
#include "Editor/Application.hpp"
#include "Editor/Managers/SettingsManager.hpp"

#include <imgui.h>
#include <imgui_internal.h>

void UISettingsTab::draw() {
  ImGui::TextUnformatted(trNOOP("UI Scale"));
  ImGui::SetNextItemWidth(-1);
  float scale = SettingsManager::instance().getValue("uiScale", 1.f);
  if (ImGui::SliderFloat("##ui_scale", &scale, 0.5f, 2.0)) {
    SettingsManager::instance().setValue("uiScale", scale);
    emit_signal(onValueChanged);
    emit_signal(onUIRefreshRequested);
  }
}
