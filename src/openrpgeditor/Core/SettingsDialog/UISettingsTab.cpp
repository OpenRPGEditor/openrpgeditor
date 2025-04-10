#include "Core/SettingsDialog/UISettingsTab.hpp"
#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include <imgui.h>

void UISettingsTab::draw() {
  if (ImGui::BeginTabItem("UI")) {
    ImGui::Text("Font Size");
    ImGui::SliderInt("##ui_font_size", &Settings::instance()->fontSize, 16, 48);
    Settings::instance()->fontSize = nextMultipleOf8(Settings::instance()->fontSize);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      emit_signal(onValueChanged);
    }
    ImGui::PushFont(App::APP->getMonoFont());
    ImGui::Text("Monospace Font Size");
    ImGui::SliderInt("##ui_mono_font_size", &Settings::instance()->monoFontSize, 16, 48);
    Settings::instance()->monoFontSize = nextMultipleOf8(Settings::instance()->monoFontSize);
    if (ImGui::IsItemDeactivatedAfterEdit()) {
      App::APP->requestFontUpdate();
      m_fontSizeChanged = true;
      emit_signal(onValueChanged);
    }
    ImGui::PopFont();
    if (m_fontSizeChanged) {
      ImGui::Text("Reloading fonts....");
      m_fontSizeChanged = !App::APP->fontUpdateRequestPerformed();
      emit_signal(onValueChanged);
    }
    ImGui::EndTabItem();
  }
}
