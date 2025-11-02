#include "Directory.hpp"

#include "Database/Database.hpp"
#include "Editor/CommonUI/ImagePicker.hpp"

Directory::Directory(const std::string_view path, const std::string_view filter, const std::string_view selectedPath) {
  m_path = std::filesystem::path(Database::instance()->basePath) / path;
  if (!selectedPath.empty()) {
    m_currentPath = m_path / selectedPath;
    if (!filter.empty() && !is_directory(m_currentPath)) {
      m_currentPath.replace_extension(filter);
    }
  } else {
    m_currentPath = m_path;
  }

  /* Find the first valid directory */
  while (!is_directory(m_currentPath) && !m_currentPath.empty()) {
    m_currentPath = m_currentPath.parent_path();
  }

  try {
    m_isParentDir = equivalent(m_path, m_currentPath);
  } catch (const std::exception&) {
    /* If we encounter an exception assume something crazy happened and use the specified path as the current path */
    m_isParentDir = false;
    m_currentPath = m_path;
  }
  setDirectoryContents(filter);
  setSubDirectories();
}

void Directory::setDirectoryContents(const std::string_view filter) {
  m_contents.clear();
  if (!is_directory(m_currentPath)) {
    return;
  }
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
  if (!is_directory(m_currentPath)) {
    return;
  }
  for (const auto& entry : fs::directory_iterator(m_currentPath)) {
    if (!entry.is_directory()) {
      continue;
    }
    const auto path = entry.path().filename().generic_string();
    m_subDirectories.emplace_back(path);
  }
}
void Directory::setDirectory(const int index) {
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
std::string Directory::getFileName(const std::string& name) { return std::filesystem::path(name).filename().generic_string(); }

std::string& Directory::getPathPrefix() { return m_pathPrefix; }
const std::string& Directory::getPathPrefix() const { return m_pathPrefix; }

std::string& Directory::getExt() { return m_extFilter; }
const std::string& Directory::getExt() const { return m_extFilter; }

std::vector<std::string>& Directory::getDirectoryContents() { return m_contents; }
