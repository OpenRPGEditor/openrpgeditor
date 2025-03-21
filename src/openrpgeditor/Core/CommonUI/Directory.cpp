#include "Directory.hpp"

#include "Core/CommonUI/ImagePicker.hpp"
#include "Database/Database.hpp"

Directory::Directory(std::string_view path, std::string_view filter, std::string_view selectedPath) {
  m_path = Database::instance()->basePath + path.data();
  if (!selectedPath.empty()) {
    // Navigate to contents
    selectedPath = selectedPath.substr(0, selectedPath.find_last_of('/'));
    if (!path.ends_with('/')) {
      m_currentPath.assign(Database::instance()->basePath + path.data() + "/" + selectedPath.data());
    } else {
      m_currentPath.assign(Database::instance()->basePath + path.data() + selectedPath.data());
    }
    if (!filter.empty() && !is_directory(m_currentPath)) {
      m_currentPath.replace_extension(filter);
    }
    if (is_regular_file(m_currentPath)) {
      m_currentPath = m_currentPath.parent_path();
    }
  } else {
    m_currentPath.assign(m_path);
  }

  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(filter);
  setSubDirectories();

  // TODO: *nix path handling
}

void Directory::setDirectoryContents(std::string_view filter) {
  m_contents.clear();
  for (const auto& entry : fs::directory_iterator(m_currentPath)) {
    if (!filter.empty() && entry.path().extension().compare(filter) != 0) {
      continue;
    }

    auto filename = entry.path().filename();
    filename = filename.replace_extension();
    const auto path = filename.generic_string();
    m_extFilter = filter;
    m_contents.emplace_back(path);
  }
}
void Directory::setSubDirectories() {
  m_subDirectories.clear();
  for (const auto& entry : fs::directory_iterator(m_currentPath)) {
    if (!entry.is_directory()) {
      continue;
    }
    const auto path = entry.path().filename().generic_string();
    m_subDirectories.emplace_back(path);
  }
}
void Directory::setDirectory(int index) {
  m_currentPath.append(m_subDirectories[index]);
  m_pathPrefix += m_pathPrefix.empty() ? m_subDirectories[index] : '/' + m_subDirectories[index];
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(m_extFilter);
  setSubDirectories();
}
void Directory::home() {
  m_currentPath = m_path;
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(m_extFilter);
  setSubDirectories();
}
void Directory::moveUp() {
  if (m_pathPrefix.find('/') == std::string::npos) {
    m_pathPrefix.clear();
  } else {
    m_pathPrefix = m_pathPrefix.substr(0, m_pathPrefix.find_last_of('/'));
  }
  m_currentPath = m_currentPath.parent_path();
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(m_extFilter);
  setSubDirectories();
}
bool Directory::isParentDirectory() const { return m_isParentDir; }

std::vector<std::string>& Directory::getDirectories() { return m_subDirectories; }
std::string Directory::getFileName(std::string name) {
  if (name.find('/') != std::string::npos) {
    name = name.substr(name.find_last_of('/') + 1, name.size());
  }
  return name;
}

std::string& Directory::getPathPrefix() { return m_pathPrefix; }
const std::string& Directory::getPathPrefix() const { return m_pathPrefix; }

std::string& Directory::getExt() { return m_extFilter; }
const std::string& Directory::getExt() const { return m_extFilter; }

std::vector<std::string>& Directory::getDirectoryContents() { return m_contents; }
