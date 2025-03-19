#pragma once

#include "Core/CommonUI/Directory.hpp"
#include "Core/DirectoryMonitor.hpp"

#include <string>
#include <vector>

namespace fs = std::filesystem;
struct Directory {
  Directory(std::string path, std::string filter, std::string selectedPath);

  std::vector<std::string>& getDirectoryContents();
  std::vector<std::string>& getDirectories();
  std::string getFileName(std::string name);

  void setSubDirectories();
  void setDirectoryContents(std::string_view filter);
  void setDirectory(int index);
  void home();
  void moveUp();
  bool isParentDirectory() const;

  std::string pathPrefix;
  std::string extFilter;

private:
  bool m_isParentDir{true};

  std::filesystem::path m_path;
  std::filesystem::path m_currentPath;

  std::vector<std::string> m_contents;
  std::vector<std::string> m_subDirectories;
};
