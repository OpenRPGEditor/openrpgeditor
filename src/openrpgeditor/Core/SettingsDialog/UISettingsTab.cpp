#include "Core/SettingsDialog/UISettingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>
#include <imgui_internal.h>

void UISettingsTab::draw() {
  ImGui::TextUnformatted(trNOOP("UI Scale"));
  ImGui::SetNextItemWidth(-1);
  if (ImGui::SliderFloat("##ui_scale", &Settings::instance()->uiScale, 0.5f, 2.0)) {
    emit_signal(onValueChanged);
    emit_signal(onUIRefreshRequested);
  }
}
