#pragma once

#include "Database/DatabaseEditor.hpp"
#include <string_view>
#include <string>

#include "Database/Tilesets.hpp"
#include "Database/Actors.hpp"
#include "Database/MapInfos.hpp"

class Project {
public:

  static Project load(std::string_view filePath, std::string_view basePath);

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isValid() const { return m_isValid; }
  [[nodiscard]] const std::string& version() const { return m_projectVersion; }

  // Files will inform Project of that they've been modified via a callback,
  // this keeps things simple and avoids delegate overhead
  [[nodiscard]] bool modified() { return m_isModified; }

  void draw();

private:
  bool m_isValid = false;
  bool m_isLoaded = false;
  bool m_isModified = false;

  std::string m_projectVersion; // As stored in the .rpgproject file
  std::string m_projectFilePath;
  std::string m_basePath;

  Tilesets m_tilesets;
  Actors m_actors;
  MapInfos m_mapInfos;
};