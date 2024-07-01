#include "Dialog_Comment.hpp"
#include <tuple>
#include "imgui.h"
#include "Core/DPIHandler.hpp"
#include "Core/Log.hpp"

std::tuple<bool, bool> Dialog_Comment::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    if (ImGui::InputTextMultiline("##source", script, IM_ARRAYSIZE(script), ImVec2(ImGui::GetContentRegionMax().x - 25, ImGui::GetContentRegionMax().y - 50), flags))
    {

    }
    if (ImGui::Button("OK")) {
      std::vector<std::string> texts = splitString(script, '\n');
      if (texts.size() > 1)
        m_isNext = true;

      command->text = texts.front();
      APP_DEBUG(command->script);
      if (m_isNext) {
        command->nextComments.clear();
        command->nextComments.reserve(texts.size());
        for (auto str = std::next(texts.begin()); str != texts.end(); ++str) {
          command->nextComments.emplace_back(std::make_shared<NextCommentCommand>())->text = *str;
          APP_DEBUG(command->moreScript.back()->script);
        }
        command->nextComments.shrink_to_fit();
      }
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
