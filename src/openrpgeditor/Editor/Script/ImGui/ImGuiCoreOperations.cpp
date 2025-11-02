#include "Editor/Script/ImGui/ImGuiCoreOperations.hpp"

#include "Editor/ImGuiExt/ImGuiUtils.hpp"

#include <angelscript.h>
#include <cassert>
#include <imgui.h>
#include <imgui_internal.h>
#include <string>
bool ImGui_Begin_Wrapper(const std::string& id, const bool showCloseButton, bool* still_open, const ImGuiWindowFlags flags) {
  bool open = true;
  const bool ret = ImGui::Begin(id.c_str(), showCloseButton ? &open : nullptr, flags);
  if (still_open) {
    *still_open = open;
  }
  return ret;
}

bool ImGui_Begin_Child_Wrapper(const std::string& id, const ImVec2& size, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags) {
  return ImGui::BeginChild(id.c_str(), size, child_flags, window_flags);
}

void ImGui_Window_Pos_Named_Wrapper(const std::string& id, const ImVec2& pos, ImGuiCond cond) { ImGui::SetWindowPos(id.c_str(), pos, cond); }
void ImGui_Window_Size_Named_Wrapper(const std::string& id, const ImVec2& size, ImGuiCond cond) { ImGui::SetWindowSize(id.c_str(), size, cond); }
void ImGui_Window_Collapsed_Named_Wrapper(const std::string& id, bool collapsed, ImGuiCond cond) { ImGui::SetWindowCollapsed(id.c_str(), collapsed, cond); }
void ImGui_Window_Focus_Named_Wrapper(const std::string& id) { ImGui::SetWindowFocus(id.c_str()); }

void ImGui_Text_Wrapper(const std::string& text) { ImGui::TextUnformatted(text.c_str()); }

void ImGui_CalcTextSize_Wrapper_Generic(asIScriptGeneric* gen) {
  // Use a pointer to the string to reduce copies
  const std::string* text = static_cast<const std::string*>(gen->GetArgAddress(0));
  const int text_end = *static_cast<int*>(gen->GetAddressOfArg(1));
  const bool hide_text_after_double_hash = *static_cast<bool*>(gen->GetAddressOfArg(2));
  const float wrap_width = *static_cast<float*>(gen->GetAddressOfArg(3));
  const ImVec2 ret = ImGui::CalcTextSize(text->c_str(), text_end == -1 ? nullptr : text->c_str() + text_end, hide_text_after_double_hash, wrap_width);
  *static_cast<ImVec2*>(gen->GetAddressOfReturnLocation()) = ret;
}

void ImGui_GetDPIScaledSize_Float_Wrapper(asIScriptGeneric* gen) {
  const float width = *static_cast<float*>(gen->GetAddressOfArg(0));
  const float height = *static_cast<float*>(gen->GetAddressOfArg(1));
  *static_cast<ImVec2*>(gen->GetAddressOfReturnLocation()) = ImGui::GetDPIScaledSize(width, height);
}

void ImGui_GetDPIScaledSize_ImVec_Wrapper(asIScriptGeneric* gen) {
  const ImVec2* size = static_cast<ImVec2*>(gen->GetArgAddress(0));
  *static_cast<ImVec2*>(gen->GetAddressOfReturnLocation()) = ImGui::GetDPIScaledSize(*size);
}
std::string ImGui_GetVersion() { return ImGui::GetVersion(); }

void ImGuiRegisterTypedefs(asIScriptEngine* engine) {
  engine->RegisterTypedef("ImGuiID", "uint");
  engine->RegisterTypedef("ImS8", "int8");
  engine->RegisterTypedef("ImU8", "uint8");
  engine->RegisterTypedef("ImS16", "int16");
  engine->RegisterTypedef("ImU16", "uint16");
  engine->RegisterTypedef("ImS32", "int");
  engine->RegisterTypedef("ImU32", "uint");
  engine->RegisterTypedef("ImS64", "int64");
  engine->RegisterTypedef("ImU64", "uint64");
  engine->RegisterTypedef("ImGuiCol", "int");
  engine->RegisterTypedef("ImGuiCond", "int");
  engine->RegisterTypedef("ImGuiDataType", "int");
  engine->RegisterTypedef("ImGuiMouseButton", "int");
  engine->RegisterTypedef("ImGuiMouseCursor", "int");
  engine->RegisterTypedef("ImGuiStyleVar", "int");
  engine->RegisterTypedef("ImGuiTableBgTarget", "int");
  engine->RegisterTypedef("ImDrawFlags", "int");
  engine->RegisterTypedef("ImDrawListFlags", "int");
  engine->RegisterTypedef("ImFontAtlasFlags", "int");
  engine->RegisterTypedef("ImGuiBackendFlags", "int");
  engine->RegisterTypedef("ImGuiChildFlags", "int");
  engine->RegisterTypedef("ImGuiColorEditFlags", "int");
  engine->RegisterTypedef("ImGuiConfigFlags", "int");
  engine->RegisterTypedef("ImGuiComboFlags", "int");
  engine->RegisterTypedef("ImGuiDockNodeFlags", "int");
  engine->RegisterTypedef("ImGuiDragDropFlags", "int");
  engine->RegisterTypedef("ImGuiFocusedFlags", "int");
  engine->RegisterTypedef("ImGuiHoveredFlags", "int");
  engine->RegisterTypedef("ImGuiInputFlags", "int");
  engine->RegisterTypedef("ImGuiInputTextFlags", "int");
  engine->RegisterTypedef("ImGuiItemFlags", "int");
  engine->RegisterTypedef("ImGuiKeyChord", "int");
  engine->RegisterTypedef("ImGuiPopupFlags", "int");
  engine->RegisterTypedef("ImGuiMultiSelectFlags", "int");
  engine->RegisterTypedef("ImGuiSelectableFlags", "int");
  engine->RegisterTypedef("ImGuiSliderFlags", "int");
  engine->RegisterTypedef("ImGuiTabBarFlags", "int");
  engine->RegisterTypedef("ImGuiTabItemFlags", "int");
  engine->RegisterTypedef("ImGuiTableFlags", "int");
  engine->RegisterTypedef("ImGuiTableColumnFlags", "int");
  engine->RegisterTypedef("ImGuiTableRowFlags", "int");
  engine->RegisterTypedef("ImGuiTreeNodeFlags", "int");
  engine->RegisterTypedef("ImGuiViewportFlags", "int");
  engine->RegisterTypedef("ImGuiWindowFlags", "int");
  engine->RegisterTypedef("ImTextureID", "uint64");
  engine->RegisterTypedef("ImWchar32", "uint");
  engine->RegisterTypedef("ImWchar16", "uint16");
#ifdef IMGUI_USE_WCHAR32
  engine->RegisterTypedef("ImWchar", "uint");
#else
  engine->RegisterTypedef("ImWchar", "uint16");
#endif
  engine->RegisterTypedef("ImGuiSelectionUserData", "int64");
}

void ImGuiCoreOperationBindings(asIScriptEngine* engine) {
  auto def = engine->GetDefaultNamespace();
  ImGuiRegisterTypedefs(engine);
  engine->SetDefaultNamespace("ImGui");
  int r = engine->RegisterGlobalFunction("string GetVersion()", asFUNCTION(ImGui_GetVersion), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool Begin(const string& in id, bool show_close_button = false, bool&out p_open = false, ImGuiWindowFlags window_flags = 0)", asFUNCTION(ImGui_Begin_Wrapper),
                                     asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void End()", asFUNCTION(ImGui::End), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool BeginChild(const string& in id, const ImVec2& in size = ImVec2(), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0)",
                                     asFUNCTION(ImGui_Begin_Child_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool BeginChild(ImGuiID id, const ImVec2& in size = ImVec2(), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0)",
                                     asFUNCTIONPR(ImGui::BeginChild, (ImGuiID, const ImVec2&, ImGuiChildFlags, ImGuiWindowFlags), bool), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void EndChild()", asFUNCTION(ImGui::EndChild), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool IsWindowAppearing()", asFUNCTION(ImGui::IsWindowAppearing), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool IsWindowCollapsed()", asFUNCTION(ImGui::IsWindowCollapsed), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool IsWindowFocused(ImGuiFocusedFlags flags=0)", asFUNCTION(ImGui::IsWindowFocused), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("bool IsWindowHovered(ImGuiHoveredFlags flags=0)", asFUNCTION(ImGui::IsWindowHovered), asCALL_CDECL);
  assert(r >= 0);
  // r = engine->RegisterGlobalFunction("ImDrawList& GetWindowDrawList()", asFUNCTION(ImGui::GetWindowDrawList), asCALL_CDECL);
  // assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetWindowDpiScale()", asFUNCTION(ImGui::GetWindowDpiScale), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetDPIScaledValue(float v)", asFUNCTION(ImGui::GetDPIScaledValue), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("ImVec2 GetDPIScaledSize(const ImVec2& in size)", asFUNCTION(ImGui_GetDPIScaledSize_ImVec_Wrapper), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("ImVec2 GetDPIScaledSize(const float w, const float h)", asFUNCTION(ImGui_GetDPIScaledSize_Float_Wrapper), asCALL_GENERIC);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowPos(const ImVec2&in pos, ImGuiCond cond = 0, const ImVec2&in pivot = ImVec2())", asFUNCTION(ImGui::SetNextWindowPos), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowSize(const ImVec2&in size, ImGuiCond cond = 0)", asFUNCTION(ImGui::SetNextWindowSize), asCALL_CDECL);
  assert(r >= 0);
  // TODO: Constraints callback?
  r = engine->RegisterGlobalFunction("void SetNextWindowSizeConstraints(const ImVec2&in size_min, const ImVec2&in size_max, ImGuiCond cond = 0)", asFUNCTION(ImGui::SetNextWindowSizeConstraints),
                                     asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowContentSize(const ImVec2&in size, ImGuiCond cond = 0)", asFUNCTION(ImGui::SetNextWindowSizeConstraints), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0)", asFUNCTION(ImGui::SetNextWindowCollapsed), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowFocus()", asFUNCTION(ImGui::SetNextWindowFocus), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowScroll(const ImVec2& in scroll)", asFUNCTION(ImGui::SetNextWindowScroll), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowBgAlpha(float alpha)", asFUNCTION(ImGui::SetNextWindowBgAlpha), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetNextWindowViewport(ImGuiID viewport_id)", asFUNCTION(ImGui::SetNextWindowViewport), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowPos(const ImVec2&in pos, ImGuiCond cond = 0)", asFUNCTIONPR(ImGui::SetWindowPos, (const ImVec2&, ImGuiCond), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowSize(const ImVec2&in size, ImGuiCond cond = 0)", asFUNCTIONPR(ImGui::SetWindowSize, (const ImVec2&, ImGuiCond), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0)", asFUNCTIONPR(ImGui::SetWindowCollapsed, (bool, ImGuiCond), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowFocus()", asFUNCTIONPR(ImGui::SetWindowFocus, (), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowFontScale(float scale)", asFUNCTION(ImGui::SetWindowFontScale), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowPos(const string&in name, const ImVec2&in pos, ImGuiCond cond = 0)", asFUNCTION(ImGui_Window_Pos_Named_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowSize(const string&in name, const ImVec2&in size, ImGuiCond cond = 0)", asFUNCTION(ImGui_Window_Size_Named_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowCollapsed(const string&in name, bool collapsed, ImGuiCond cond = 0)", asFUNCTION(ImGui_Window_Collapsed_Named_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetWindowFocus(const string&in name)", asFUNCTION(ImGui_Window_Focus_Named_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetScrollX()", asFUNCTION(ImGui::GetScrollX), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetScrollY()", asFUNCTION(ImGui::GetScrollY), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollX(float scroll_x)", asFUNCTIONPR(ImGui::SetScrollX, (float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollY(float scroll_y)", asFUNCTIONPR(ImGui::SetScrollY, (float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetScrollMaxX()", asFUNCTION(ImGui::GetScrollMaxX), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("float GetScrollMaxY()", asFUNCTION(ImGui::GetScrollMaxY), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollHereX(float center_x_ratio = 0.5f)", asFUNCTIONPR(ImGui::SetScrollHereX, (float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollHereY(float center_y_ratio = 0.5f)", asFUNCTIONPR(ImGui::SetScrollHereY, (float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollFromPosX(float local_x, float center_x_ratio = 0.5f)", asFUNCTIONPR(ImGui::SetScrollFromPosX, (float, float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void SetScrollFromPosY(float local_y, float center_y_ratio = 0.5f)", asFUNCTIONPR(ImGui::SetScrollFromPosY, (float, float), void), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("void Text(const string& in text)", asFUNCTION(ImGui_Text_Wrapper), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("double GetTime()", asFUNCTION(ImGui::GetTime), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("int GetFrameCount()", asFUNCTION(ImGui::GetFrameCount), asCALL_CDECL);
  assert(r >= 0);
  r = engine->RegisterGlobalFunction("ImVec2 CalcTextSize(const string& in text, const int text_end = -1, const bool hide_text_after_double_hash = false, const float wrap_width = -1.0f)",
                                     asFUNCTION(ImGui_CalcTextSize_Wrapper_Generic), asCALL_GENERIC);
  assert(r >= 0);

  engine->SetDefaultNamespace(def);
}