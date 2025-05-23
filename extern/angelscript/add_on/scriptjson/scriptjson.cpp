#include "scriptjson.h"

#include <assert.h> // assert()
#include <fstream>

#include "../scriptarray/scriptarray.h"
#include "../scriptjson/scriptjson.h"

#include <iostream>

BEGIN_AS_NAMESPACE

// TODO: This needs some rework, some of the Generic calls are completely busted, those have been disabled for the time being
// TODO: Array subscripting is incomplete, fix
// TODO: Add ability to create json objects with known value up front

CScriptJson* CScriptJson::Create(asIScriptEngine* engine) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  return obj;
}

CScriptJson* CScriptJson::Create(asIScriptEngine* engine, bool v) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  *obj->js_info = v;
  return obj;
}

CScriptJson* CScriptJson::Create(asIScriptEngine* engine, asINT64 v) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  *obj->js_info = v;
  return obj;
}

CScriptJson* CScriptJson::Create(asIScriptEngine* engine, double v) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  *obj->js_info = v;
  return obj;
}

CScriptJson* CScriptJson::Create(asIScriptEngine* engine, const std::string& v) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  *obj->js_info = v;
  return obj;
}

CScriptJson* CScriptJson::Create(asIScriptEngine* engine, nlohmann::ordered_json js) {
  // Use the custom memory routine from AngelScript to allow application to better control how much memory is used
  CScriptJson* obj = (CScriptJson*)asAllocMem(sizeof(CScriptJson));
  new (obj) CScriptJson(engine);
  *(obj->js_info) = js;
  return obj;
}

void CScriptJson::AddRef() const { asAtomicInc(refCount); }

void CScriptJson::Release() const {
  if (asAtomicDec(refCount) == 0) {
    this->~CScriptJson();
    asFreeMem(const_cast<CScriptJson*>(this));
  }
}

CScriptJson& CScriptJson::operator=(bool other) {
  // Clear everything we had before
  js_info->clear();

  *js_info = other;

  return *this;
}

CScriptJson& CScriptJson::operator=(asINT64 other) {
  // Clear everything we had before
  js_info->clear();

  *js_info = other;

  return *this;
}

CScriptJson& CScriptJson::operator=(double other) {
  // Clear everything we had before
  js_info->clear();

  *js_info = other;

  return *this;
}

CScriptJson& CScriptJson::operator=(const std::string& other) {
  // Clear everything we had before
  js_info->clear();

  *js_info = other;

  return *this;
}

CScriptJson& CScriptJson::operator=(const CScriptArray& other) {
  nlohmann::ordered_json js_temp = nlohmann::ordered_json::array({});
  for (asUINT i = 0; i < other.GetSize(); i++) {
    CScriptJson** node = (CScriptJson**)other.At(i);
    if (node && *node) {
      js_temp += *(*node)->js_info;
    }
  }

  // Clear everything we had before
  js_info->clear();

  *js_info = js_temp;

  return *this;
}

CScriptJson& CScriptJson::operator=(const CScriptJson& other) {
  // Clear everything we had before
  js_info->clear();

  *js_info = *other.js_info;

  return *this;
}

void CScriptJson::Set(const jsonKey_t& key, const bool& value) { (*js_info)[key] = value; }

void CScriptJson::Set(const jsonKey_t& key, const asINT64& value) { (*js_info)[key] = value; }

void CScriptJson::Set(const jsonKey_t& key, const double& value) { (*js_info)[key] = value; }

void CScriptJson::Set(const jsonKey_t& key, const std::string& value) { (*js_info)[key] = value; }

void CScriptJson::Set(const jsonKey_t& key, const CScriptArray& value) {
  nlohmann::ordered_json js_temp = nlohmann::ordered_json::array({});
  for (asUINT i = 0; i < value.GetSize(); i++) {
    CScriptJson** node = (CScriptJson**)value.At(i);
    if (node && *node) {
      js_temp += *(*node)->js_info;
    }
  }
  (*js_info)[key] = js_temp;
}

bool CScriptJson::Get(const jsonKey_t& key, bool& value) const {
  if (js_info->contains(key)) {
    if (js_info->is_boolean()) {
      value = static_cast<bool>((*js_info)[key]);
      return true;
    }
  }
  return false;
}

bool CScriptJson::Get(const jsonKey_t& key, asINT64& value) const {
  if (js_info->contains(key)) {
    if (js_info->is_number()) {
      value = static_cast<asINT64>((*js_info)[key]);
      return true;
    }
  }
  return false;
}

bool CScriptJson::Get(const jsonKey_t& key, double& value) const {
  if (js_info->contains(key)) {
    if (js_info->is_number()) {
      value = static_cast<double>((*js_info)[key]);
      return true;
    }
  }
  return false;
}

bool CScriptJson::Get(const jsonKey_t& key, std::string& value) const {
  if (js_info->contains(key)) {
    if (js_info->is_string()) {
      value = static_cast<std::string>((*js_info)[key]);
      return true;
    }
  }
  return false;
}

bool CScriptJson::Get(const jsonKey_t& key, CScriptArray& value) const {
  if (!js_info->contains(key) || !(*js_info)[key].is_array())
    return false;

  nlohmann::ordered_json js_temp = (*js_info)[key];
  value.Resize(js_temp.size());

  for (asUINT i = 0; i < js_temp.size(); ++i) {
    CScriptJson* childNode = Create(engine);
    *(childNode->js_info) = js_temp[i];
    value.SetValue(i, &childNode);
    childNode->Release();
  }
  return true;
}

bool CScriptJson::GetBool() { return static_cast<bool>(*js_info); }

int CScriptJson::GetNumber() { return static_cast<int>(*js_info); }

double CScriptJson::GetReal() { return static_cast<double>(*js_info); }

std::string CScriptJson::GetString() { return static_cast<std::string>(*js_info); }

std::string CScriptJson::Dump(int indent) { return js_info->dump(indent); }

CScriptArray* CScriptJson::GetArray() {
  CScriptArray* retVal = CScriptArray::Create(engine->GetTypeInfoByDecl("array<JsonValue@>"));
  for (nlohmann::ordered_json::iterator it = js_info->begin(); it != js_info->end(); ++it) {
    CScriptJson* childNode = CScriptJson::Create(engine, *it);

    retVal->InsertLast(childNode);
    childNode->Release();
  }
  return retVal;
}

CScriptJson* CScriptJson::operator[](const jsonKey_t& key) {
  CScriptJson* retVal = Create(engine);
  retVal->js_info = &(*js_info)[key];
  // Return the existing value if it exists, else insert an empty value
  return retVal;
}

const CScriptJson* CScriptJson::operator[](const jsonKey_t& key) const {
  if (js_info->contains(key)) {
    CScriptJson* retVal = Create(engine);
    *(retVal->js_info) = (*js_info)[key];
    return retVal;
  }

  return 0;
}

CScriptJson* CScriptJson::operator[](size_t idx) {
  if (!js_info->is_array()) {
    return 0;
  }
  if (idx < js_info->size()) {
    CScriptJson* retVal = Create(engine);
    retVal->js_info = &(*js_info)[idx];
    // Return the existing value if it exists, else insert an empty value
    return retVal;
  }

  // Need to do this the slow way because we don't have `resize` >.<
  while (js_info->size() < idx) {
    js_info->emplace_back();
  }

  CScriptJson* retVal = Create(engine);
  retVal->js_info = &(*js_info)[idx];
  // Return the existing value if it exists, else insert an empty value
  return retVal;
}

const CScriptJson* CScriptJson::operator[](size_t idx) const {
  if (!js_info->is_array()) {
    return 0;
  }
  if (idx < js_info->size()) {
    CScriptJson* retVal = Create(engine);
    *(retVal->js_info) = (*js_info)[idx];
    return retVal;
  }

  return 0;
}

void CScriptJson::push_back(CScriptJson* value) {
  if (!js_info->is_null() && !js_info->is_array()) {
    asIScriptContext* ctx = asGetActiveContext();
    if (ctx)
      ctx->SetException("Invalid access to non-array json");
    return;
  }

  js_info->push_back(*value->js_info);
}

bool CScriptJson::Exists(const jsonKey_t& key) const { return js_info->contains(key); }

bool CScriptJson::IsEmpty() const { return js_info->empty(); }

asUINT CScriptJson::GetSize() const { return js_info->size(); }

void CScriptJson::Clear() { js_info->clear(); }

CScriptJsonType CScriptJson::Type() {
  switch (js_info->type()) {
  case nlohmann::ordered_json::value_t::object:
    return OBJECT_VALUE;
  case nlohmann::ordered_json::value_t::array:
    return ARRAY_VALUE;
  case nlohmann::ordered_json::value_t::string:
    return STRING_VALUE;
  case nlohmann::ordered_json::value_t::boolean:
    return BOOLEAN_VALUE;
  case nlohmann::ordered_json::value_t::number_integer:
  case nlohmann::ordered_json::value_t::number_unsigned:
    return NUMBER_VALUE;
  case nlohmann::ordered_json::value_t::number_float:
    return REAL_VALUE;
  default:
    return NULL_VALUE;
  }
}

int CScriptJson::GetRefCount() { return refCount; }

CScriptJson::CScriptJson(asIScriptEngine* e) {
  js_info = new nlohmann::ordered_json();
  // We start with one reference
  refCount = 1;

  engine = e;
}

CScriptJson::~CScriptJson() {
  Clear();
  delete js_info;
}

void ScriptJsonFactory_Generic(asIScriptGeneric* gen) { *(CScriptJson**)gen->GetAddressOfReturnLocation() = CScriptJson::Create(gen->GetEngine()); }
void ScriptJsonFactory_Generic_Bool(asIScriptGeneric* gen) { *(CScriptJson**)gen->GetAddressOfReturnLocation() = CScriptJson::Create(gen->GetEngine(), (bool)gen->GetArgByte(0)); }
void ScriptJsonFactory_Generic_Int64(asIScriptGeneric* gen) {
  //
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = CScriptJson::Create(gen->GetEngine(), (asINT64)gen->GetArgQWord(0));
}
void ScriptJsonFactory_Generic_Double(asIScriptGeneric* gen) { *(CScriptJson**)gen->GetAddressOfReturnLocation() = CScriptJson::Create(gen->GetEngine(), (double)gen->GetArgDouble(0)); }
void ScriptJsonFactory_Generic_String(asIScriptGeneric* gen) { *(CScriptJson**)gen->GetAddressOfReturnLocation() = CScriptJson::Create(gen->GetEngine(), *(std::string*)(gen->GetArgAddress(0))); }

void ScriptJsonAddRef_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  json->AddRef();
}

void ScriptJsonRelease_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  json->Release();
}

void ScriptJsonAssignBool_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  *json = (bool)gen->GetArgByte(0);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonAssignInt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  *json = (asINT64)gen->GetArgQWord(0);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonAssignFlt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  *json = gen->GetArgDouble(0);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonAssignStr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  std::string* other = *(std::string**)gen->GetAddressOfArg(0);
  *json = *other;
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonAssignArr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  CScriptArray* other = *(CScriptArray**)gen->GetAddressOfArg(0);
  *json = *other;
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonAssign_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  CScriptJson* other = *(CScriptJson**)gen->GetAddressOfArg(0);
  *json = *other;
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = json;
}

void ScriptJsonSetBool_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  json->Set(*key, *(bool*)ref);
}

void ScriptJsonSetInt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  json->Set(*key, *(asINT64*)ref);
}

void ScriptJsonSetFlt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  json->Set(*key, *(double*)ref);
}

void ScriptJsonSetStr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  json->Set(*key, *(std::string*)ref);
}

void ScriptJsonSetArr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  json->Set(*key, *(CScriptArray*)ref);
}

void ScriptJsonGetBool_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  *(bool*)gen->GetAddressOfReturnLocation() = json->Get(*key, *(bool*)ref);
}

void ScriptJsonGetInt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  *(bool*)gen->GetAddressOfReturnLocation() = json->Get(*key, *(asINT64*)ref);
}

void ScriptJsonGetFlt_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  *(bool*)gen->GetAddressOfReturnLocation() = json->Get(*key, *(double*)ref);
}

void ScriptJsonGetStr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  *(bool*)gen->GetAddressOfReturnLocation() = json->Get(*key, *(std::string*)ref);
}

void ScriptJsonGetArr_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  void* ref = *(void**)gen->GetAddressOfArg(1);
  *(bool*)gen->GetAddressOfReturnLocation() = json->Get(*key, *(CScriptArray*)ref);
}

void ScriptJsonExists_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  bool ret = json->Exists(*key);
  *(bool*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptJsonIsEmpty_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  bool ret = json->IsEmpty();
  *(bool*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptJsonGetSize_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  asUINT ret = json->GetSize();
  *(asUINT*)gen->GetAddressOfReturnLocation() = ret;
}

void ScriptJsonClear_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  json->Clear();
}

void ScriptJsonGetType_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetObject();
  CScriptJsonType ret = json->Type();
  *(CScriptJsonType*)gen->GetAddressOfReturnLocation() = ret;
}

static void CScriptJson_opIndex_Generic(asIScriptGeneric* gen) {
  CScriptJson* self = (CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = self->operator[](*key);
}

static void CScriptJson_opIndex_const_Generic(asIScriptGeneric* gen) {
  const CScriptJson* self = (const CScriptJson*)gen->GetObject();
  jsonKey_t* key = *(jsonKey_t**)gen->GetAddressOfArg(0);
  *(const CScriptJson**)gen->GetAddressOfReturnLocation() = self->operator[](*key);
}

static CScriptJson* JsonParseFile(const std::string& file) {
  std::ifstream inputFile(file.c_str());
  if (!inputFile.is_open()) {
    return nullptr;
  }

  asIScriptContext* currentContext = asGetActiveContext();
  if (currentContext) {
    asIScriptEngine* engine = currentContext->GetEngine();
    if (engine) {
      CScriptJson* newNode = CScriptJson::Create(engine);
      *(newNode->js_info) = nlohmann::ordered_json::parse(inputFile);
      return newNode;
    }
  }
  return NULL;
}

static CScriptJson* JsonParse(const std::string& str) {
  asIScriptContext* currentContext = asGetActiveContext();
  if (currentContext) {
    asIScriptEngine* engine = currentContext->GetEngine();
    if (engine) {
      CScriptJson* newNode = CScriptJson::Create(engine);
      *(newNode->js_info) = nlohmann::ordered_json::parse(str.c_str());
      return newNode;
    }
  }
  return NULL;
}

// Json to text
static bool JsonWriteFile(const CScriptJson& node, const std::string& file) {
  FILE* outputFile = NULL;
  if ((outputFile = fopen(file.c_str(), "w")) == NULL) {
    return false;
  }
  char* data_str;
  std::string dump_str = node.js_info->dump(4);
  data_str = (char*)malloc(dump_str.length() + 1);
  strcpy(data_str, dump_str.c_str());

  fwrite(data_str, strlen(data_str), 1, outputFile);
  fclose(outputFile);
  free(data_str);
  return true;
}

static bool JsonWrite(const CScriptJson& node, std::string& content) {
  content = node.js_info->dump(1, '\t');
  return true;
}

static void ScriptJson_ParseFile_Generic(asIScriptGeneric* gen) {
  std::string* file = (std::string*)gen->GetArgAddress(0);

  CScriptJson* ret = JsonParseFile(*file);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = ret;
}

static void ScriptJson_Parse_Generic(asIScriptGeneric* gen) {
  std::string* file = (std::string*)gen->GetArgAddress(0);

  CScriptJson* ret = JsonParse(*file);
  *(CScriptJson**)gen->GetAddressOfReturnLocation() = ret;
}

static void ScriptJson_WriteFile_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetArgAddress(0);
  std::string* file = (std::string*)gen->GetArgAddress(1);

  bool ret = JsonWriteFile(*json, *file);
  *(bool*)gen->GetAddressOfReturnLocation() = ret;
}

static void ScriptJson_Write_Generic(asIScriptGeneric* gen) {
  CScriptJson* json = (CScriptJson*)gen->GetArgAddress(0);
  std::string* content = (std::string*)gen->GetArgAddress(1);

  bool ret = JsonWrite(*json, *content);
  *(bool*)gen->GetAddressOfReturnLocation() = ret;
}

//--------------------------------------------------------------------------
// Register the type

void RegisterScriptJson_Native(asIScriptEngine* engine) {
  int r;

  // The array<string> type must be available
  assert(engine->GetTypeInfoByDecl("array<string>"));

  r = engine->RegisterEnum("jsonType");
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "OBJECT_VALUE", OBJECT_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "ARRAY_VALUE", ARRAY_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "BOOLEAN_VALUE", BOOLEAN_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "STRING_VALUE", STRING_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "NUMBER_VALUE", NUMBER_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "REAL_VALUE", REAL_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "NULL_VALUE", NULL_VALUE);
  assert(r >= 0);

  r = engine->RegisterObjectType("json", sizeof(CScriptJson), asOBJ_REF);
  assert(r >= 0);
  // Use the generic interface to construct the object since we need the engine pointer, we could also have retrieved the engine pointer from the active context
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f()", asFUNCTION(ScriptJsonFactory_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f(bool v)", asFUNCTION(ScriptJsonFactory_Generic_Bool), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f(int64 v)", asFUNCTION(ScriptJsonFactory_Generic_Int64), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f(double v)", asFUNCTION(ScriptJsonFactory_Generic_Double), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f(const string& in)", asFUNCTION(ScriptJsonFactory_Generic_String), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_ADDREF, "void f()", asMETHOD(CScriptJson, AddRef), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_RELEASE, "void f()", asMETHOD(CScriptJson, Release), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "json &opAssign(bool)", asMETHODPR(CScriptJson, operator=, (bool), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(int64)", asMETHODPR(CScriptJson, operator=, (asINT64), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(double)", asMETHODPR(CScriptJson, operator=, (double), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const string& in)", asMETHODPR(CScriptJson, operator=, (const std::string&), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const array<json@> &in)", asMETHODPR(CScriptJson, operator=, (const CScriptArray&), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const json &in)", asMETHODPR(CScriptJson, operator=, (const CScriptJson&), CScriptJson&), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const bool&in)", asMETHODPR(CScriptJson, Set, (const std::string&, const bool&), void), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, bool &out) const", asMETHODPR(CScriptJson, Get, (const std::string&, bool&) const, bool), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const int64&in)", asMETHODPR(CScriptJson, Set, (const std::string&, const asINT64&), void), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, int64 &out) const", asMETHODPR(CScriptJson, Get, (const std::string&, asINT64&) const, bool), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const double&in)", asMETHODPR(CScriptJson, Set, (const std::string&, const double&), void), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, double &out) const", asMETHODPR(CScriptJson, Get, (const std::string&, double&) const, bool), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const string&in)", asMETHODPR(CScriptJson, Set, (const std::string&, const std::string&), void), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, string &out) const", asMETHODPR(CScriptJson, Get, (const std::string&, std::string&) const, bool), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const array<json@>&in)", asMETHODPR(CScriptJson, Set, (const std::string&, const CScriptArray&), void), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, array<json@> &out) const", asMETHODPR(CScriptJson, Get, (const std::string&, CScriptArray&) const, bool), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "bool exists(const string &in) const", asMETHOD(CScriptJson, Exists), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool isEmpty() const", asMETHOD(CScriptJson, IsEmpty), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "uint getSize() const", asMETHOD(CScriptJson, GetSize), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "void clear()", asMETHOD(CScriptJson, Clear), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "jsonType getType()", asMETHOD(CScriptJson, Type), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "json &opIndex(const string &in)", asMETHODPR(CScriptJson, operator[], (const jsonKey_t&), CScriptJson*), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "const json &opIndex(const string &in) const", asMETHODPR(CScriptJson, operator[], (const jsonKey_t&) const, const CScriptJson*), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "json &opIndex(uint)", asMETHODPR(CScriptJson, operator[], (size_t), CScriptJson*), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "const json &opIndex(uint) const", asMETHODPR(CScriptJson, operator[], (size_t) const, const CScriptJson*), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void push_back(json& in) const", asMETHODPR(CScriptJson, push_back, (CScriptJson*), void), asCALL_THISCALL);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "bool opConv()", asMETHOD(CScriptJson, GetBool), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "string opConv()", asMETHOD(CScriptJson, GetString), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "int opConv()", asMETHOD(CScriptJson, GetNumber), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "double opConv()", asMETHOD(CScriptJson, GetReal), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "array<json@>& opConv()", asMETHOD(CScriptJson, GetArray), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "string dump(int depth=-1)", asMETHOD(CScriptJson, Dump), asCALL_THISCALL);
  assert(r >= 0);

  // Json functions
  r = engine->RegisterGlobalFunction("json@ jsonParseFile(const string& in)", asFUNCTIONPR(JsonParseFile, (const std::string&), CScriptJson*), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("json@ jsonParse(const string& in)", asFUNCTIONPR(JsonParse, (const std::string&), CScriptJson*), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool jsonWriteFile(const json& in json,const string& in)", asFUNCTIONPR(JsonWriteFile, (const CScriptJson& node, const std::string&), bool), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool jsonWrite(const json& in json,string& out)", asFUNCTIONPR(JsonWrite, (const CScriptJson& node, std::string&), bool), asCALL_CDECL);
  assert(r >= 0);
}

void RegisterScriptJson_Generic(asIScriptEngine* engine) {
  int r;

  r = engine->RegisterEnum("jsonType");
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "OBJECT_VALUE", OBJECT_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "ARRAY_VALUE", ARRAY_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "BOOLEAN_VALUE", BOOLEAN_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "STRING_VALUE", STRING_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "NUMBER_VALUE", NUMBER_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "REAL_VALUE", REAL_VALUE);
  assert(r >= 0);
  r = engine->RegisterEnumValue("jsonType", "NULL_VALUE", NULL_VALUE);
  assert(r >= 0);

  r = engine->RegisterObjectType("json", sizeof(CScriptJson), asOBJ_REF);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_FACTORY, "json@ f()", asFUNCTIONPR(ScriptJsonFactory_Generic, (asIScriptGeneric*), void), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_ADDREF, "void f()", asFUNCTION(ScriptJsonAddRef_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("json", asBEHAVE_RELEASE, "void f()", asFUNCTION(ScriptJsonRelease_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "json &opAssign(bool)", asFUNCTION(ScriptJsonAssignBool_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(int64)", asFUNCTION(ScriptJsonAssignInt_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(double)", asFUNCTION(ScriptJsonAssignFlt_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const string &in)", asFUNCTION(ScriptJsonAssignStr_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const array<json@> &in)", asFUNCTION(ScriptJsonAssignArr_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "json &opAssign(const json &in)", asFUNCTION(ScriptJsonAssign_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const bool&in)", asFUNCTION(ScriptJsonSetBool_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, bool&out) const", asFUNCTION(ScriptJsonGetBool_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const int64&in)", asFUNCTION(ScriptJsonSetInt_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, int64&out) const", asFUNCTION(ScriptJsonGetInt_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const double&in)", asFUNCTION(ScriptJsonSetFlt_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, double&out) const", asFUNCTION(ScriptJsonGetFlt_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const string&in)", asFUNCTION(ScriptJsonSetStr_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, string&out) const", asFUNCTION(ScriptJsonGetStr_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "void set(const string &in, const array<json@>&in)", asFUNCTION(ScriptJsonSetArr_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool get(const string &in, array<json@>&out) const", asFUNCTION(ScriptJsonGetArr_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "bool exists(const string &in) const", asFUNCTION(ScriptJsonExists_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "bool isEmpty() const", asFUNCTION(ScriptJsonIsEmpty_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "uint getSize() const", asFUNCTION(ScriptJsonGetSize_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "void clear()", asFUNCTION(ScriptJsonClear_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "void jsonType getType()", asFUNCTION(ScriptJsonGetType_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "json &opIndex(const string &in)", asFUNCTION(CScriptJson_opIndex_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "const json &opIndex(const string &in) const", asFUNCTION(CScriptJson_opIndex_const_Generic), asCALL_GENERIC);
  assert(r >= 0);

  r = engine->RegisterObjectMethod("json", "bool opConv()", asFUNCTION(ScriptJsonGetBool_Generic), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "string opConv()", asFUNCTION(ScriptJsonGetStr_Generic), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "int opConv()", asFUNCTION(ScriptJsonGetInt_Generic), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "double opConv()", asFUNCTION(ScriptJsonGetFlt_Generic), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "array<json@>& opConv()", asFUNCTION(ScriptJsonGetArr_Generic), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("json", "string dump(int depth=-1)", asMETHOD(CScriptJson, Dump), asCALL_THISCALL);
  assert(r >= 0);
  // Json functions
  r = engine->RegisterGlobalFunction("json@ jsonParseFile(const string& file)", asFUNCTION(ScriptJson_ParseFile_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("json@ jsonParse(const string& str)", asFUNCTION(ScriptJson_Parse_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool jsonWriteFile(const json& in json,const string& file)", asFUNCTION(ScriptJson_WriteFile_Generic), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool jsonWrite(const json& in json,string& out str)", asFUNCTION(ScriptJson_Write_Generic), asCALL_GENERIC);
  assert(r >= 0);
}

void RegisterScriptJson(asIScriptEngine* engine) {
  //    if( strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") )
  //        RegisterScriptJson_Generic(engine);
  //    else
  RegisterScriptJson_Native(engine);
}

END_AS_NAMESPACE
