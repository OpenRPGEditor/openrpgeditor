#pragma once

#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <unordered_map>
class asITypeInfo;
class asIScriptFunction;
class asIScriptModule;
class asIScriptObject;
class EditorPlugin {
public:
  struct PluginInfo {
    friend void to_json(nlohmann::ordered_json& j, const PluginInfo& p);
    friend void from_json(const nlohmann::ordered_json& j, PluginInfo& p);
    std::string name;
    std::string author;
    std::string email;
    std::string identifier;
    std::string license;
    std::string description;
    bool compiled;
    std::string dateCreated;
    std::string lastModified;
    std::string mainScript;
    std::string version;
    std::vector<std::string> dependencies;

    std::unordered_map<std::filesystem::path, std::filesystem::path> files; // absolute->relative
  };

  EditorPlugin() = default;

  bool load(const std::filesystem::path& pluginBasedir, const PluginInfo& info);

  void callInitialize();
  void callDraw() const;
  void callShutdown();

private:
  asIScriptModule* m_module = nullptr;
  asITypeInfo* m_typeInfo = nullptr;
  asIScriptObject* m_object = nullptr;
  asIScriptFunction* m_initializeFunction = nullptr;
  asIScriptFunction* m_drawFunction = nullptr;
  asIScriptFunction* m_shutdownFunction = nullptr;
  bool m_initialized = false;
};
