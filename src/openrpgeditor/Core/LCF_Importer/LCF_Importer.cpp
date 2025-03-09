#include "Core/LCF_Importer/LCF_Importer.hpp"
#include "Database/Database.hpp"
#include "LCF_Mapping.hpp"

#include <algorithm>

bool startsWithInsensitive(const std::string& str, const std::string& prefix) {
  if (str.length() < prefix.length()) {
    return false;
  }

  std::string str_lower(str.begin(), str.begin() + prefix.length());
  std::string prefix_lower(prefix);

  std::transform(str_lower.begin(), str_lower.end(), str_lower.begin(), ::tolower);
  std::transform(prefix_lower.begin(), prefix_lower.end(), prefix_lower.begin(), ::tolower);

  return std::equal(str_lower.begin(), str_lower.end(), prefix_lower.begin());
}

bool LCF_Importer::loadProject() {
  if (m_projectPath.empty()) {
    return false;
  }
  if (m_database && m_treeMap) {
    return true;
  }

  if (!exists(m_projectPath / "RPG_RT.ldb") || !exists(m_projectPath / "RPG_RT.lmt")) {
    return false;
  }

  m_database = lcf::LDB_Reader::Load((m_projectPath / "RPG_RT.ldb").generic_string(), "SJIS");
  m_treeMap = lcf::LMT_Reader::Load((m_projectPath / "RPG_RT.lmt").generic_string(), "SJIS");

  if (!std::filesystem::exists(Database::instance()->basePath + "editor/LCFMapping.json")) {
    m_mapper.serialize(Database::instance()->basePath + "editor/LCFMapping.json"); // Make new file if it doesn't exist
  } else {
    m_mapper.load(Database::instance()->basePath + "editor/LCFMapping.json");
  }

  return m_database && m_treeMap;
}

std::unique_ptr<lcf::rpg::Map> LCF_Importer::loadMap(const int id) const {
  std::string mapName = std::format("Map{:04}.lmu", id);
  if (exists(m_projectPath / mapName)) {
    return lcf::LMU_Reader::Load((m_projectPath / mapName).generic_string().c_str(), "SJIS");
  }
  return nullptr;
}
