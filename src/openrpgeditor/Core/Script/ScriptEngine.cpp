
#include "Core/Script/ScriptEngine.hpp"

#include "Bindings.hpp"
#include "Core/Log.hpp"
#include "Core/Script/ByteCodeStream.hpp"
#include "scriptdictionary/scriptdictionary.h"

#include <angelscript.h>
#include <scriptbuilder/scriptbuilder.h>

#include <cassert>
#include <cstdarg>
#include <iostream>
#include <ranges>
#include <scriptarray/scriptarray.h>
#include <scripthandle/scripthandle.h>
#include <scriptjson/scriptjson.h>
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

namespace {
template <class Stream>
void printEnumList(const asIScriptEngine* engine, Stream& stream) {
  for (int i = 0; i < engine->GetEnumCount(); i++) {
    const auto e = engine->GetEnumByIndex(i);
    if (not e)
      continue;
    const std::string_view ns = e->GetNamespace();
    if (not ns.empty())
      stream << std::format("namespace {} {{\n", ns);
    stream << std::format("enum {} {{\n", e->GetName());
    for (int j = 0; j < e->GetEnumValueCount(); ++j) {
      stream << std::format("\t{}", e->GetEnumValueByIndex(j, nullptr));
      if (j < e->GetEnumValueCount() - 1)
        stream << ",";
      stream << "\n";
    }
    stream << "}\n";
    if (not ns.empty())
      stream << "}\n";
  }
}

template <class Stream>
void printClassTypeList(const asIScriptEngine* engine, Stream& stream) {
  for (int i = 0; i < engine->GetObjectTypeCount(); i++) {
    const auto t = engine->GetObjectTypeByIndex(i);
    if (not t)
      continue;

    const std::string_view ns = t->GetNamespace();
    if (not ns.empty())
      stream << std::format("namespace {} {{\n", ns);

    stream << std::format("class {}", t->GetName());
    if (t->GetSubTypeCount() > 0) {
      stream << "<";
      for (int sub = 0; sub < t->GetSubTypeCount(); ++sub) {
        if (sub < t->GetSubTypeCount() - 1)
          stream << ", ";
        const auto st = t->GetSubType(sub);
        stream << st->GetName();
      }

      stream << ">";
    }

    stream << "{\n";
    for (int j = 0; j < t->GetBehaviourCount(); ++j) {
      asEBehaviours behaviours;
      const auto f = t->GetBehaviourByIndex(j, &behaviours);
      if (behaviours == asBEHAVE_CONSTRUCT || behaviours == asBEHAVE_DESTRUCT) {
        stream << std::format("\t{};\n", f->GetDeclaration(false, true, true));
      }
    }
    for (int j = 0; j < t->GetMethodCount(); ++j) {
      const auto m = t->GetMethodByIndex(j);
      stream << std::format("\t{};\n", m->GetDeclaration(false, true, true));
    }
    for (int j = 0; j < t->GetPropertyCount(); ++j) {
      stream << std::format("\t{};\n", t->GetPropertyDeclaration(j, true));
    }
    for (int j = 0; j < t->GetChildFuncdefCount(); ++j) {
      stream << std::format("\tfuncdef {};\n", t->GetChildFuncdef(j)->GetFuncdefSignature()->GetDeclaration(false));
    }
    stream << "}\n";
    if (not ns.empty())
      stream << "}\n";
  }
}

template <class Stream>
void printGlobalFunctionList(const asIScriptEngine* engine, Stream& stream) {
  for (int i = 0; i < engine->GetGlobalFunctionCount(); i++) {
    const auto f = engine->GetGlobalFunctionByIndex(i);
    if (not f)
      continue;
    const std::string_view ns = f->GetNamespace();
    if (not ns.empty())
      stream << std::format("namespace {} {{ ", ns);
    stream << std::format("{};", f->GetDeclaration(false, false, true));
    if (not ns.empty())
      stream << " }";
    stream << "\n";
  }
}

template <class Stream>
void printGlobalPropertyList(const asIScriptEngine* engine, Stream& stream) {
  for (int i = 0; i < engine->GetGlobalPropertyCount(); i++) {
    const char* name;
    const char* ns0;
    int type;
    engine->GetGlobalPropertyByIndex(i, &name, &ns0, &type, nullptr, nullptr, nullptr, nullptr);

    const std::string t = engine->GetTypeDeclaration(type, true);
    if (t.empty())
      continue;

    std::string_view ns = ns0;
    if (not ns.empty())
      stream << std::format("namespace {} {{ ", ns);

    stream << std::format("{} {};", t, name);
    if (not ns.empty())
      stream << " }";
    stream << "\n";
  }
}

template <class Stream>
void printGlobalTypedef(const asIScriptEngine* engine, Stream& stream) {
  for (int i = 0; i < engine->GetTypedefCount(); ++i) {
    const auto type = engine->GetTypedefByIndex(i);
    if (not type)
      continue;
    const std::string_view ns = type->GetNamespace();
    if (not ns.empty())
      stream << std::format("namespace {} {{\n", ns);
    stream << std::format("typedef {} {};\n", engine->GetTypeDeclaration(type->GetTypedefTypeId()), type->GetName());
    if (not ns.empty())
      stream << "}\n";
  }
}
} // namespace

/// @brief Generate 'as.predefined' file, which contains all defined symbols in C++. It is used by the language server.
void GenerateScriptPredefined(const asIScriptEngine* engine, const std::string& path) {
  assert(path.ends_with("as.predefined"));

  std::ofstream stream{path};

  printEnumList(engine, stream);

  printClassTypeList(engine, stream);

  printGlobalFunctionList(engine, stream);

  printGlobalPropertyList(engine, stream);

  printGlobalTypedef(engine, stream);
}
bool ScriptEngine::initialize() {

  m_engine = asCreateScriptEngine();
  if (!m_engine) {
    return false;
  }
  int r = m_engine->SetMessageCallback(asMETHOD(ScriptEngine, messageCallback), this, asCALL_THISCALL);
  assert(r >= 0);

  RegisterScriptArray(m_engine, true);
  RegisterStdString(m_engine);
  RegisterStdStringUtils(m_engine);
  RegisterScriptHandle(m_engine);
  RegisterScriptWeakRef(m_engine);
  RegisterScriptMath(m_engine);
  RegisterScriptMathComplex(m_engine);
  RegisterScriptDictionary(m_engine);
  RegisterScriptJson(m_engine);

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

  // GenerateScriptPredefined(m_engine, "as.predefined");
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
