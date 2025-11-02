#pragma once
#include "../Editor/Log.hpp"
#include "../Editor/MapTools/IMapTool.hpp"

#include <ranges>
#include <unordered_map>

class MapToolsManager {
public:
  static MapToolsManager& instance() {
    static MapToolsManager _instance;
    return _instance;
  }

  bool registerMapTool(const std::weak_ptr<IMapTool>& mapTool) {
    const std::shared_ptr tool{mapTool.lock()};
    if (m_mapTools.contains(tool->identifier())) {
      return false;
    }

    m_mapTools[tool->identifier()] = tool;
    if (m_currentTool == nullptr) {
      m_currentTool = tool;
    }

    return true;
  }

  bool setCurrentTool(const std::string_view identifier) {
    if (!m_mapTools.contains(identifier)) {
      return false;
    }

    m_currentTool = m_mapTools[identifier];
    return true;
  }

  std::string_view currentToolId() const { return m_currentTool ? m_currentTool->identifier() : "INVALID_TOOL"; }
  std::shared_ptr<IMapTool> currentTool() { return m_mapTools[m_currentTool->identifier()]; }

  std::vector<std::shared_ptr<IMapTool>> tools() { return m_mapTools | std::views::values | std::ranges::to<std::vector>(); }

protected:
  MapToolsManager() = default;

private:
  std::unordered_map<std::string_view, std::shared_ptr<IMapTool>> m_mapTools;
  std::shared_ptr<IMapTool> m_currentTool;
};