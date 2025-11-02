#pragma once

#include "LCF_Mapping.hpp"

#include <filesystem>
#include <lcf/ldb/reader.h>
#include <lcf/lmt/reader.h>
#include <lcf/lmu/reader.h>
#include <map>
#include <memory>

class LCF_Importer {
public:
  explicit LCF_Importer() = default;

  bool loadProject();

  const lcf::rpg::Database* database() const { return m_database.get(); }
  const lcf::rpg::TreeMap* treeMap() const { return m_treeMap.get(); }
  std::unique_ptr<lcf::rpg::Map> loadMap(int id) const;

  void setProject(const std::filesystem::path& projectPath) { m_projectPath = projectPath; }

  LCF_Mapping* mapper() { return &m_mapper; }

private:
  std::filesystem::path m_projectPath;
  std::unique_ptr<lcf::rpg::Database> m_database;
  std::unique_ptr<lcf::rpg::TreeMap> m_treeMap;
  LCF_Mapping m_mapper;
};
