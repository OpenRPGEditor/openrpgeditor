#include "Editor/EventCommands/Dialog_Script.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <algorithm>
#include <imgui.h>
#include <imgui_internal.h>

namespace {

ImVec2 scriptDialogMinSize() { return ImGui::GetDPIScaledSize(880, 400); }

ImVec2 scriptDialogMaxSize() {
  const auto* viewport = ImGui::GetMainViewport();
  return {viewport->Size.x * 0.92f, viewport->Size.y * 0.85f};
}

ImVec2 scriptDialogDefaultSize() {
  const auto minSize = scriptDialogMinSize();
  const auto maxSize = scriptDialogMaxSize();
  const auto* viewport = ImGui::GetMainViewport();
  ImVec2 size = {viewport->Size.x * 0.65f, viewport->Size.y * 0.55f};
  size.x = std::clamp(size.x, minSize.x, maxSize.x);
  size.y = std::clamp(size.y, minSize.y, maxSize.y);
  return size;
}

} // namespace

std::tuple<bool, bool> Dialog_Script::draw() {
  TextEditor::DrawPickers();
  if (isOpen()) {
    ImGui::OpenPopup("###Script");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(scriptDialogDefaultSize(), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSizeConstraints(scriptDialogMinSize(), scriptDialogMaxSize());

  if (ImGui::BeginPopupModal(std::format("{}###Script", m_name).c_str(), &m_open)) {
    ImGui::BeginVertical("##script_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      const float footerHeight = ImGui::GetFrameHeightWithSpacing() + ImGui::GetDPIScaledValue(1.5f) + ImGui::GetStyle().ItemSpacing.y;
      m_textEditor.DrawPanel("TextEditor", false, true, footerHeight);
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##script_buttons_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##script_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          const auto script = m_textEditor.GetText();
          std::vector<std::string> scripts = splitString(script, '\n');

          m_command->script = scripts.front();
          if (scripts.size() > 1) {
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
