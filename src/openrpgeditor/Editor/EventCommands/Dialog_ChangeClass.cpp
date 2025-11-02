#include "Editor/EventCommands/Dialog_ChangeClass.hpp"

#include "Database/Database.hpp"

#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeClass::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeClass");
  }
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize =
      ImVec2{ImGui::CalcTextSize("#############################").x + (ImGui::GetStyle().FramePadding.x * 2), (ImGui::GetFrameHeightWithSpacing() * 7) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  if (ImGui::BeginPopupModal(std::format("{}###ChangeClass", m_name).c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
    drawPickers();

    ImGui::BeginVertical("##change_class_main_layout", ImGui::GetContentRegionAvail());
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_class_actor_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (actorGroupBox.begin()) {
        ImGui::PushID("##change_class_actor");
        if (ImGui::EllipsesButton(Database::instance()->actorNameAndId(m_actor).c_str(), ImVec2{-1, 0})) {
          m_actorPicker = ActorPicker(Database::instance()->actors->actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      actorGroupBox.end();

      GroupBox classGroupBox(trNOOP("Class"), "##change_class_class_group", {-1, 0}, nullptr, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
      if (classGroupBox.begin()) {
        ImGui::PushID("##change_class_classid");
        if (ImGui::EllipsesButton(Database::instance()->classNameAndId(m_class).c_str(), ImVec2{-1, 0})) {
          m_classPicker = ClassPicker(Database::instance()->classes->classes(), m_class);
          m_classPicker->setOpen(true);
        }
        ImGui::PopID();
      }
      classGroupBox.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5));
      ImGui::BeginHorizontal("##change_class_buttons");
      {
        // saveLevel checkbox
        ImGui::Checkbox("Save Level", &m_saveLevel);
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##change_class_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actor = m_actor;
          m_command->classId = m_class;
          m_command->saveLevel = m_saveLevel;
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

void Dialog_ChangeClass::drawPickers() {
  if (m_actorPicker) {
    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        m_actor = m_actorPicker->selection();
      }
      m_actorPicker.reset();
    }
  }
  if (m_classPicker) {
    if (const auto [closed, confirmed] = m_classPicker->draw(); closed) {
      if (confirmed) {
        m_class = m_classPicker->selection();
      }
      m_classPicker.reset();
    }
  }
}