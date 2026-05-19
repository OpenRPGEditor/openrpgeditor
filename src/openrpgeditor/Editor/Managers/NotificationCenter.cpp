#include "Editor/Managers/NotificationCenter.hpp"

#include "IconsFontAwesome6.h"
#include <orei18n.hpp>

INotification::INotification(const std::string_view title)
: m_title(title) {
  NotificationCenter::instance().addNotification(this);
}

INotification::~INotification() { NotificationCenter::instance().removeNotification(this); }

void SimpleNotification::draw() { ImGui::Text("%s", m_text.c_str()); }

void ProgressNotification::draw() { ImGui::ProgressBar(progress(), ImVec2(-FLT_MIN, 0), m_text.c_str()); }

void NotificationCenter::draw() {
  const float iconSize = ImGui::GetFrameHeightWithSpacing();
  if (!m_isOpen) {
    return;
  }
  // Quick and dirty imp to test
  // TODO: Properly implement this

  if (ImGui::Begin(trNOOP("Notification Center"), &m_isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar)) {
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows) && !ImGui::IsWindowFocused() && !m_isPinned) {
      m_isOpen = false;
      ImGui::End();
      return;
    }

    ImGui::BeginVertical("##notification_center_body", ImVec2(-1, -1), 0.f);
    {
      ImGui::BeginHorizontal("##notification_center_top", ImVec2(ImGui::GetContentRegionAvail().x, -1), 0.f);
      {
        ImGui::TextUnformatted(trNOOP("Notification Center"));
        ImGui::Spring();
        auto buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        if (!m_isPinned) {
          buttonColor.w = 0.f;
        }
        ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
        if (ImGui::Button(ICON_FA_THUMBTACK, {iconSize, iconSize})) {
          m_isPinned ^= 1;
        }
        ImGui::PopStyleColor();
        ImGui::SetItemTooltip("%s", trNOOP("Pin the Notification Center to prevent it from closing"));
      }
      ImGui::EndHorizontal();
      for (const auto& notif : m_notifications) {
        bool remove = false;
        ImGui::BeginChild(ImGui::GetID(notif), ImVec2(-1, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
        {
          ImGui::BeginVertical(std::format("##notification_layout_{}", reinterpret_cast<uintptr_t>(notif)).c_str(), ImVec2(-1, -1), 0.f);
          {
            ImGui::BeginHorizontal(std::format("##notification_inner_layout_{}", reinterpret_cast<uintptr_t>(notif)).c_str(), ImVec2(-1, -1), 0.f);
            {
              ImGui::TextUnformatted(notif->title().data());
              ImGui::Spring();
              remove = ImGui::Button(ICON_FA_XMARK, {iconSize, iconSize});
            }
            ImGui::EndHorizontal();
            ImGui::Separator();
            notif->draw();
          }
          ImGui::EndVertical();
        }
        ImGui::EndChild();

        // If we're a timed notification check if we've expired
        if (const auto* t = dynamic_cast<TimedNotification*>(notif); t && t->isExpired()) {
          remove = true;
        }
        if (remove) {
          removeNotification(notif);
          break;
        }
      }
    }
    ImGui::EndVertical();
  }

  ImGui::End();
}