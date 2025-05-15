#include "Core/EventCommands/Dialog_Script.hpp"
#include "Core/Application.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

std::tuple<bool, bool> Dialog_Script::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###Script");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 40) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 12) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###Script", m_name).c_str(), &m_open)) {
    ImGui::BeginVertical("##script_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      ImGui::PushFont(App::APP->getMonoFont());
      m_textEditor.Render("TextEditor", {0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()}, true);
      ImGui::PopFont();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##script_buttons_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##script_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          const auto script = m_textEditor.GetText();
          std::vector<std::string> scripts = splitString(script, '\n');
          if (scripts.size() > 1) {
            m_isNext = true;
          }

          m_command->script = scripts.front();
          if (m_isNext) {
            m_command->moreScript.clear();
            m_command->moreScript.reserve(scripts.size());
            for (auto str = std::next(scripts.begin()); str != scripts.end(); ++str) {
              m_command->moreScript.emplace_back(std::make_shared<NextScriptCommand>())->script = *str;
              m_command->moreScript.back()->setIndent(m_command->indent().value());
            }
            m_command->moreScript.shrink_to_fit();
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
