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
  if (!m_isOpen) {
    return;
  }
  // Quick and dirty imp to test
  // TODO: Properly implement this

  if (ImGui::Begin(trNOOP("Notification Center"), &m_isOpen, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse)) {
    for (const auto& notif : m_notifications) {
      bool remove = false;
      ImGui::BeginChild(ImGui::GetID(notif), ImVec2(-1, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle);
      {
        ImGui::BeginVertical(std::format("##notification_layout_{}", reinterpret_cast<uintptr_t>(notif)).c_str(), ImVec2(-1, -1), 0.f);
        {
          ImGui::BeginHorizontal(std::format("##notification_layout_{}", reinterpret_cast<uintptr_t>(notif)).c_str(), ImVec2(-1, -1), 0.f);
          {
            ImGui::Text("%s", notif->title().data());
            ImGui::Spring();

            //ImGui::PushStyleVarY(ImGuiStyleVar_ItemInnerSpacing, ImGui::GetStyle().ItemInnerSpacing.x);
            remove = ImGui::Button(ICON_FA_XMARK);
            //ImGui::PopStyleVar();
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

  ImGui::End();
}