#include "Core/FirstBootWizard/WelcomePage.hpp"

#include "imgui.h"
#include "imgui_internal.h"

void WelcomePage::draw() {
  ImGui::BeginGroup();
  ImGui::Text(
      "Welcome Open RPG Editor, the free Open Source\n"
      "drop-in replacement for RPG Maker MV/MZ.");
  ImGui::Text("License");
  static constexpr char license[] = R"(MIT License

Copyright (c) 2024 OpenRPGEditor Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)";
  const auto textSize = ImGui::CalcTextSize("ABCDEFGHIJK");
  ImGui::InputTextMultiline("##ore_license_text", const_cast<char*>(license), sizeof(license),
                            {ImGui::GetContentRegionMax().x - ImGui::GetStyle().FramePadding.x * 2,
                             ImGui::GetContentRegionMax().y - (ImGui::GetCursorPosY() + textSize.y + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.y)},
                            ImGuiInputTextFlags_ReadOnly);
  ImGui::EndGroup();
}
