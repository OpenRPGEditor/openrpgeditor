#include "Core/FirstBootWizard/ExperimentalWarning.hpp"

#include "Core/Application.hpp"

#include <imgui.h>

void ExperimentalWarning::draw() {
  ImGui::BeginHorizontal("##wizard_experimental_page_warning", {-1, 0}, 0);
  {
    ImGui::PushFont(ImGui::GetFont(), ImGui::GetCurrentContext()->FontSizeBase * 2);
    ImGui::PushStyleColor(ImGuiCol_Text, {1, 0, 0, 1});
    ImGui::Spring(0.5f);
    ImGui::TextUnformatted(trNOOP("⚠ EXPERIMENTAL ⚠"));
    ImGui::Spring(0.5f);
    ImGui::PopStyleColor();
    ImGui::PopFont();
  }
  ImGui::EndHorizontal();
  ImGui::TextUnformatted(
      trNOOP("\n"
             "This version of Open RPG Editor is EXPERIMENTAL and is not recommended for regular development use.\n"
             "The UI is preliminary and subject to change, and there may be edge cases that can cause data corruption.\n"
             "Be sure to keep backups of your projects just in case you encounter a bug or oversight.\n"
             "If you're not comfortable using this software, please close the software and continue using RPG Maker\nfor the time being.\n"));
}