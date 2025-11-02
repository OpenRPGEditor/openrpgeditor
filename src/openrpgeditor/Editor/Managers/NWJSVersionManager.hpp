#pragma once

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

  // TODO: create proper settings manager and serialize these
  struct Platform {
    std::string name;
    std::string identifier;
    std::string extension;
    std::string executablePath;
    struct Architecture {
      std::string name;
      std::string identifier;
      bool download;
      bool downloadSDK;
    };
    std::vector<Architecture> architectures;
  };

  std::filesystem::path nwjsPathForCurrentPlatform(bool sdk = true) const;
  void initialize();

  bool isFullyInitialized() const { return m_selectedVersion != -3; }

  bool nwjsForCurrentPlatformAvailable() const { return exists(nwjsPathForCurrentPlatform()); }

private:
  std::pair<const Platform*, const Platform::Architecture*> platformSettingsForCurrentArchitectureAndOS() const;
  bool getListing();
  void addDownload(std::string_view version, std::string_view platform, std::string_view architecture, std::string_view extension, bool sdk) const;
  void addDownloads() const;
  void queueSelectedVersionForExtraction();
  NWJSVersionManager();
  int m_versionDownloadHandle = -1;
  std::vector<std::string> m_versions{};
  std::filesystem::path m_configPath{};
  int m_selectedVersion = -3; // -3 not initialized, -2 None, -1 Latest
  bool m_isOpen{false};

  std::vector<Platform> m_platforms;
  std::vector<archive*> m_queuedArchives;
};
