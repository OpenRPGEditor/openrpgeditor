#include "Directory.hpp"

#include "Core/CommonUI/ImagePicker.hpp"
#include "Database/Database.hpp"

Directory::Directory(std::string path, std::string filter, std::string selectedPath) {
  m_path = Database::instance()->basePath + path;
  if (!selectedPath.empty()) {
    // Navigate to contents
    selectedPath = selectedPath.substr(0, selectedPath.find_last_of('/'));
    m_currentPath.assign(Database::instance()->basePath + path + "/" + selectedPath);
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
    extFilter = filter;
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
  pathPrefix += pathPrefix.empty() ? m_subDirectories[index] : '/' + m_subDirectories[index];
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(extFilter);
  setSubDirectories();
}
void Directory::home() {
  m_currentPath = m_path;
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(extFilter);
  setSubDirectories();
}
void Directory::moveUp() {
  if (pathPrefix.find('/') == std::string::npos) {
    pathPrefix.clear();
  } else {
    pathPrefix = pathPrefix.substr(0, pathPrefix.find_last_of('/'));
  }
  m_currentPath = m_currentPath.parent_path();
  m_isParentDir = equivalent(m_path, m_currentPath);
  setDirectoryContents(extFilter);
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
std::vector<std::string>& Directory::getDirectoryContents() { return m_contents; }
