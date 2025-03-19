#pragma once

#include "Core/CommonUI/Directory.hpp"
#include "Core/DirectoryMonitor.hpp"

#include <string>
#include <vector>

namespace fs = std::filesystem;
struct Directory {
  Directory(std::string path, std::string filter);

  std::vector<std::string>& getDirectoryContents();
  std::vector<std::string>& getDirectories();

  void setSubDirectories();
  void setDirectoryContents(std::string_view filter);
  void setDirectory(int index);
  void home();
  void moveUp();
  bool isParentDirectory() const;
  std::string pathPrefix;

private:
  bool isParentDir;

  std::string m_filter;

  std::filesystem::path m_path;
  std::filesystem::path m_currentPath;

  std::vector<std::string> m_contents;
  std::vector<std::string> m_subDirectories;
};
