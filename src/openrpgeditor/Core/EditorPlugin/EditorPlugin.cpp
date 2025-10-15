#include "Core/EditorPlugin/EditorPlugin.hpp"
#include "Core/Script/ScriptEngine.hpp"

#include "angelscript.h"
#include "Core/Script/ByteCodeStream.hpp"

#include <nlohmann/json.hpp>

bool EditorPlugin::load(const std::filesystem::path& pluginBasedir, const PluginInfo& info) {
#if !ORE_DISABLE_SCRIPTING
  std::filesystem::path binFilename = info.mainScript;
  binFilename.replace_extension("plug");
  std::filesystem::path scriptFilename = info.mainScript;
  scriptFilename.replace_extension("as");

  bool saveBinary = false;
  if (info.compiled && exists(pluginBasedir / "bin" / binFilename)) {
    ByteCodeReader reader((pluginBasedir / "bin" / binFilename).generic_string());
    m_module = ScriptEngine::instance()->createModule(info.identifier, reader);
  } else {
#ifndef NDEBUG
    saveBinary = true;
#else
    saveBinary = info.compiled;
#endif
  }
  if (!m_module && exists(pluginBasedir / "src" / scriptFilename)) {
    m_module = ScriptEngine::instance()->createModule(info.identifier, pluginBasedir / "src" / scriptFilename);
    if (saveBinary && m_module) {
      ByteCodeWriter writer((pluginBasedir / "bin" / binFilename).generic_string());
      m_module->SaveByteCode(&writer, true);
    }
  }

  if (m_module) {
    m_initializeFunction = m_module->GetFunctionByDecl("void initialize()");
    m_drawFunction = m_module->GetFunctionByDecl("void draw()");
    m_shutdownFunction = m_module->GetFunctionByDecl("void shutdown()");
    return true;
  }
#endif
  return false;
}

void EditorPlugin::callInitialize() {
#if !ORE_DISABLE_SCRIPTING
  if (!m_initializeFunction) {
    return;
  }
  m_module->BindAllImportedFunctions();
  if (!m_initialized) {
    if (const auto ctx = ScriptEngine::instance()->prepareContextFromPool(m_initializeFunction)) {
      ScriptEngine::executeCall(ctx);
      ScriptEngine::instance()->returnContextToPool(ctx);
    }
    m_initialized = true;
  }
#endif
}
void EditorPlugin::callDraw() const {
#if !ORE_DISABLE_SCRIPTING
  if (!m_drawFunction || !m_initialized) {
    return;
  }
  if (const auto ctx = ScriptEngine::instance()->prepareContextFromPool(m_drawFunction)) {
    ScriptEngine::executeCall(ctx);
    ScriptEngine::instance()->returnContextToPool(ctx);
  }
#endif
}

void EditorPlugin::callShutdown() {
#if !ORE_DISABLE_SCRIPTING
  m_initialized = false;
  if (!m_shutdownFunction) {
    return;
  }
  if (const auto ctx = ScriptEngine::instance()->prepareContextFromPool(m_shutdownFunction)) {
    ScriptEngine::executeCall(ctx);
    ScriptEngine::instance()->returnContextToPool(ctx);
  }
#endif
}

void to_json(nlohmann::ordered_json& j, const EditorPlugin::PluginInfo& p) {
  j = {
      {"name", p.name},
      {"author", p.author},
      {"email", p.email},
      {"identifier", p.identifier},
      {"license", p.license},
      {"version", p.version},
      {"description", p.description},
      {"compiled", p.compiled},
      {"created", p.dateCreated},
      {"last-modified", p.lastModified},
      {"main-script", p.mainScript},
      {"dependencies", p.dependencies},
  };
}

void from_json(const nlohmann::ordered_json& j, EditorPlugin::PluginInfo& p) {
  p.name = j.value("name", p.name);
  p.author = j.value("author", p.author);
  p.email = j.value("email", p.email);
  p.identifier = j.value("identifier", p.identifier);
  p.license = j.value("license", p.license);
  p.version = j.value("version", p.version);
  p.description = j.value("description", p.description);
  p.compiled = j.value("compiled", p.compiled);
  p.dateCreated = j.value("created", p.dateCreated);
  p.lastModified = j.value("last-modified", p.lastModified);
  p.mainScript = j.value("main-script", p.mainScript);
  p.dependencies = j.value("dependencies", p.dependencies);
}
