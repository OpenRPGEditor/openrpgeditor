#include "Core/CommonUI/Preview.hpp"

#include "Database/Database.hpp"
#include "Database/EventCommands/Script.hpp"
#include "Database/EventCommands/ShowPicture.hpp"
#include "Database/EventCommands/Wait.hpp"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <format>

#include <algorithm>
#include <regex>

Preview::Preview(int commonId)
: IDialogController("Preview") {
  m_interpreter.emplace(Database::instance()->commonEvents->event(commonId)->commands());
  for (const int frameIndex : m_interpreter.value().getKeyFrames()) {
    m_keyFrames.push_back(frameIndex);
  }
  for (const int variable : m_interpreter.value().getVariables()) {
    m_variables[variable] = 0;
  }
  for (const int sw : m_interpreter.value().getSwitches()) {
    m_switches[sw] = false;
  }
}

std::tuple<bool, bool> Preview::draw() {
  if (!m_isPaused) {
    m_interpreter.value().update();
    m_selectedKeyFrame = m_interpreter->getKeyFrameIndex();
  }

  const std::string title = std::format("{}##{}", m_name, reinterpret_cast<uintptr_t>(this));
  if (isOpen()) {
    ImGui::OpenPopup(title.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{ImGui::GetMainViewport()->Size.x / 2, 900}, ImGuiCond_Once);
  if (ImGui::BeginPopupModal(title.c_str(), &m_open, ImGuiWindowFlags_NoSavedSettings)) {

    ImGui::BeginChild("##orpg_preview_navigation_bar", ImVec2{ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, 30.f});
    {
      if (ImGui::Button(m_isPaused ? "\u25B6" : "\u23F8", ImVec2{50.f, 0})) {
        m_isPaused = !m_isPaused;
      }
      ImGui::SameLine();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
      if (ImGui::Button(m_isFastForward ? "2x" : "1x", ImVec2{50.f, 0})) {
        m_isFastForward = !m_isFastForward;
        m_interpreter.value().setFastForward(m_isFastForward);
      }
      ImGui::SameLine();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
      if (ImGui::Button("Test", ImVec2{50.f, 0})) {}
      if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
      {
        if (!m_variables.empty()) {
          int index{0};
          for (auto& pair : m_variables) {
            if (ImGui::InputInt(std::format("{}##orpg_preview_context_variablePreview_{}", Database::instance()->variableName(pair.first), index).c_str(), &pair.second)) {
              m_interpreter.value().setVariable(pair.first, pair.second);
            }
            index++;
          }
        }
        if (!m_switches.empty()) {
          int index{0};
          for (auto& pair : m_switches) {
            if (ImGui::Checkbox(std::format("{}##orpg_preview_context_switchPreview_{}", Database::instance()->variableName(pair.first), index).c_str(), &pair.second)) {
              m_interpreter.value().setSwitch(pair.first, pair.second);
            }
            index++;
          }
        }
        if (m_switches.empty() && m_variables.empty()) {
          ImGui::TextUnformatted("No variables or switches found!");
        }
        // if (ImGui::Button("Close")) {
        //   ImGui::CloseCurrentPopup();
        // }
        ImGui::EndPopup();
      }
      ImGui::SetItemTooltip("Right-click to open popup");
    }
    ImGui::EndChild();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.f);
    ImGui::BeginChild("##orpg_preview_navigation_keyframes",
                      ImVec2{ImGui::GetContentRegionMax().x - ImGui::GetStyle().FramePadding.x, m_keyFrames.empty() ? 30.f : (m_keyFrames.size() > 17 ? 50.f : 30.f)}, 0,
                      ImGuiWindowFlags_HorizontalScrollbar);
    {
      for (int i = 0; i < m_keyFrames.size(); i++) {
        std::string text = m_selectedKeyFrame > m_keyFrames.size() - 1 && i == m_keyFrames.size() - 1 ? "\u25D8" : (m_selectedKeyFrame == i ? "\u25D8" : "\u25E6");

        if (i > 0) {
          ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8.f);
        }
        if (ImGui::Button(std::format("{}##{}", text, i).c_str(), ImVec2(50.f, 0.f))) {
          m_isPaused = true;
          m_interpreter.value().setIndex(m_keyFrames[i], i);
          m_selectedKeyFrame = m_interpreter->getKeyFrameIndex();
        }
        if (m_selectedKeyFrame == i) {
          // ImGui::SetScrollHereX(); // TODO: Funny behavior when using this. Need to smoothen it out.
        }
        if (i < m_keyFrames.size() - 1) {
          ImGui::SameLine();
        }
      }
    }
    ImGui::EndChild();
    ImGui::BeginGroup();
    {
      if (m_interpreter.value().image()) {
        ImGui::Image(m_interpreter.value().getImage().texture(),
                     ImVec2{static_cast<float>(m_interpreter.value().getImage().imageWidth()), static_cast<float>(m_interpreter.value().getImage().imageHeight())});
        // ImGui::SameLine();
        // ImGui::TextUnformatted(m_interpreter.value().getImage().imageName().c_str());
      }
    }
    ImGui::EndGroup();
    ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - (ImGui::GetStyle().FramePadding.y * 5));
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
