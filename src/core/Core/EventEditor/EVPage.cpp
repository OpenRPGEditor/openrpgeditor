#include "Core/EventEditor/EVPage.hpp"
#include "Core/EventEditor.hpp"
#include "Core/Project.hpp"

#include <string>
#include <format>

#include "imgui.h"
#include "Core/Application.hpp"
#include "Core/DPIHandler.hpp"
#include "Core/ImGuiParsedText.hpp"
#include "Core/ImGuiUtils.hpp"
#include "Database/EventPage.hpp"

EVPage::EVPage(EventEditor* parent, EventPage* page)
: m_parent(parent), m_page(page), m_commandEditor(parent->project()) {
  m_commandEditor.setCommands(&m_page->list);
}

std::tuple<bool, bool> EVPage::draw(bool canDelete, int index) {
  bool open = true;
  bool* p_open = canDelete ? &open : nullptr;
  std::string title = m_page->name.empty() ? std::format("Page {}", std::to_string(index + 1)) : m_page->name;
  bool selected = ImGui::BeginTabItem(title.c_str(), p_open);
  if (selected) {
    ImGui::SetNextItemWidth(App::DPIHandler::scale_value(250));
    ImGui::BeginGroup();
    {
      char nameBuf[4096];
      strncpy(nameBuf, m_page->name.c_str(), 4096);
      if (ImGui::LabelOverLineEdit(
              "##event_page_name_edit", "Page Name:", nameBuf, 4096, App::DPIHandler::scale_value(150),
              "Page names are an OpenRPGMaker addition and will not be viewable in RPG Maker MV/MZ")) {
        m_page->name = nameBuf;
      }

      ImGui::BeginGroup();
      { ImGui::SeparatorText("Conditions"); }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Image"); }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Autonomous Movement"); }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      { ImGui::SeparatorText("Options"); }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImGui::BeginGroup();
        { ImGui::SeparatorText("Priority"); }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        { ImGui::SeparatorText("Trigger"); }
        ImGui::EndGroup();
      }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_event_page_commands");
    { m_commandEditor.draw(); }
    ImGui::EndChild();
    ImGui::EndTabItem();
  }

  open ^= 1;
  return std::make_tuple(open, selected);
}

void EVPage::clearPage() {
  m_page->list.clear();
  m_page->list.emplace_back(new EventDummy())->indent = 0;
}
