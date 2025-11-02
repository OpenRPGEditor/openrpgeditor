#include <angelscript.h>
#include <cassert>
#include <imgui.h>
#include <new>

void ImGui_Construct_ImVec2(ImVec2* vec) { new (vec) ImVec2(0.f, 0.f); }
void ImGui_Construct_ImVec2Args(const float x, const float y, ImVec2* vec) { new (vec) ImVec2(x, y); }
void ImGui_Destruct_ImVec2(ImVec2* vec) { vec->~ImVec2(); }
void ImGui_Construct_ImVec4(ImVec4* vec) { new (vec) ImVec4(); }
void ImGui_Construct_ImVec4Args(const float x, const float y, float z, float w, ImVec4* vec) { new (vec) ImVec4(x, y, z, w); }
void ImGui_Destruct_ImVec4(ImVec4* vec) { vec->~ImVec4(); }
template <typename T>
void ImGui_Op_Negate_ImVec_Generic(asIScriptGeneric* generic) {
  *static_cast<T*>(generic->GetAddressOfReturnLocation()) = -(*static_cast<T*>(generic->GetObject()));
}

void ImGuiVec2Bindings(asIScriptEngine* engine) {
  int r = engine->RegisterObjectType("ImVec2", sizeof(ImVec2), asOBJ_VALUE | asOBJ_APP_CLASS_DAK);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ImGui_Construct_ImVec2), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(ImGui_Construct_ImVec2Args), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec2", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ImGui_Destruct_ImVec2), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opAssign(const ImVec2 &in)", asMETHODPR(ImVec2, operator=, (const ImVec2&), ImVec2&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "float& opIndex(uint index)", asMETHODPR(ImVec2, operator[], (size_t idx), float&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opMul(const float)", asFUNCTIONPR(operator*, (const ImVec2&, const float), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opMul(const ImVec2 &in)", asFUNCTIONPR(operator*, (const ImVec2&, const ImVec2&), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opDiv(const float)", asFUNCTIONPR(operator/, (const ImVec2&, const float), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opAdd(const ImVec2 &in)", asFUNCTIONPR(operator+, (const ImVec2&, const ImVec2&), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opSub(const ImVec2 &in)", asFUNCTIONPR(operator-, (const ImVec2&, const ImVec2&), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opDiv(const ImVec2 &in)", asFUNCTIONPR(operator/, (const ImVec2&, const ImVec2&), ImVec2), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opNeg()", asFUNCTION(ImGui_Op_Negate_ImVec_Generic<ImVec2>), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opMulAssign(const float)", asFUNCTIONPR(operator*=, (ImVec2&, const float), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opDivAssign(const float)", asFUNCTIONPR(operator/=, (ImVec2&, const float), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opAddAssign(const ImVec2 &in)", asFUNCTIONPR(operator+=, (ImVec2&, const ImVec2&), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opSubAssign(const ImVec2 &in)", asFUNCTIONPR(operator-=, (ImVec2&, const ImVec2&), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opMulAssign(const ImVec2 &in)", asFUNCTIONPR(operator*=, (ImVec2&, const ImVec2&), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 &opDivAssign(const ImVec2 &in)", asFUNCTIONPR(operator*=, (ImVec2&, const ImVec2&), ImVec2&), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec2", "ImVec2 opEquals(const ImVec2 &in)", asFUNCTIONPR(operator==, (const ImVec2&, const ImVec2&), bool), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec2", "float x", asOFFSET(ImVec2, x));
  assert(r >= 0);
  r = engine->RegisterObjectProperty("ImVec2", "float y", asOFFSET(ImVec2, y));
  assert(r >= 0);
}

void ImGuiVec4Bindings(asIScriptEngine* engine) {
  int r = engine->RegisterObjectType("ImVec4", sizeof(ImVec4), asOBJ_VALUE | asOBJ_APP_CLASS_DAK);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ImGui_Construct_ImVec4), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(ImGui_Construct_ImVec4Args), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectBehaviour("ImVec4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ImGui_Destruct_ImVec4), asCALL_CDECL_OBJLAST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec4", "ImVec4 &opAssign(const ImVec4 &in)", asMETHODPR(ImVec4, operator=, (const ImVec4&), ImVec4&), asCALL_THISCALL);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec4", "ImVec4 opMul(const ImVec4 &in)", asFUNCTIONPR(operator*, (const ImVec4&, const ImVec4&), ImVec4), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec4", "ImVec4 opSub(const ImVec4 &in)", asFUNCTIONPR(operator-, (const ImVec4&, const ImVec4&), ImVec4), asCALL_CDECL_OBJFIRST);
  assert(r >= 0);
  r = engine->RegisterObjectMethod("ImVec4", "ImVec4 opEquals(const ImVec4 &in)", asFUNCTIONPR(operator==, (const ImVec4&, const ImVec4&), bool), asCALL_CDECL_OBJFIRST);
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
void ImGuiMathBindings(asIScriptEngine* engine) {
  ImGuiVec2Bindings(engine);
  ImGuiVec4Bindings(engine);
}