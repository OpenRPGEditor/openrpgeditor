#include "Core/CommonUI/CreateNewProjectDialog.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/Settings.hpp"

#include <filesystem>
#include <string_view>

using namespace std::string_view_literals;
#include "nfd.h"

void CreateNewProjectDialog::setOpen(bool open) {
  IDialogController::setOpen(open);

  if (!Settings::instance()->projectBaseDirectory.empty()) {
    int i = 1;
    std::filesystem::path projectPath;
    do {
      ++i;
      projectPath = std::filesystem::path(Settings::instance()->projectBaseDirectory) / std::filesystem::path(std::format("Project{}", i));
    } while (is_directory(projectPath));
    m_gameTitle = m_projectName = std::format("Project{}", i);
  }
}

std::tuple<bool, bool> CreateNewProjectDialog::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{894, 768}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      ImGui::BeginGroup();
      {
        char name[256]{};
        strncpy(name, m_projectName.c_str(), sizeof(name));
        if (ImGui::LabelOverLineEdit("##project_name_label", "Name:", name, sizeof(name), 0.f, nullptr, ImGuiInputTextFlags_None)) {
          m_projectName = name;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
        char title[4096]{};
        strncpy(title, m_gameTitle.c_str(), sizeof(title));
        if (ImGui::LabelOverLineEdit("##project_title_label", "Game Title:", title, sizeof(title), 0.f, nullptr, ImGuiInputTextFlags_None)) {
          m_gameTitle = title;
        }
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        char location[4096];
        strncpy(location, Settings::instance()->projectBaseDirectory.c_str(), 4096);
        if (ImGui::LabelOverLineEdit("##project_location_label", "Location:", location, 4096, 0.f, nullptr, ImGuiInputTextFlags_None)) {
          Settings::instance()->projectBaseDirectory = location;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
        ImGui::BeginGroup();
        {
          ImGui::NewLine();
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
          if (ImGui::Button("Choose...")) {
            nfdu8char_t* loc;
            const auto result = NFD_PickFolder(&loc, !Settings::instance()->projectBaseDirectory.empty() ? Settings::instance()->projectBaseDirectory.c_str() : nullptr);
            if (result == NFD_OKAY) {
              const std::filesystem::path path{loc};
              Settings::instance()->projectBaseDirectory = absolute(path).generic_string();
              NFD_FreePathU8(loc);
            }
          }
        }
        ImGui::EndGroup();
        ImGui::Checkbox("Copy example project from RPG Maker", &m_copyExample);
      }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmed = false;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}
