#pragma once

#include "Database/ISettingsManager.hpp"

class SettingsManager : public ISettingsManager {
public:
  static SettingsManager& instance();

private:
  SettingsManager() = default;
};

#ifndef DEBUG
#define ORE_CHECK_DEBUG_BEGIN() if (SettingsManager::instance().getValue<bool>("enableDebugFeatures", false)) {
#else
#define ORE_CHECK_DEBUG_BEGIN()
#endif

#ifndef DEBUG
#define ORE_CHECK_DEBUG_END() }
#else
#define ORE_CHECK_DEBUG_END()
#endif

#ifndef DEBUG
#define ORE_CHECK_EXPERIMENTAL_BEGIN() if (SettingsManager::instance().getValue<bool>("enableExperimentalFeatures", false)) {
#else
#define ORE_CHECK_EXPERIMENTAL_BEGIN()
#endif

#ifndef DEBUG
#define ORE_CHECK_EXPERIMENTAL_END() }
#else
#define ORE_CHECK_EXPERIMENTAL_END()
#endif

#ifndef DEBUG
#define ORE_DISABLE_EXPERIMENTAL_BEGIN() ImGui::BeginDisabled(SettingsManager::instance().getValue<bool>("enableExperimentalFeatures", false))
#else
#define ORE_DISABLE_EXPERIMENTAL_BEGIN()
#endif

#ifndef DEBUG
#define ORE_DISABLE_EXPERIMENTAL_END() ImGui::EndDisabled()
#else
#define ORE_DISABLE_EXPERIMENTAL_END()
#endif