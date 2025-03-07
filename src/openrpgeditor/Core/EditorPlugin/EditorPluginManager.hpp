#pragma once
#include "Core/DirectoryMonitor.hpp"
#include "Core/EditorPlugin/EditorPlugin.hpp"

#include "angelscript.h"

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <unordered_map>

class EditorPluginManager {
public:
  static EditorPluginManager* instance();

  const std::unordered_map<std::filesystem::path, EditorPlugin::PluginInfo>& plugins() const { return m_plugins; }

  bool initialize();

  bool pluginLoaded(const std::string& identifier);
  void initializeAllPlugins();
  void initializePluginById(const std::string_view identifier);
  void shutdownAllPlugins();
  void shutdownPluginById(const std::string_view identifier, bool dependencies = false);
  void callDraws();

  void draw();

  void setOpen(const bool open = true) { m_open = open; }

private:
  EditorPluginManager();
  EditorPluginManager(const EditorPluginManager&) = delete;
  EditorPluginManager& operator=(const EditorPluginManager&) = delete;
  EditorPluginManager(EditorPluginManager&&) = delete;
  EditorPluginManager& operator=(EditorPluginManager&&) = delete;

  DirectoryMonitor m_directoryMonitor;
  std::unordered_map<std::filesystem::path, EditorPlugin::PluginInfo> m_plugins;
  std::unordered_map<std::string, EditorPlugin::PluginInfo*> m_pluginInfoIdentifierMapping; // identifier->plugin
  std::unordered_map<std::filesystem::path, EditorPlugin> m_pluginInstances;
  std::unordered_map<std::string, EditorPlugin*> m_pluginInstanceIdentifierMapping;
  std::filesystem::path m_pluginBaseDir;
  void onDirectoryAdded(const std::filesystem::path& path);
  void onDirectoryModified(const std::filesystem::path& path);
  void onDirectoryDeleted(const std::filesystem::path& path);
  void onFileAdded(const std::filesystem::path& path);
  void registerPluginFiles(const std::filesystem::path& path, EditorPlugin::PluginInfo& plugin);
  void onFileModified(const std::filesystem::path& path);
  void onFileDeleted(const std::filesystem::path& path);

  bool m_open = false;
  std::string m_selectedIdentifier;
};
