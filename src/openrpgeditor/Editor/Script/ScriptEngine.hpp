#pragma once

#include "angelscript.h"

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class ByteCodeReader;
class asIScriptEngine;
class asIScriptFunction;
class asIScriptContext;
struct asSMessageInfo;
class asITypeInfo;
class EditorPlugin;
class ScriptEngine {
public:
  ~ScriptEngine();
  [[nodiscard]] bool hasCompileErrors() const { return m_hasCompilerErrors; }

  bool initialize();

  asIScriptEngine* engine() const { return m_engine; }
  static ScriptEngine* instance();

  asIScriptModule* createModule(std::string_view name, const std::filesystem::path& scriptPath) const;
  asIScriptModule* createModule(std::string_view name, ByteCodeReader& reader) const;
  static int executeCall(asIScriptContext* ctx);
  asIScriptContext* prepareContextFromPool(asIScriptFunction* func);
  void returnContextToPool(asIScriptContext* ctx);

private:
  void messageCallback(const asSMessageInfo& msg);

  ScriptEngine() = default;
  ScriptEngine(const ScriptEngine&) = delete;
  ScriptEngine& operator=(const ScriptEngine&) = delete;
  ScriptEngine(ScriptEngine&&) = delete;
  ScriptEngine& operator=(ScriptEngine&&) = delete;

  bool m_hasCompilerErrors{false};
  asIScriptEngine* m_engine = nullptr;

  std::vector<asIScriptContext*> m_contexts;
};