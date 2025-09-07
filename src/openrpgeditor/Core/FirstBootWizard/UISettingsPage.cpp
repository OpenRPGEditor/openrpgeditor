#include "Core/FirstBootWizard/UISettingsPage.hpp"

#include "Core/Application.hpp"
#include "Core/Settings.hpp"

#include <imgui.h>

static inline float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void UISettingsPage::draw() {
  ImGui::BeginChild("UISettingsPage", ImVec2(0, 0), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiWindowFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground);
  ImGui::Text("UI Scale");
  ImGui::SetNextItemWidth(-1);
  if (ImGui::SliderFloat("##ui_scale", &Settings::instance()->uiScale, 0.5f, 2.f)) {
    App::APP->requestScaleUpdate();
    m_uiScaleChanged = true;
  }
  
  ImGui::EndChild();
}
