#include "Core/EventCommands/Dialog_Comment.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Log.hpp"

#include "misc/cpp/imgui_stdlib.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <tuple>

std::tuple<bool, bool> Dialog_Comment::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###Comment");
  }
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetTextLineHeightWithSpacing() * 9) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  if (ImGui::BeginPopupModal(std::format("{}###Comment", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::BeginVertical("##comment_main_layout", ImGui::GetContentRegionAvail());
    {
      static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
      ImGui::InputTextMultiline("##source", &m_comment, ImVec2{-1, (ImGui::GetTextLineHeight() * 6) + (ImGui::GetStyle().FramePadding.y * 2)}, flags);
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##comment_buttons_layout");
      {
        ImGui::Spring();
        auto ret = ImGui::ButtonGroup("##comment_buttons", {trNOOP("OK"), trNOOP("Cancel")});
        if (std::ranges::count(m_comment, '\n') >= 6) {
          ret = 0;
        }

        if (ret == 0) {
          std::vector<std::string> texts = splitString(m_comment, '\n');
          // This should never happen but just to make sure we only get 6 lines per comment
          if (texts.size() > 6) {
            texts.resize(6);
          }
          
          if (texts.size() > 0) {
            m_command->text = texts.front();
          }
          if (texts.size() > 1) {
            m_command->nextComments.clear();
            m_command->nextComments.reserve(texts.size());
            for (auto str = std::next(texts.begin()); str != texts.end(); ++str) {
              m_command->nextComments.emplace_back(std::make_shared<NextCommentCommand>())->text = *str;
            }
            m_command->nextComments.shrink_to_fit();
          }
          ImGui::CloseCurrentPopup();
          setOpen(false);
          m_confirmed = true;
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}