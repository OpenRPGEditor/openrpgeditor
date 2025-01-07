#include "Core/EditorPlugin/EditorPluginManager.hpp"

#include "Core/Application.hpp"
#include "Core/Script/ScriptEngine.hpp"

#include <iostream>

static const std::vector<std::filesystem::path> kFileBlacklist = {
    ".gitkeep",
    "meta.json",
};

EditorPluginManager* EditorPluginManager::instance() {
  static EditorPluginManager pluginManager;
  return &pluginManager;
}

EditorPluginManager::EditorPluginManager() {
  m_pluginBaseDir = std::filesystem::path(App::APP->userConfigPath()) / "plugins";
  m_directoryMonitor.setWatchDirectory(m_pluginBaseDir.generic_string());
  m_directoryMonitor.directoryAdded().connect<&EditorPluginManager::onDirectoryAdded>(this);
  m_directoryMonitor.directoryChanged().connect<&EditorPluginManager::onDirectoryModified>(this);
  m_directoryMonitor.directoryDeleted().connect<&EditorPluginManager::onDirectoryDeleted>(this);
  m_directoryMonitor.fileAdded().connect<&EditorPluginManager::onFileAdded>(this);
  m_directoryMonitor.fileChanged().connect<&EditorPluginManager::onFileModified>(this);
  m_directoryMonitor.fileDeleted().connect<&EditorPluginManager::onFileDeleted>(this);
  m_directoryMonitor.start();
  // populatePlugins();
}

static bool PluginLoaded(const std::string& identifier) { return EditorPluginManager::instance()->pluginLoaded(identifier); }

void EditorPluginManager::draw() {
  if (!m_open) {
    return;
  }

  ImGui::SetNextWindowSizeConstraints({640.f, 640.f}, {FLT_MAX, FLT_MAX});
  if (ImGui::Begin("Plugin Manager", &m_open)) {
    if (ImGui::BeginListBox("##pluginList",
                            ImVec2(std::max(240.f, ImGui::GetContentRegionAvail().x / 3), ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y))) {
      for (const auto& plugin : m_plugins | std::views::values) {
        ImGui::Selectable(std::format("{} - {}", plugin.identifier, plugin.name).c_str());
      }
      ImGui::EndListBox();
    }
  }
  ImGui::End();
}
bool EditorPluginManager::pluginLoaded(const std::string& identifier) {
  auto it = m_plugins.begin();
  for (; it != m_plugins.end(); ++it) {
    if (it->second.identifier == identifier) {
      break;
    }
  }
  if (it != m_plugins.end()) {
    return m_pluginInstances.contains(it->first);
  }
  return false;
}
bool EditorPluginManager::initialize() {
  int ret = ScriptEngine::instance()->engine()->RegisterGlobalFunction("bool pluginAvailable(const string& in)", asFUNCTION(PluginLoaded), asCALL_CDECL);
  return ret >= asSUCCESS;
}

void EditorPluginManager::initializeAllPlugins() {
  for (auto& plugin : m_pluginInstances | std::views::values) {
    plugin.callInitialize();
  }
}

void EditorPluginManager::initializePluginById(const std::string_view identifier) {
  if (m_pluginInstanceIdentifierMapping.contains(identifier.data())) {
    m_pluginInstanceIdentifierMapping[identifier.data()]->callInitialize();
  }
}

void EditorPluginManager::callDraws() {
  for (auto& plugin : m_pluginInstances | std::views::values) {
    plugin.callDraw();
  }
}

void EditorPluginManager::shutdownAllPlugins() {
  for (auto& plugin : m_pluginInstances | std::views::values) {
    plugin.callShutdown();
  }
}

void EditorPluginManager::shutdownPluginById(const std::string_view identifier, bool dependencies) {
  if (m_pluginInstanceIdentifierMapping.contains(identifier.data())) {
    auto plugin = m_pluginInstanceIdentifierMapping[identifier.data()];
    /* Remove ourselves from the list so we don't have an infinite loop in case of cyclical dependencies */
    m_pluginInstanceIdentifierMapping.erase(identifier.data());
    if (dependencies) {
      for (const auto& dependency : m_pluginInfoIdentifierMapping[identifier.data()]->dependencies) {
        shutdownPluginById(dependency);
      }
    }
    plugin->callShutdown();
  }
}

void EditorPluginManager::onDirectoryAdded(const std::filesystem::path& path) {
  if (!is_regular_file(path / "meta.json")) {
    return;
  }

  try {
    std::ifstream jsonFile(path / "meta.json");
    nlohmann::ordered_json json = nlohmann::ordered_json::parse(jsonFile);
    EditorPlugin::PluginInfo plugin;
    json.get_to(plugin);

    if (!m_plugins.contains(plugin.identifier)) {
      m_plugins[path] = plugin;
      m_pluginInfoIdentifierMapping[plugin.identifier] = &m_plugins[path];
      Settings::instance()->plugins[plugin.identifier].compiled = plugin.compiled;
      APP_INFO("Added plugin: {}", plugin.name);
      APP_INFO("Path: {}", path.string());
      APP_INFO("Meta json: {}", json.dump());
      registerPluginFiles(path, m_plugins[path]);
      // if (Settings::instance()->plugins[plugin.identifier].enabled)
      {
        m_pluginInstances[path] = EditorPlugin();
        if (!m_pluginInstances[path].load(path, plugin)) {
          APP_ERROR("Failed to load plugin: {}", plugin.identifier);
        }
      }
    } else {
      APP_ERROR("Duplicate plugin identifier: {}", plugin.identifier);
    }
  } catch (std::exception& e) { APP_ERROR("Could not parse meta.json"); }
}

void EditorPluginManager::onDirectoryModified(const std::filesystem::path& path) {}

void EditorPluginManager::onDirectoryDeleted(const std::filesystem::path& path) {}

void EditorPluginManager::onFileAdded(const std::filesystem::path& path) {}

void EditorPluginManager::registerPluginFiles(const std::filesystem::path& path, EditorPlugin::PluginInfo& plugin) {
  for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
    const auto& rel = relative(entry.path(), path);
    if (entry.is_regular_file() && std::ranges::find(kFileBlacklist, rel.filename()) == kFileBlacklist.end()) {
      APP_INFO("Registering plugin file: {}", rel.generic_string());
      plugin.files[entry.path()] = rel;
    }
  }
}

void EditorPluginManager::onFileModified(const std::filesystem::path& path) {
  if (path.filename() == "meta.json") {
    try {
      std::ifstream jsonFile(path);
      nlohmann::ordered_json json = nlohmann::ordered_json::parse(jsonFile);
      EditorPlugin::PluginInfo plugin;
      json.get_to(plugin);
      if (m_plugins.contains(path.parent_path())) {
        auto& entry = m_plugins[path.parent_path()];
        const std::string oldIdentifier = entry.identifier;
        entry = plugin;
        if (plugin.identifier != oldIdentifier) {
          if (!m_pluginInfoIdentifierMapping.contains(plugin.identifier)) {
            m_pluginInfoIdentifierMapping.erase(oldIdentifier);
            m_pluginInfoIdentifierMapping[entry.identifier] = &entry;
            Settings::instance()->plugins[entry.identifier].compiled = entry.compiled;
            Settings::instance()->plugins[entry.identifier].enabled = Settings::instance()->plugins[oldIdentifier].enabled;
            Settings::instance()->plugins.erase(oldIdentifier);
            APP_INFO("Added plugin changed identifier: {} {} {} -> {}", path.parent_path().generic_string(), plugin.name, oldIdentifier, plugin.identifier);
          } else if (m_pluginInfoIdentifierMapping.contains(plugin.identifier)) {
            entry.identifier = oldIdentifier;
            Settings::instance()->plugins[entry.identifier].compiled = entry.compiled;
            APP_ERROR("Duplicate plugin meta detected on change! Keeping old identifier");
          }
        } else {
          Settings::instance()->plugins[entry.identifier].compiled = entry.compiled;
        }

        registerPluginFiles(path.parent_path(), entry);
        std::cout << entry.identifier << " modified!" << std::endl;
      }

    } catch (const std::exception& e) { APP_ERROR("Could not parse meta.json"); }
  }
}

void EditorPluginManager::onFileDeleted(const std::filesystem::path& path) {}