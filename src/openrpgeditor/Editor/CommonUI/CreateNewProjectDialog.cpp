#include "Editor/CommonUI/CreateNewProjectDialog.hpp"

#include "Editor/Application.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Managers/SettingsManager.hpp"

#include <filesystem>
#include <string_view>

#include <SDL3/SDL_dialog.h>

using namespace std::string_view_literals;

void CreateNewProjectDialog::setOpen(bool open) {
  IDialogController::setOpen(open);
  const auto basePath = SettingsManager::instance().getValue<std::string>("projectBaseDirectory", {});
  if (!basePath.empty()) {
    int i = 1;
    std::filesystem::path projectPath;
    do {
      ++i;
      projectPath = std::filesystem::path() / std::filesystem::path(std::format("Project{}", i));
    } while (is_directory(projectPath));
    m_gameTitle = m_projectName = std::format("Project{}", i);
  }
}

std::tuple<bool, bool> CreateNewProjectDialog::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_dialogId.c_str());
  }
  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{894, 768}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_dialogId.c_str(), &m_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      const auto mvLocation = SettingsManager::instance().getValue<std::string>("rpgMakerMVLocation", {});
      const auto mzLocation = SettingsManager::instance().getValue<std::string>("rpgMakerMZLocation", {});
      if (!mvLocation.empty() || !mzLocation.empty()) {
        ImGui::BeginGroup();
        {
          ImGui::BeginDisabled(mvLocation.empty());
          if (ImGui::RadioButton(trNOOP("RPG Maker MV"), m_projectType == ProjectType::RPGMV)) {
            m_projectType = ProjectType::RPGMV;
          }
          ImGui::EndDisabled();
          ImGui::SameLine();
          ImGui::BeginDisabled(mzLocation.empty());
          if (ImGui::RadioButton(trNOOP("RPG Maker MZ"), m_projectType == ProjectType::RPGMZ)) {
            m_projectType = ProjectType::RPGMZ;
          }
          // TODO: corescripts combo
          ImGui::EndDisabled();
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
          strncpy(location, SettingsManager::instance().getValue<std::string>("projectBaseDirectory").c_str(), 4096);
          if (ImGui::LabelOverLineEdit("##project_location_label", "Location:", location, 4096, 0.f, nullptr, ImGuiInputTextFlags_None)) {
            SettingsManager::instance().setValue("projectBaseDirectory", location);
          }
          ImGui::SameLine();
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().FramePadding.y);
          ImGui::BeginGroup();
          {
            ImGui::NewLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetStyle().FramePadding.y);
            if (ImGui::Button("Choose...")) {
              const auto basePath = SettingsManager::instance().getValue<std::string>("projectBaseDirectory");
              SDL_ShowOpenFolderDialog(
                  [](void*, const char* const* fileList, int filter) {
                    if (!fileList || !*fileList || filter >= 1) {
                      return;
                    }
                    const std::filesystem::path path{fileList[0]};
                    SettingsManager::instance().setValue("projectBaseDirectory", absolute(path).generic_string());
                  },
                  nullptr, App::APP->getWindow()->getNativeWindow(), !basePath.empty() ? basePath.c_str() : nullptr, false);
            }
          }
          ImGui::EndGroup();
          ImGui::Checkbox("Copy example project from RPG Maker", &m_copyExample);
        }
        ImGui::EndGroup();
      } else {
        ImGui::TextUnformatted(
            trNOOP("RPG Maker Locations have not been specified!\n"
                   "You need to specify either an RPG Maker MV or MZ directory.\n"
                   "You can, optionally, specify both."));
      }
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
