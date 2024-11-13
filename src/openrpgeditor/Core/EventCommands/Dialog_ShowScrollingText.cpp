#include "Core/EventCommands/Dialog_ShowScrollingText.hpp"

#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_ShowScrollingText::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{551, 280} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::SeparatorText("Text");
    ImGui::InputTextMultiline("##showscrolling_text", m_textLine, 4096,
                              ImVec2{App::DPIHandler::scale_value(537), App::DPIHandler::scale_value(107)});
    ImGui::Text("Speed: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(App::DPIHandler::scale_value(50));
    if (ImGui::InputInt("##showscrolling_speed", &m_speed, 0)) {
      if (m_speed > 8)
        m_speed = 8;
      if (m_speed < 1)
        m_speed = 1;
    }
    ImGui::SameLine();
    ImGui::Checkbox("No Fast Forward", &m_noFast);
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + App::DPIHandler::scale_value(190));
    if (ImGui::Button("Preview...", ImVec2{App::DPIHandler::scale_value(100), 0})) {
      // TODO
    }
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        command->speed = m_speed;
        command->noFast = m_noFast;
        std::vector<std::string> split = splitString(m_textLine, '\n');
        for (auto str : split) {
          command->text.push_back(std::make_shared<NextScrollingTextCommand>());
          command->text.back()->text = str;
        }
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
        SetOpen(false);
      }
      ImGui::EndGroup();
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
