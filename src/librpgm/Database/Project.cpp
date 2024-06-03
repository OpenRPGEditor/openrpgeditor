#include "Project.hpp"

#include "imgui.h"

#include <fstream>

#include <array>
#include <string_view>
#include <algorithm>

using namespace std::literals::string_view_literals;

constexpr std::array KnownRPGMVVersions = {
  "RPGMV 1.6.1"sv,
  "RPGMV 1.6.2"sv,
};

Project Project::load(std::string_view filePath, std::string_view basePath) {
  std::string version;
  std::ifstream file(filePath.data());
  file >> version;

  auto it = std::find(KnownRPGMVVersions.begin(), KnownRPGMVVersions.end(), version);

  Project ret;
  ret.m_isValid = it != KnownRPGMVVersions.end();
  ret.m_projectVersion = version;
  ret.m_basePath = basePath;
  ret.m_projectFilePath = filePath;
  ret.m_tilesets = Tilesets::load(ret.m_basePath + "/data/Tilesets.json");
  ret.m_actors = Actors::load(ret.m_basePath + "/data/Actors.json");
  return Project{};
}


void Project::draw() {
}