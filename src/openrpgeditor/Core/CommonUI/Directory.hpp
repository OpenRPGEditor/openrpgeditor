#pragma once

#include "Core/CommonUI/Directory.hpp"
#include "Core/DirectoryMonitor.hpp"

#include <string>
#include <vector>

namespace fs = std::filesystem;
struct Directory {
  Directory(std::string_view path, std::string_view filter, std::string_view selectedPath);

  std::vector<std::string>& getDirectoryContents();
  std::vector<std::string>& getDirectories();
  std::string getFileName(std::string name);

  std::string& getPathPrefix();
  const std::string& getPathPrefix() const;

  std::string& getExt();
  const std::string& getExt() const;

  void setSubDirectories();
  void setDirectoryContents(std::string_view filter);
  void setDirectory(int index);

  void home();
  void moveUp();
  bool isParentDirectory() const;

private:
  bool m_isParentDir{true};

  std::filesystem::path m_path;
  std::filesystem::path m_currentPath;

  std::string m_pathPrefix;
  std::string m_extFilter;

  std::vector<std::string> m_contents;
  std::vector<std::string> m_subDirectories;
};
