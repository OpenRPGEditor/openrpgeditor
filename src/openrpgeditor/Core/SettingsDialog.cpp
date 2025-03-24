#include "Core/SettingsDialog.hpp"

#include "Core/Application.hpp"
#include "Core/Settings.hpp"
#include "imgui.h"

std::tuple<bool, bool> SettingsDialog::draw() {
  if (m_uiRefreshRequested) {
    App::APP->requestFontUpdate();
    m_uiRefreshRequested = false;
    m_wasUIRefreshRequested = true;
  }

  if (isOpen()) {
    if (m_tempSettings.empty()) {
      m_tempSettings = Settings::instance()->serializeToJson();
    }
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 3, ImGuiCond_Always);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      if (ImGui::BeginTabBar("##settings_tabbar")) {
        for (const auto& tab : m_tabs) {
          tab->draw();
        }
        ImGui::EndTabBar();
      }
    }
    ImGui::EndGroup();
    const auto textSize = ImGui::CalcTextSize("OKAPPLYCANCEL");
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (textSize.y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y));
    ImGui::Separator();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (textSize.x + (ImGui::GetStyle().FramePadding.x * 2) + (ImGui::GetStyle().ItemSpacing.x * 4)));
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;

        m_wasUIRefreshRequested = false;
        m_tempSettings.clear();
        ImGui::CloseCurrentPopup();
        if (m_hasChangedSettings) {
          App::APP->serializeSettings();
        }
        m_uiRefreshRequested = false;
        m_hasChangedSettings = false;
        m_closedHere = true;
      }
      ImGui::SameLine();
      ImGui::BeginDisabled(!m_hasChangedSettings);
      if (ImGui::Button("Apply")) {
        m_confirmed = false;
        m_open = true;

        m_tempSettings = Settings::instance()->serializeToJson();
        if (m_wasUIRefreshRequested) {
          App::APP->requestFontUpdate();
          m_wasUIRefreshRequested = false;
        }

        if (m_hasChangedSettings) {
          App::APP->serializeSettings();
          m_hasChangedSettings = false;
        }
      }
      ImGui::EndDisabled();
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmed = false;
        m_open = false;
        m_hasChangedSettings = false;
        Settings::instance()->loadFromJson(m_tempSettings);
        m_uiRefreshRequested = true;
        m_tempSettings.clear();
        if (m_wasUIRefreshRequested) {
          App::APP->requestFontUpdate();
          m_wasUIRefreshRequested = false;
        }
        ImGui::CloseCurrentPopup();
        m_closedHere = true;
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();

    if (!m_open && !m_closedHere) {
      Settings::instance()->loadFromJson(m_tempSettings);
      m_tempSettings.clear();
      if (m_wasUIRefreshRequested) {
        App::APP->requestFontUpdate();
        m_wasUIRefreshRequested = false;
      }
      m_closedHere = false;
    }
  }
  return {!m_open, m_confirmed};
}

void SettingsDialog::onValueChanged() { m_hasChangedSettings = true; }
void SettingsDialog::onUIRefreshRequested() { m_uiRefreshRequested = true; }
