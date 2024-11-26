#include "Core/EventCommands/Dialog_Comment.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_Comment::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    if (ImGui::InputTextMultiline("##source", script, IM_ARRAYSIZE(script), ImVec2{575, 550} * App::DPIHandler::get_ui_scale(), flags)) {}
    if (ImGui::Button("OK")) {
      std::vector<std::string> texts = splitString(script, '\n');
      if (texts.size() > 1)
        m_isNext = true;

      command->text = texts.front();
      if (m_isNext) {
        command->nextComments.clear();
        command->nextComments.reserve(texts.size());
        for (auto str = std::next(texts.begin()); str != texts.end(); ++str) {
          command->nextComments.emplace_back(std::make_shared<NextCommentCommand>())->text = *str;
        }
        command->nextComments.shrink_to_fit();
      }
      ImGui::CloseCurrentPopup();
      setOpen(false);
      m_confirmed = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
