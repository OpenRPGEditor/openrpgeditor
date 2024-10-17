#include "Core/NWJSVersionManager.hpp"
#include "Core/Settings.hpp"

#include "imgui.h"
#include "App/ProjectInfo.hpp"
#include "Database/EventCommands/ChangeVictoryME.hpp"
#include <tinyxml2.h>
#include <cstring>
#include <iostream>
#include <curl/curl.h>
#include <memory>

#include <SDL2/SDL.h>

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
  return std::distance(std::istream_iterator<std::string>(iss1), std::istream_iterator<std::string>()) >
         std::distance(std::istream_iterator<std::string>(iss2), std::istream_iterator<std::string>());
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

NWJSVersionManager::NWJSVersionManager(std::string_view nwjsPath) : m_NWJSPath(nwjsPath) {}

size_t NWJSVersionManager::indexWriteCallback(const void* contents, const size_t size, const size_t nmemb,
                                              std::vector<char>* userp) {
  const auto* buf = static_cast<const char*>(contents);
  for (size_t i = 0; i < size * nmemb; ++i) {
    userp->push_back(buf[i]);
  }
  return size * nmemb;
}

size_t NWJSVersionManager::payloadWriteCallback(const void* content, const size_t size, const size_t nmemb,
                                                FILE* userp) {
  return fwrite(content, size, nmemb, userp);
}

bool NWJSVersionManager::getListing() {
  CURL* curl = curl_easy_init();
  CURLcode res = CURLE_FAILED_INIT;

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, (m_NWJSPath + "/index.html").c_str());
    std::vector<char> buffer;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, indexWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::string index(buffer.begin(), buffer.end());
    std::vector<std::string> lines = splitString(index, '\n');
    for (const auto& line : lines) {
      const auto& version = extractVersionFromHtml(line);
      if (version.empty() || version[0] != 'v') {
        continue;
      }

      m_versions.push_back(version);
    }

    /* Sort versions in descending order to the latest (largest version number) is the first in the list */
    std::sort(m_versions.begin(), m_versions.end(), versionCompare);
  }

  return res == CURLE_OK;
}

void NWJSVersionManager::draw() {
  if (!m_isOpen) {
    return;
  }

  if (m_versions.empty()) {
    if (getListing() && !Settings::instance()->currentNWJSVersion.empty()) {
      for (int i = 0; i < m_versions.size(); ++i) {
        if (m_versions[i] == Settings::instance()->currentNWJSVersion) {
          m_selectedVersion = i;
        }
      }
    }
  }

  if (ImGui::Begin("NWJS Version Manager", &m_isOpen, ImGuiWindowFlags_AlwaysAutoResize)) {
    if (m_versions.empty()) {
      ImGui::Text("Unable to get version list!");
    } else {
      ImGui::Text("Current Version: %s", Settings::instance()->currentNWJSVersion.empty()
                                             ? "None"
                                             : Settings::instance()->currentNWJSVersion.c_str());
      ImGui::Text("Latest Version: %s", m_versions[0].c_str());
      ImGui::SeparatorText("Available Versions");
      std::string version = m_selectedVersion < 0 ? "None" : m_versions[m_selectedVersion];
      if (ImGui::BeginCombo("##NWJSVersion", version.c_str())) {
        if (ImGui::Selectable("None", m_selectedVersion == -1)) {
          m_selectedVersion = -1;
          Settings::instance()->currentNWJSVersion.clear();
        }
        int i = 0;
        for (const auto& version : m_versions) {
          if (ImGui::Selectable(version.c_str(), i == m_selectedVersion)) {
            m_selectedVersion = i;
            Settings::instance()->currentNWJSVersion = version;
          }
          ++i;
        }
        ImGui::EndCombo();
      }
      ImGui::SameLine();
      ImGui::BeginDisabled(m_selectedVersion == -1);
      if (ImGui::Button("Download")) {
        CURL* curl = curl_easy_init();
        curl_off_t length = 0;
        std::string versionPath = (m_NWJSPath + "/" + m_versions[m_selectedVersion] + "/nwjs-sdk-" + m_versions[m_selectedVersion] + "-linux-x64.tar.gz");
        curl_easy_setopt(curl, CURLOPT_URL, versionPath.c_str());
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &length);
        std::cout << "Downloading NWJS version " << versionPath << std::endl;
        std::cout << "length = " << length << std::endl;

        const char* conf_path = SDL_GetPrefPath(App::COMPANY_NAMESPACE, App::APP_NAME);
        std::string user_config_path{conf_path};
        SDL_free((void*)conf_path);

        user_config_path +=  std::string("/nwjs-sdk-") + m_versions[m_selectedVersion] + "-linux-x64.tar.gz";

        FILE* f = fopen(user_config_path.c_str(), "wb");

        curl_easy_setopt(curl, CURLOPT_NOBODY, 0);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, payloadWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(f);
      }
      ImGui::EndDisabled();
    }
  }
  ImGui::End();
}
