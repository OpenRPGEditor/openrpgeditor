#pragma once
// Based on and modified from
// https://gist.github.com/ddovod/be210315f285becc6b0e455b775286e1
#include <vector>
#include <string>
// #define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"


namespace ImGui {
void TextParsedV(const char* fmt, va_list args);
void TextParsed(const char* fmt, ...);
} // namespace ImGui
