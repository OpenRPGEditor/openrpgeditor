#include "Core/NWJSVersionManager.hpp"
#include "Core/Settings.hpp"

#include "App/ProjectInfo.hpp"
#include "Database/EventCommands/ChangeVictoryME.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>

#include <regex>
#include <string_view>

bool versionCompare(const std::string& v1, const std::string& v2) {
  std::istringstream iss1(v1);
  std::istringstream iss2(v2);
  std::string token1, token2;

  while (std::getline(iss1, token1, '.') && std::getline(iss2, token2, '.')) {
    if (token1[0] == 'v') {
      token1.erase(0, 1);
    }
    if (token2[0] == 'v') {
      token2.erase(0, 1);
    }
    int num1 = std::stoi(token1);
    int num2 = std::stoi(token2);

    if (num1 != num2) {
      return num1 > num2; // Change to '<' for ascending order
    }
  }

  // If one version has more parts than the other
  return std::distance(std::istream_iterator<std::string>(iss1), std::istream_iterator<std::string>()) > std::distance(std::istream_iterator<std::string>(iss2), std::istream_iterator<std::string>());
}

std::string extractVersionFromHtml(const std::string& html) {
  // Regex pattern to capture the version value without the trailing slash
  std::regex pattern(R"(href="([^/]+)/)");
  std::smatch matches;

  if (std::regex_search(html.begin(), html.end(), matches, pattern)) {
    return matches[1]; // Return the extracted version
  }
  return ""; // Return an empty string if no match is found
}

NWJSVersionManager::NWJSVersionManager(std::string_view nwjsPath)
: m_NWJSPath(nwjsPath) {}

bool NWJSVersionManager::getListing() {
  std::filesystem::path path = std::filesystem::temp_directory_path() / "nwjs_versions.html";
  if (m_versionDownloadHandle == -1) {
    m_versionDownloadHandle = m_downloadManager.addDownload(m_NWJSPath, (std::filesystem::temp_directory_path() / "nwjs_versions.html").generic_string());
  }
  
  if (!(m_versionDownloadHandle != -1 && m_downloadManager.transferComplete(m_versionDownloadHandle)) || !exists(path)) {
    return false;
  }

  std::ifstream file(path.generic_string(), std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  for (const auto& l : lines) {
    const auto& version = extractVersionFromHtml(l);
    if (version.empty() || version[0] != 'v') {
      continue;
    }

    m_versions.push_back(version);
  }

  /* Sort versions in descending order to the latest (largest version number) is the first in the list */
  std::ranges::sort(m_versions, versionCompare);
  remove(path);
  return !m_versions.empty();
}

void NWJSVersionManager::draw() {
  if (!m_isOpen) {
    return;
  }

  m_downloadManager.processDownloads();

  if (m_versions.empty()) {
    if (getListing() && !Settings::instance()->currentNWJSVersion.empty()) {
      for (int i = 0; i < m_versions.size(); ++i) {
        if (m_versions[i] == Settings::instance()->currentNWJSVersion) {
          m_selectedVersion = i;
        }
      }
    }
  }

  if (ImGui::Begin("NWJS Version Manager", &m_isOpen)) {
    if (m_versions.empty()) {
      ImGui::TextUnformatted((m_versionDownloadHandle == -1 || m_downloadManager.transferComplete(m_versionDownloadHandle)) ? trNOOP("Unable to get version list!")
                                                                                                                            : trNOOP("Retrieving version listing..."));
    } else {
      ImGui::Text(trNOOP("Current Version: %s"), Settings::instance()->currentNWJSVersion.empty() ? trNOOP("None") : Settings::instance()->currentNWJSVersion.c_str());
      ImGui::Text(trNOOP("Latest Version: %s"), m_versions[0].c_str());
      ImGui::SeparatorText(trNOOP("Available Versions"));
      std::string version = m_selectedVersion < 0 ? trNOOP("None") : m_versions[m_selectedVersion];
      if (ImGui::BeginCombo("##NWJSVersion", version.c_str())) {
        if (ImGui::Selectable(trNOOP("None"), m_selectedVersion == -1)) {
          m_selectedVersion = -1;
          Settings::instance()->currentNWJSVersion.clear();
        }
        int i = 0;
        for (const auto& v : m_versions) {
          if (ImGui::Selectable(v.c_str(), i == m_selectedVersion)) {
            m_selectedVersion = i;
            Settings::instance()->currentNWJSVersion = v;
          }
          ++i;
        }
        ImGui::EndCombo();
      }
      ImGui::SameLine();
      ImGui::BeginDisabled(m_selectedVersion == -1);
      // TODO: Add ia32, x64, and arm64 checkboxes for each applicable platform
      // TODO: Figure out issues with DownloadManager
      if (ImGui::Button(trNOOP("Download"))) {
        const char* confPath = SDL_GetPrefPath(App::COMPANY_NAMESPACE, App::APP_NAME);
        if (m_downloadWindows) {
          m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-{1}-win-x64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                        std::format("{0}/nwjs-{1}-win-x64.zip", confPath, m_versions[m_selectedVersion]));
          if (m_downloadSDK) {
            m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-sdk-{1}-win-x64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                          std::format("{0}/nwjs-sdk-{1}-win-x64.zip", confPath, m_versions[m_selectedVersion]));
          }
        }

        if (m_downloadMacOS) {
          m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-{1}-osx-x64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                        std::format("{0}/nwjs-{1}-osx-x64.zip", confPath, m_versions[m_selectedVersion]));
          m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-{1}-osx-arm64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                        std::format("{0}/nwjs-{1}-osx-arm64.zip", confPath, m_versions[m_selectedVersion]));
          if (m_downloadSDK) {
            m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-sdk-{1}-osx-x64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                          std::format("{0}/nwjs-sdk-{1}-osx-x64.zip", confPath, m_versions[m_selectedVersion]));
            m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-sdk-{1}-osx-arm64.zip", m_NWJSPath, m_versions[m_selectedVersion]),
                                          std::format("{0}/nwjs-sdk-{1}-osx-arm64.zip", confPath, m_versions[m_selectedVersion]));
          }
        }

        if (m_downloadLinux) {
          m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-{1}-linux-x64.tar.gz", m_NWJSPath, m_versions[m_selectedVersion]),
                                        std::format("{0}/nwjs-{1}-linux-x64.tar.gz", confPath, m_versions[m_selectedVersion]));
          if (m_downloadSDK) {
            m_downloadManager.addDownload(std::format("{0}/{1}/nwjs-sdk-{1}-linux-x64.tar.gz", m_NWJSPath, m_versions[m_selectedVersion]),
                                          std::format("{0}/nwjs-sdk-{1}-linux-x64.tar.gz", confPath, m_versions[m_selectedVersion]));
          }
        }
        SDL_free((void*)confPath);
      }
      ImGui::EndDisabled();
      ImGui::Checkbox(trNOOP("Windows"), &m_downloadWindows);
      ImGui::SameLine();
      ImGui::Checkbox(trNOOP("macOS"), &m_downloadMacOS);
      ImGui::SameLine();
      ImGui::Checkbox(trNOOP("Linux"), &m_downloadLinux);
      ImGui::SameLine();
      ImGui::Checkbox(trNOOP("SDK"), &m_downloadSDK);
    }

    ImGui::BeginChild("##progress_list", ImGui::GetContentRegionAvail() - ImGui::GetStyle().FramePadding, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginColumns("##progress_columns", 2);
      for (const auto& handle : m_downloadManager.handles()) {
        ImGui::TextUnformatted(handle.url().data());
        ImGui::NextColumn();
        ImGui::ProgressBar(handle.percent());
        ImGui::NextColumn();
      }
      ImGui::EndColumns();
    }
    ImGui::EndChild();
#endif
  }
  ImGui::End();
}
