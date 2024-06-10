#pragma once

#include "imgui.h"

namespace ImGui {
bool ParseColor(const char* s, ImU32* col, int* skipChars);
}