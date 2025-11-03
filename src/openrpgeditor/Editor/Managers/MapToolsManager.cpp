#include "MapToolsManager.hpp"

#include "Editor/MapTools/PenMapTool.hpp"

MapToolsManager::MapToolsManager() {
   registerMapTool(std::make_shared<PenMapTool>());
 }
