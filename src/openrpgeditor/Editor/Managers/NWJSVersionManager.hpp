#pragma once

#include "../../../../cmake-build-release/_deps/sdl3-src/src/video/khronos/vulkan/vulkan_core.h"
#include "../Editor/CommonUI/IDialogController.hpp"
#include "../Editor/Settings.hpp"
#include "DownloadManager.hpp"

#include <string>
#include <vector>

struct archive;
struct archive_entry;
class NWJSVersionManager final {
public:
  static constexpr std::string_view kNWJS_URL = "https://dl.nwjs.io";
  static constexpr std::string_view kNWJS_DIR = "nwjs_versions";

  void draw();

  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

  static NWJSVersionManager& instance() {
    static NWJSVersionManager instance;
    return instance;
  }

  struct Platform {
    std::string name;
    std::string identifier;
    std::string extension;
    std::string executablePath;
    struct Architecture {
      std::string name;
      std::string identifier;
    };
    std::vector<Architecture> architectures;
    struct ArchAvailability {
      bool deploymentRuntimeDownloaded{false};
      bool developmentRuntimeDownloaded{false};
      bool deploymentRuntimeExtracted{false};
      bool developmentRuntimeExtracted{false};
    };
    std::map<std::string, std::unordered_map<std::string, ArchAvailability>> availableVersions;
  };

  [[nodiscard]] std::filesystem::path nwjsPathForCurrentPlatform(bool sdk = true) const;
  void initialize();

  [[nodiscard]] bool isFullyInitialized() const { return m_selectedVersion != -3; }

  [[nodiscard]] bool nwjsForCurrentPlatformAvailable() const { return exists(nwjsPathForCurrentPlatform()); }

private:
  NWJSVersionManager();

  [[nodiscard]] std::pair<const Platform*, const Platform::Architecture*> platformSettingsForCurrentArchitectureAndOS() const;
  bool getListing();
  void addDownload(std::string_view version, std::string_view platform, std::string_view architecture, std::string_view extension, bool sdk) const;
  static std::filesystem::path archivePathForPlatformAndArch(const std::string& version, const std::filesystem::path& baseDirectory, const std::string& platform, const std::string& arch,
                                                      const std::string& extension, bool sdk);
  void queueSelectedVersionForExtraction();
  void detectInstalledVersions();

  int m_versionDownloadHandle = -1;
  std::vector<std::string> m_versions{};
  std::filesystem::path m_configPath{};
  int m_selectedVersion = -3; // -3 not initialized, -2 None, -1 Latest
  bool m_isOpen{false};

  std::vector<Platform> m_platforms;
};
