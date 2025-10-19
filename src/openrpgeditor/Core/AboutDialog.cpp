#include "Core/AboutDialog.hpp"

#include "Application.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <orei18n.hpp>

std::tuple<bool, bool> AboutDialog::draw() {
  if (!isOpen()) {
    return {!m_open, m_confirmed};
  }
  *(volatile int*)(0) = 1;

  if (ImGui::Begin(std::format("{}###OREAboutDialog", trNOOP("About Open RPG Editor")).c_str(), &m_open, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##about_dialog_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::BeginHorizontal("##about_dialog_header", {-1, 0}, 0);
      {
        ImGui::Spring(0.5f);
        ImGui::TextUnformatted(trNOOP("About Open RPG Editor"));
        ImGui::Spring(0.5f);
      }
      ImGui::EndHorizontal();
      ImGui::PushFont(App::APP->getMonoFont());
      const auto aboutText = trNOOP(
          R"(
Open RPG Editor is an open source, drop-in replacement for RPG Maker MV/MZ.
A variety of QoL features are provided to help developers more easily create
their games using a familiar engine.


  DEVELOPERS:
    Phillip "Antidote" Stephens - Project creator, lead developer
    Nexus                       - Project co-creator, lead developer
)");
      ImGui::TextUnformatted(aboutText);
      ImGui::PopFont();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));

      ImGui::BeginHorizontal("##about_dialog_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##about_dialog_buttons", {trNOOP("OK"), trNOOP("About ImGui")}); ret == 0) {
          setOpen(false);
        } else if (ret == 1) {
          m_showImGuiCredits = true;
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }

  ImGui::End();
  if (m_showImGuiCredits) {
    ImGui::ShowAboutWindow(&m_showImGuiCredits);
  }

  return {!m_open, m_confirmed};
}
