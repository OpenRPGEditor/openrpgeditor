#include "Core/EventCommands/Dialog_ChangeProfile.hpp"

#include "Database/Database.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>
std::tuple<bool, bool> Dialog_ChangeProfile::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeProfile");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(ImGui::CalcTextSize("#").x * 60) + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeProfile", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_profile_main_layout");
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_profile_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::PushID("##change_profile_selection_actor");
        if (ImGui::EllipsesButton(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ActorPicker( Database::instance()->actors->actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      actorGroupBox.end();
      GroupBox profileGroupBox(trNOOP("Profile"), "##change_profile_profile_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (profileGroupBox.begin()) {
        ImGui::InputTextMultiline("##profile_input", &m_profile, ImVec2{-1, ImGui::GetTextLineHeightWithSpacing() * 2}, ImGuiInputTextFlags_CallbackEdit, multilineTextCallback);
      }
      profileGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##change_profile_buttons", {-1, -1});
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_profile_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actor = m_actor;
          m_command->profile = m_profile;
          ImGui::CloseCurrentPopup();
          setOpen(false);
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

void Dialog_ChangeProfile::drawPickers() {
  if (m_actorPicker) {
    auto [closed, confirmed] = m_actorPicker->draw();
    if (closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
        m_actorPicker.reset();
      }
      m_actorPicker.reset();
    }
  }
}

int Dialog_ChangeProfile::multilineTextCallback(ImGuiInputTextCallbackData* data) {
  std::string multilineText;
  multilineText.resize(data->BufTextLen);
  strncpy(multilineText.data(), data->Buf, data->BufTextLen);

  auto profileLines = splitString(multilineText, '\n');
  multilineText.clear();
  bool dirty = false;
  if (profileLines.size() >= 2) {
    profileLines.resize(2);
    dirty = true;
  }
  for (int i = 0; const auto& line : profileLines) {
    multilineText += i > 0 ? "\n" + line : line;
    ++i;
  }

  if (dirty) {
    memset(data->Buf, 0, data->BufSize);
    strncpy(data->Buf, multilineText.c_str(), multilineText.length());
    data->BufTextLen = strlen(data->Buf);
    data->BufDirty = true;
    data->CursorPos = data->BufTextLen;
  }
  return 0;
}
