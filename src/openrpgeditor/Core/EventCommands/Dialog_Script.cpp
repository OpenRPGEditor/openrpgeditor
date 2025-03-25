#include "Core/EventCommands/Dialog_Script.hpp"
#include "Core/Application.hpp"

#include "Core/Log.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <tuple>

std::tuple<bool, bool> Dialog_Script::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
    // SetOpen(false);
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{600, 600}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open)) {

    ImGui::PushFont(App::APP->getMonoFont());
    auto size = ImGui::CalcTextSize("OKCANCEL");
    ImGui::InputTextMultiline(
        "##no_title", &m_Text,
        {ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, ImGui::GetContentRegionAvail().y - (size.y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y)},
        ImGuiInputTextFlags_AllowTabInput);
    // m_textEditor.Render("##no_title", {0, ImGui::GetContentRegionAvail().y - (size.y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y)});
    ImGui::PopFont();

    if (ImGui::Button("OK")) {
      auto script = m_textEditor.GetText();
      std::vector<std::string> scripts = splitString(script, '\n');
      if (scripts.size() > 1)
        m_isNext = true;

      command->script = scripts.front();
      if (m_isNext) {
        command->moreScript.clear();
        command->moreScript.reserve(scripts.size());
        for (auto str = std::next(scripts.begin()); str != scripts.end(); ++str) {
          command->moreScript.emplace_back(std::make_shared<NextScriptCommand>())->script = *str;
        }
        command->moreScript.shrink_to_fit();
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
