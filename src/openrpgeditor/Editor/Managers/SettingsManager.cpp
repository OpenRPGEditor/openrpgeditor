#include "Editor/Managers/SettingsManager.hpp"


SettingsManager& SettingsManager::instance() {
  static SettingsManager instance;
  return instance;
}