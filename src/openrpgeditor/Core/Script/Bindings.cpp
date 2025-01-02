#include "Core/Script/Bindings.hpp"

#include "Core/MainWindow.hpp"
#include "ScriptEngine.hpp"

void ImGuiBindings();

void RegisterBindings() {
  MainWindow::RegisterBindings();
  ImGuiBindings();
}

bool ImGui_Begin_Wrapper(const std::string& id, bool open, bool& closed, int flags) {
  bool isOpen = ImGui::Begin(id.c_str(), &open, flags);
  closed = !open;
  return isOpen;
}

void ImGui_Text_Wrapper(const std::string& text) { ImGui::TextUnformatted(text.c_str()); }

void ImGui_Construct_ImVec2(ImVec2* vec) { new (vec) ImVec2(); }
void ImGui_Construct_ImVec2Args(const float x, const float y, ImVec2* vec) { new (vec) ImVec2(x, y); }
void ImGui_Destruct_ImVec2(ImVec2* vec) { vec->~ImVec2(); }
void ImGui_Construct_ImVec4(ImVec4* vec) { new (vec) ImVec4(); }
void ImGui_Construct_ImVec4Args(const float x, const float y, float z, float w, ImVec4* vec) { new (vec) ImVec4(x, y, z, w); }
void ImGui_Destruct_ImVec4(ImVec4* vec) { vec->~ImVec4(); }

void ImGuiMathBindings(asIScriptEngine* engine) {
  int r = engine->RegisterObjectType("ImVec2", sizeof(ImVec2), asOBJ_VALUE | asOBJ_APP_CLASS_DAK);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ImGui_Construct_ImVec2), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(ImGui_Construct_ImVec2Args), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ImGui_Destruct_ImVec2), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  engine->RegisterObjectMethod("ImVec2", "ImVec2 &opAssign(const ImVec2 &in)", asMETHODPR(ImVec2, operator=, (const ImVec2&), ImVec2&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec2", "float x", asOFFSET(ImVec2, x));
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec2", "float y", asOFFSET(ImVec2, y));
  assert(r >= 0);
  r = engine->RegisterObjectType("ImVec4", sizeof(ImVec4), asOBJ_VALUE | asOBJ_APP_CLASS_DAK);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ImGui_Construct_ImVec4), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(ImGui_Construct_ImVec4Args), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ImGui_Destruct_ImVec4), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  engine->RegisterObjectMethod("ImVec4", "ImVec4 &opAssign(const ImVec4 &in)", asMETHODPR(ImVec4, operator=, (const ImVec4&), ImVec4&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec4", "float x", asOFFSET(ImVec4, x));
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec4", "float y", asOFFSET(ImVec4, y));
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec4", "float z", asOFFSET(ImVec4, z));
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec4", "float w", asOFFSET(ImVec4, w));
  assert(r >= 0);
}
void ImGuiBindings() {
  auto engine = ScriptEngine::instance()->engine();

  auto def = engine->GetDefaultNamespace();
  ImGuiMathBindings(engine);
  engine->SetDefaultNamespace("ImGui");
  int r = engine->RegisterGlobalFunction("bool Begin(const string& in, bool, bool &out, int = 0)", asFUNCTION(ImGui_Begin_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void End()", asFUNCTION(ImGui::End), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void Text(const string& in)", asFUNCTION(ImGui_Text_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("double GetTime()", asFUNCTION(ImGui::GetTime), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("int GetFrameCount()", asFUNCTION(ImGui::GetFrameCount), asCALL_CDECL);
  assert(r >= 0);
  engine->SetDefaultNamespace(def);
}
