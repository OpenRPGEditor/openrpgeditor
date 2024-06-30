#include "Dialog_Script.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"

std::tuple<bool, bool> Dialog_Script::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    char scriptText[4096];
    if (ImGui::InputTextMultiline("##source", scriptText, IM_ARRAYSIZE(scriptText), ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y - 50), flags));
    {
      script = scriptText;
    }
    if (ImGui::Button("OK")) {
      std::vector<std::string> scripts = splitString(script, ',');
      command->script = scripts.front();
      for (auto str = std::next(scripts.begin()); str != scripts.end(); ++str) {
        m_isNext = true;
        //nextScripts.emplace_back();
        //nextScripts.back()->setNextString(*str);
      }
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      SetOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
