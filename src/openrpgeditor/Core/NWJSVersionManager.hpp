#pragma once

#include "DownloadManager.hpp"

#include <string>
#include <vector>

class NWJSVersionManager {
public:
  explicit NWJSVersionManager(std::string_view nwjsPath);

  bool getListing();

  bool downloadSelectedVersion();

  void draw();

  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

private:
  DownloadManager m_downloadManager;

  int m_versionDownloadHandle = -1;
  std::string m_NWJSPath;
  std::vector<std::string> m_versions;
  int m_selectedVersion = -1;
  bool m_downloadWindows
#ifdef _WIN32
      {true};
#else
      {false};
#endif

  bool m_downloadMacOS
#ifdef __APPLE__
      {true};
#else
      {false};
#endif

  bool m_downloadLinux
#ifdef __linux__
      {true};
#else
      {false};
#endif

  bool m_downloadSDK{true};

  bool m_isOpen = false;
};
