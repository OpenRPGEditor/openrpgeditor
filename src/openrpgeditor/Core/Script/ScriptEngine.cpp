#include "Core/Script/ScriptEngine.hpp"

#include "Bindings.hpp"
#include "Core/Log.hpp"
#include "Core/Script/ByteCodeStream.hpp"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include <bits/fs_ops.h>
#include <bits/fs_path.h>
#include <cassert>
#include <cstdarg>
#include <iostream>
#include <ranges>
#include <scriptarray/scriptarray.h>
#include <scripthandle/scripthandle.h>
#include <scriptmath/scriptmath.h>
#include <scriptmath/scriptmathcomplex.h>
#include <scriptstdstring/scriptstdstring.h>
#include <weakref/weakref.h>
ScriptEngine::~ScriptEngine() {
  for (const auto& context : m_contexts) {
    (void)context->Release();
  }

  if (m_engine) {
    m_engine->ShutDownAndRelease();
  }
}

std::vector<std::string> splitByDoubleColons(const std::string& str) {
  std::vector<std::string> result;
  std::string current;

  for (char c : str) {
    if (c == ':' && current.back() == ':') {
      result.push_back(current.substr(0, current.size() - 1)); // Remove last ':'
      current.clear();
    } else {
      current += c;
    }
  }

  if (!current.empty()) {
    result.push_back(current);
  }

  return result;
}

void PrintString(int level, std::string fmt, ...) {
  va_list args;
  int size = 100;
  std::string str;

  while (true) {
    str.resize(size);
    va_start(args, fmt);
    int n = vsnprintf(const_cast<char*>(str.c_str()), size, fmt.c_str(), args);
    va_end(args);
    if (n > -1 && n < size) {
      str.resize(n);
      break;
    }
    if (n > -1) {
      size = n + 1;
    } else {
      size *= 2;
    }
  }

  APP_INFO(str);
}
bool ScriptEngine::initialize() {
  int r;

  m_engine = asCreateScriptEngine();
  r = m_engine->SetMessageCallback(asMETHOD(ScriptEngine, messageCallback), this, asCALL_THISCALL);
  assert(r >= 0);

  RegisterScriptArray(m_engine, true);
  RegisterStdString(m_engine);
  RegisterStdStringUtils(m_engine);
  RegisterScriptHandle(m_engine);
  RegisterScriptWeakRef(m_engine);
  RegisterScriptMath(m_engine);
  RegisterScriptMathComplex(m_engine);

  r = m_engine->RegisterEnum("LogLevel");
  assert(r >= 0);
  r = m_engine->RegisterEnumValue("LogLevel", "Message", 0);
  assert(r >= 0);
  r = m_engine->RegisterEnumValue("LogLevel", "Info", 1);
  assert(r >= 0);
  r = m_engine->RegisterEnumValue("LogLevel", "Warning", 2);
  assert(r >= 0);
  r = m_engine->RegisterEnumValue("LogLevel", "Error", 3);
  assert(r >= 0);
  r = m_engine->RegisterEnumValue("LogLevel", "Fatal", 4);
  assert(r >= 0);

  r = m_engine->RegisterGlobalFunction("void print(LogLevel level, const string& in)", asFUNCTION(PrintString), asCALL_CDECL);
  assert(r >= 0);
  RegisterBindings();

  int processedCount = 0;
  std::vector<const char*> processedNamespaces;

  auto defaultNs = m_engine->GetDefaultNamespace();
  m_engine->SetDefaultNamespace("Test1::Test2::Test3");
  r = m_engine->RegisterGlobalFunction("void print(LogLevel level, const string& in)", asFUNCTION(PrintString), asCALL_CDECL);
  m_engine->SetDefaultNamespace(defaultNs);
  while (processedCount < m_engine->GetGlobalFunctionCount()) {
    const char* ns = nullptr;
    int depth = 0;
    for (int i = 0; i < m_engine->GetGlobalFunctionCount(); i++) {
      auto func = m_engine->GetGlobalFunctionByIndex(i);
      if (func->GetNamespace() != ns && ns != nullptr) {
        continue;
      }

      if (ns == nullptr) {
        const auto thisns = func->GetNamespace();
        if (thisns != nullptr) {
          if (std::ranges::find(processedNamespaces, thisns) != processedNamespaces.end()) {
            continue;
          }
          ns = thisns;
          if (strlen(ns) > 0) {
            const auto spaces = splitByDoubleColons(ns);
            for (const auto& space : spaces) {
              std::cout << std::string(depth * 2, ' ') + "namespace " + space + " {" << std::endl;
              depth++;
            }
          }
        }
      }

      std::cout << std::string(depth * 2, ' ') + func->GetDeclaration(true, false, true) << std::endl;
      ++processedCount;
    }

    if (ns != nullptr) {
      if (strlen(ns) > 0) {
        const auto spaces = splitByDoubleColons(ns);
        for (const auto& space : spaces | std::views::reverse) {
          depth--;
          std::cout << std::string(depth * 2, ' ') + "} // " + space << std::endl;
        }
      }
      processedNamespaces.push_back(ns);
    }
  }
  return r >= 0;
}

asIScriptModule* ScriptEngine::createModule(const std::string_view name, const std::filesystem::path& scriptPath) const {

  if (asIScriptModule* mod = m_engine->GetModule(name.data(), asGM_ONLY_IF_EXISTS)) {
    return mod;
  }

  CScriptBuilder builder;
  int ret = builder.StartNewModule(m_engine, name.data());
  if (ret < 0) {
    return nullptr;
  }

  if (!exists(scriptPath)) {
    return nullptr;
  }

  ret = builder.AddSectionFromFile(scriptPath.generic_string().c_str());
  if (ret < 0) {
    return nullptr;
  }

  ret = builder.BuildModule();

  if (ret < 0) {
    return nullptr;
  }

  return m_engine->GetModule(name.data(), asGM_ONLY_IF_EXISTS);
}

asIScriptModule* ScriptEngine::createModule(const std::string_view name, ByteCodeReader& reader) const {

  if (asIScriptModule* mod = m_engine->GetModule(name.data(), asGM_ONLY_IF_EXISTS)) {
    return mod;
  }

  asIScriptModule* mod = m_engine->GetModule(name.data(), asGM_CREATE_IF_NOT_EXISTS);
  mod->LoadByteCode(&reader);
  return mod;
}

void ScriptEngine::messageCallback(const asSMessageInfo& msg) {
  if (msg.type == asMSGTYPE_WARNING) {
    APP_WARN("{} ({}, {}) : {}", msg.section, msg.row, msg.col, msg.message);
  } else if (msg.type == asMSGTYPE_ERROR) {
    APP_ERROR("{} ({}, {}) : {}", msg.section, msg.row, msg.col, msg.message);
  } else if (msg.type == asMSGTYPE_INFORMATION) {
    APP_INFO("{} ({}, {}) : {}", msg.section, msg.row, msg.col, msg.message);
  }

  if (msg.type == asMSGTYPE_ERROR) {
    m_hasCompilerErrors = true;
  }
}

asIScriptContext* ScriptEngine::prepareContextFromPool(asIScriptFunction* func) {
  asIScriptContext* ctx = nullptr;
  if (!m_contexts.empty()) {
    ctx = *m_contexts.rbegin();
    m_contexts.pop_back();
  } else {
    ctx = m_engine->CreateContext();
  }

  int ret = ctx->Prepare(func);
  assert(ret >= 0);
  return ctx;
}

int ScriptEngine::executeCall(asIScriptContext* ctx) {
  int r = ctx->Execute();
  if (r != asEXECUTION_FINISHED) {
    if (r == asEXECUTION_EXCEPTION) {
      APP_ERROR("Exception: {}", ctx->GetExceptionString());
      APP_ERROR("Function: {} Line {}", ctx->GetExceptionFunction()->GetDeclaration(), ctx->GetExceptionLineNumber());
    }
  }

  return r;
}

void ScriptEngine::returnContextToPool(asIScriptContext* ctx) {
  m_contexts.push_back(ctx);
  ctx->Unprepare();
}

ScriptEngine* ScriptEngine::instance() {
  static ScriptEngine instance;
  return &instance;
}
