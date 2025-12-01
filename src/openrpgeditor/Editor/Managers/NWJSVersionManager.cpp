#include "Editor/Managers/NWJSVersionManager.hpp"

#include "ArchiveManager.hpp"
#include "Editor/Settings.hpp"
#include "IconsFontAwesome6.h"

#include "Database/EventCommands/ChangeVictoryME.hpp"
#include "Editor/CommonUI/GroupBox.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "Editor/Log.hpp"
#include "Editor/Process.hpp"
#include "Editor/ProjectInfo.hpp"
#include "Editor/StopWatch.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <cstring>
#include <curl/curl.h>
#include <iostream>
#include <memory>

#include <SDL3/SDL.h>

#include <archive.h>
#include <archive_entry.h>
#include <regex>
#include <string_view>

using namespace std::string_view_literals;

static constexpr auto kLinuxIdentifier = "linux"sv;
static constexpr auto kMacOSIdentifier = "osx"sv;
static constexpr auto kWindowsIdentifier = "win"sv;
static constexpr auto kIA32Identifier = "ia32"sv;
static constexpr auto kX64Identifier = "x64"sv;
static constexpr auto kARM64Identifier = "arm64"sv;

static const std::vector<NWJSVersionManager::Platform> kPlatforms{
    {
        "Linux",
        kLinuxIdentifier.data(),
        "tar.gz",
        "nw",
        {
            {
                "64 Bit",
                kX64Identifier.data(),
            },
            {
                "32 Bit",
                kIA32Identifier.data(),
            },
        },
    },
    {
        "macOS",
        kMacOSIdentifier.data(),
        "zip",
        "nwjs.app/Contents/MacOS/nwjs",
        {
            {
                "Intel",
                kX64Identifier.data(),
            },
            {
                "ARM",
                kARM64Identifier.data(),
            },
        },
    },
    {
        "Windows",
        kWindowsIdentifier.data(),
        "zip",
        "nw.exe",
        {
            {
                "64 Bit",
                kX64Identifier.data(),
            },
            {
                "32 Bit",
                kIA32Identifier.data(),
            },
        },
    },
};

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
    // ReSharper disable CppTooWideScopeInitStatement
    int num2 = std::stoi(token2);
    // ReSharper restore CppTooWideScopeInitStatement

    if (num1 != num2) {
      return num1 > num2; // Change to '<' for ascending order
    }
  }

  // If one version has more parts than the other
  return std::distance(std::istream_iterator<std::string>(iss1), std::istream_iterator<std::string>()) > std::distance(std::istream_iterator<std::string>(iss2), std::istream_iterator<std::string>());
}

std::string extractVersionFromHtml(const std::string& html) {
  // Regex pattern to capture the version value without the trailing slash
  const std::regex pattern(R"(href="([^/]+)/)");

  if (std::smatch matches; std::regex_search(html.begin(), html.end(), matches, pattern)) {
    return matches[1]; // Return the extracted version
  }
  return ""; // Return an empty string if no match is found
}

NWJSVersionManager::NWJSVersionManager()
: m_platforms(kPlatforms) {}

bool NWJSVersionManager::getListing() {
  std::filesystem::path path = std::filesystem::temp_directory_path() / "nwjs_versions.html";
  if (m_versionDownloadHandle == -1) {
    m_versionDownloadHandle = DownloadManager::instance().addDownload(kNWJS_URL, path.generic_string());
  }

  if (!(m_versionDownloadHandle != -1 && DownloadManager::instance().transferComplete(m_versionDownloadHandle)) || !exists(path)) {
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

void NWJSVersionManager::initialize() {
  if (m_configPath.empty()) {
    const char* confPath = SDL_GetPrefPath(App::COMPANY_NAMESPACE, App::APP_NAME);
    m_configPath = confPath;
    SDL_free((void*)confPath);
  }

  if (m_versions.empty()) {
    getListing();
    return;
  }

  if (!Settings::instance()->selectedNWJSVersion.empty()) {
    if (Settings::instance()->selectedNWJSVersion == "latest") {
      m_selectedVersion = -1;
    } else if (!m_versions.empty()) {
      for (int i = 0; i < m_versions.size(); ++i) {
        if (m_versions[i] == Settings::instance()->selectedNWJSVersion) {
          m_selectedVersion = i;
        }
      }
    }
  } else if (!m_versions.empty()) {
    m_selectedVersion = -2;
  }

  detectInstalledVersions();
}

void NWJSVersionManager::draw() {
  if (!m_isOpen) {
    return;
  }

  static StopWatch sw;
  // Detect versions every 5 seconds
  if (sw.elapsedSeconds() >= 5.f) {
    sw.reset();
    detectInstalledVersions();
  }
  const auto windowMinSize = ImGui::GetDPIScaledSize(640, 480);
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Once, {0.5f, 0.5f});
  ImGui::SetNextWindowSize(windowMinSize, ImGuiCond_Once);
  ImGui::SetNextWindowSizeConstraints(windowMinSize, {FLT_MAX, FLT_MAX});
  if (ImGui::Begin(std::format("{}##NWJSVersionManager", trNOOP("NWJS Version Manager")).c_str(), &m_isOpen)) {
    if (m_versions.empty()) {
      ImGui::TextUnformatted(m_versionDownloadHandle == -1 || DownloadManager::instance().transferComplete(m_versionDownloadHandle) ? trNOOP("Unable to get version list!")
                                                                                                                                    : trNOOP("Retrieving version listing..."));
    } else {
      GroupBox availableVersionsGroup(trNOOP("Available Versions"), "##nwjs_available_versions_group", {-1, 0}, nullptr, ImGuiChildFlags_ResizeY, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar);
      if (availableVersionsGroup.begin()) {
        ImGui::BeginVertical("##nwjs_available_version_main_layout", {-1, 0}, 0);
        {
          // TL-NOTE: The second format specifier is update status if the latest version doesn't match the currently set version
          ImGui::Text(trNOOP("Current Version: %s%s"), Settings::instance()->currentNWJSVersion.c_str(), Settings::instance()->currentNWJSVersion != m_versions[0] ? trNOOP(" Update Available!") : "");
          ImGui::Text(trNOOP("Latest Version: %s"), m_versions[0].c_str());
          GroupBox selectedVersion(trNOOP("Selected Version:"), "##selected_version", {-1, 0});
          if (selectedVersion.begin()) {
            const std::string version = m_selectedVersion == -2 ? trNOOP("None") : m_selectedVersion == -1 ? trNOOP("Latest") : m_versions[m_selectedVersion];
            ImGui::SetNextItemWidth(-1);
            if (ImGui::BeginCombo("##NWJSVersion", version.c_str())) {
              if (ImGui::Selectable(trNOOP("None"), m_selectedVersion == -2)) {
                m_selectedVersion = -2;
                Settings::instance()->selectedNWJSVersion.clear();
              }
              if (ImGui::Selectable(trNOOP("Latest"), m_selectedVersion == -1)) {
                m_selectedVersion = -1;
                Settings::instance()->selectedNWJSVersion = "latest";
              }
              int i = 0;
              for (const auto& v : m_versions) {
                if (ImGui::Selectable(v.c_str(), i == m_selectedVersion)) {
                  m_selectedVersion = i;
                  Settings::instance()->selectedNWJSVersion = v;
                }
                ++i;
              }
              ImGui::EndCombo();
            }
            ImGui::SameLine();
          }
          selectedVersion.end();

          bool hasPrevious = false;

          const auto version = m_selectedVersion > -1 ? m_versions[m_selectedVersion] : m_selectedVersion == -1 ? m_versions[0] : "";
          if (!version.empty()) {
            for (int i = 0; auto& platform : m_platforms) {
              ++i;
              if (!(i % 3)) {
                hasPrevious = false;
              }
              if (hasPrevious) {
                ImGui::SameLine();
              }
              GroupBox platformGroup(platform.name, std::format("##nwjs_manager_platform_{}", platform.identifier));
              if (platformGroup.begin()) {
                ImGui::BeginHorizontal(std::format("##nwjs_manager_platform_{}_main_layout", platform.identifier).c_str(), {-1, -1}, 0);
                {
                  for (auto& [name, identifier] : platform.architectures) {
                    GroupBox archGroup(name, std::format("##nwjs_manager_platform_arch_{}_{}_group", platform.identifier, identifier));
                    if (archGroup.begin()) {
                      ImGui::BeginVertical(std::format("##nwjs_manager_platform_arch_{}_{}_group_layout", platform.identifier, identifier).c_str(), {-1, -1}, 0);
                      {
                        GroupBox releaseGroup(trNOOP("Release"), "##nwjs_manager_platform_release_group", {0, 0});
                        if (releaseGroup.begin()) {
                          ImGui::BeginHorizontal(std::format("##nwjs_manager_platform_{}_{}_layout_release", platform.identifier, identifier).c_str(), {-1, 0}, 0);
                          {
                            const bool disableDownload = !platform.availableVersions[version].empty() && platform.availableVersions[version][identifier].deploymentRuntimeDownloaded;
                            const bool disableExtract = !platform.availableVersions[version].empty() && platform.availableVersions[version][identifier].deploymentRuntimeExtracted;
                            if (const auto ret = ImGui::ButtonGroup("##nwjs_manager_platform_release_buttons", {trNOOP("Download"), trNOOP("Extract")}, false, {}, {disableDownload, disableExtract});
                                ret == 0) {
                              addDownload(version, platform.identifier, identifier, platform.extension, false);
                              if (m_selectedVersion == -1) {
                                Settings::instance()->currentNWJSVersion = m_versions[0];
                              }
                            } else if (ret == 1) {
                              ArchiveManager::instance().addJob(archivePathForPlatformAndArch(version, m_configPath / kNWJS_DIR / version, platform.identifier, identifier, platform.extension, false),
                                                                m_configPath / kNWJS_DIR / version / platform.identifier / identifier / "runtime");
                            }
                          }
                          ImGui::EndHorizontal();
                        }
                        releaseGroup.end();
                        GroupBox devGroup(trNOOP("Development"), "##nwjs_manager_platform_dev_group", {0, 0});
                        if (devGroup.begin()) {
                          ImGui::BeginHorizontal(std::format("##nwjs_manager_platform_{}_{}_layout_development", platform.identifier, identifier).c_str(), {-1, 0}, 0);
                          {
                            const bool disableDownload = !platform.availableVersions[version].empty() && platform.availableVersions[version][identifier].developmentRuntimeDownloaded;
                            const bool disableExtract = !platform.availableVersions[version].empty() && platform.availableVersions[version][identifier].developmentRuntimeExtracted;
                            if (const auto ret = ImGui::ButtonGroup("##nwjs_manager_platform_release_buttons", {trNOOP("Download"), trNOOP("Extract")}, false, {}, {disableDownload, disableExtract});
                                ret == 0) {
                              addDownload(version, platform.identifier, identifier, platform.extension, true);
                              if (m_selectedVersion == -1) {
                                Settings::instance()->currentNWJSVersion = m_versions[0];
                              }
                            } else if (ret == 1) {
                              ArchiveManager::instance().addJob(archivePathForPlatformAndArch(version, m_configPath / kNWJS_DIR / version, platform.identifier, identifier, platform.extension, true),
                                                                m_configPath / kNWJS_DIR / version / platform.identifier / identifier / "sdk");
                            }
                          }
                          ImGui::EndHorizontal();
                        }
                        devGroup.end();
                      }
                      ImGui::EndVertical();
                    }
                    archGroup.end();
                  }
                }
                ImGui::EndHorizontal();
              }
              platformGroup.end();
              hasPrevious = true;
            }
          }
        }
        ImGui::EndVertical();
      }
      availableVersionsGroup.end();
    }

    ImGui::BeginChild("##progress_list", ImGui::GetContentRegionAvail() - ImGui::GetStyle().FramePadding, ImGuiChildFlags_Borders, ImGuiWindowFlags_HorizontalScrollbar);
    {
      ImGui::BeginColumns("##progress_columns", 2);
      for (const auto& handle : DownloadManager::instance().handlesFor(this)) {
        ImGui::TextUnformatted(handle->url().data());
        ImGui::NextColumn();
        ImGui::ProgressBar(handle->percent());
        ImGui::NextColumn();
      }
      ImGui::EndColumns();
    }
    ImGui::EndChild();
  }
  ImGui::End();
}

void NWJSVersionManager::addDownload(const std::string_view version, const std::string_view platform, const std::string_view architecture, const std::string_view extension, const bool sdk) const {
  // URL on nwjs download server
  const auto url = std::format("{0}/{1}/nwjs-{2}{1}-{3}-{4}.{5}", kNWJS_URL, version, sdk ? "sdk-" : "", platform, architecture, extension);
  // Local path
  const auto destinationPath = m_configPath / kNWJS_DIR / version / std::format("nwjs-{}{}-{}-{}.{}", sdk ? "sdk-" : "", version, platform, architecture, extension);
  if (!exists(destinationPath.parent_path())) {
    create_directories(destinationPath.parent_path());
  }

  DownloadManager::instance().addDownload(url, destinationPath.generic_string(), this);
}

std::filesystem::path NWJSVersionManager::archivePathForPlatformAndArch(const std::string& version, const std::filesystem::path& baseDirectory, const std::string& platform, const std::string& arch,
                                                                        const std::string& extension, const bool sdk) {
  return baseDirectory / (sdk ? std::format("nwjs-sdk-{}-{}-{}.{}", version, platform, arch, extension) : std::format("nwjs-{}-{}-{}.{}", version, platform, arch, extension));
}
void NWJSVersionManager::queueSelectedVersionForExtraction() {
  const auto version = m_selectedVersion > -1 ? m_versions[m_selectedVersion] : m_selectedVersion == -1 ? m_versions[0] : "";
  if (version.empty()) {
    return;
  }

  const auto baseDirectory = m_configPath / kNWJS_DIR / version;
  if (!exists(baseDirectory)) {
    return;
  }

  for (const auto& platform : m_platforms) {
    for (const auto& arch : platform.architectures) {
      const auto destPath = baseDirectory / platform.identifier;
      std::filesystem::path path = archivePathForPlatformAndArch(version, baseDirectory, platform.identifier, arch.identifier, platform.extension, false);
      const auto sdkPath = archivePathForPlatformAndArch(version, baseDirectory, platform.identifier, arch.identifier, platform.extension, true);
      if (!exists(path)) {
        continue;
      }

      if (!exists(destPath)) {
        create_directories(destPath);
      }
    }
  }
}

std::filesystem::path NWJSVersionManager::nwjsPathForCurrentPlatform(const bool sdk) const {
  const auto version = m_selectedVersion > -1 ? m_versions[m_selectedVersion] : m_selectedVersion == -1 ? m_versions[0] : "";
  if (version.empty()) {
    return {};
  }

  auto out = m_configPath / kNWJS_DIR / version;

  const auto [platform, architecture] = platformSettingsForCurrentArchitectureAndOS();
  if (platform == nullptr || architecture == nullptr) {
    return {};
  }

  out = out / platform->identifier / architecture->identifier / (sdk ? "sdk" : "runtime") / platform->executablePath;

  return out;
}

std::pair<const NWJSVersionManager::Platform*, const NWJSVersionManager::Platform::Architecture*> NWJSVersionManager::platformSettingsForCurrentArchitectureAndOS() const {
  // TODO: can this be done more cleanly and with less reliance on preprocessor defines?
  // This works, but is kind of a mess for portability
#if defined(__linux__)
  const std::string platform = kLinuxIdentifier.data();
#elif defined(__APPLE__)
  const std::string platform = kMacOSIdentifier.data();
#elif defined(_WIN32)
  const std::string platform = kWindowsIdentifier.data();
#endif

#if defined(_M_X64) || defined(__x86_64__)
  const std::string arch = kX64Identifier.data();
#elif defined(_M_IX86) || defined(__i386__)
  const std::string arch = kIA32Identifier.data();
#elif defined(__aarch64__)
  const std::string arch = kARM64Identifier.data();
#endif

  for (const auto& p : m_platforms) {
    if (p.identifier != platform) {
      continue;
    }

    for (const auto& a : p.architectures) {
      if (a.identifier != arch) {
        continue;
      }

      return {&p, &a};
    }
  }
  return {nullptr, nullptr};
}

void NWJSVersionManager::detectInstalledVersions() {
  auto basePath = m_configPath / kNWJS_DIR;

  for (const auto& versionEntry : std::filesystem::directory_iterator(basePath)) {
    if (!versionEntry.is_directory()) {
      continue;
    }
    std::string version = versionEntry.path().filename().generic_string();
    if (!version.starts_with('v')) {
      continue;
    }

    for (auto& platform : m_platforms) {
      for (auto& arches = platform.availableVersions[version]; const auto& arch : platform.architectures) {
        auto& [deploymentRuntimeDownloaded, developmentRuntimeDownloaded, deploymentRuntimeExtracted, developmentRuntimeExtracted] = arches[arch.identifier];
        deploymentRuntimeDownloaded = exists(archivePathForPlatformAndArch(version, versionEntry.path(), platform.identifier, arch.identifier, platform.extension, false));
        developmentRuntimeDownloaded = exists(archivePathForPlatformAndArch(version, versionEntry.path(), platform.identifier, arch.identifier, platform.extension, true));
        const auto runtimePath = versionEntry.path() / platform.identifier / arch.identifier / "runtime" / platform.executablePath;
        deploymentRuntimeExtracted = exists(runtimePath);
        const auto sdkPath = versionEntry.path() / platform.identifier / arch.identifier / "sdk" / platform.executablePath;
        developmentRuntimeExtracted = exists(sdkPath);
        if (deploymentRuntimeDownloaded || developmentRuntimeDownloaded || deploymentRuntimeExtracted || developmentRuntimeExtracted) {
          APP_INFO("Detected NWJS version {} for platform {} and arch {}, release {}, dev {}", version, platform.name, arch.identifier, deploymentRuntimeDownloaded, developmentRuntimeDownloaded);
        }
      }
    }
  }
}
