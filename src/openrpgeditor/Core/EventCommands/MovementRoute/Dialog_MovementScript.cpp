#include "Dialog_MovementScript.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/Application.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_MovementScript::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    ImGui::PushFont(App::APP->getMonoFont());
    m_textEditor.Render("##no_title", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x,
                                             ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(32) -
                                                 ImGui::GetStyle().FramePadding.y});
    ImGui::PopFont();
    if (ImGui::Button("OK")) {
      auto script = m_textEditor.GetText();
      std::vector<std::string> scripts = splitString(script, '\n');
      if (scripts.size() > 1)
        m_isNext = true;

      command->script = scripts.front();
      /*
      if (m_isNext) {
        command->moreScript.clear();
        command->moreScript.reserve(scripts.size());
        for (auto str = std::next(scripts.begin()); str != scripts.end(); ++str) {
          command->moreScript.emplace_back(std::make_shared<NextScriptCommand>())->script = *str;
        }
        command->moreScript.shrink_to_fit();
      }
      */
      ImGui::CloseCurrentPopup();
      SetOpen(false);
      m_confirmed = true;
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
