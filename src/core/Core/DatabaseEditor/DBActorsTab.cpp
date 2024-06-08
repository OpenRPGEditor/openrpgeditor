#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"
#include "Core/DatabaseEditor.hpp"
#include "Core/ImGuiUtils.hpp"

DBActorsTab::DBActorsTab(Actors& actors, DatabaseEditor* parent) : IDBEditorTab(parent), m_actors(actors) {
  m_selectedActor = &m_actors.m_actors[1];
  m_maxActors = m_actors.m_actors.size() - 1;
}

void DBActorsTab::draw() {
  ImGui::BeginChild("##orpg_actors_editor");
  {
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{300.f, 0});
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Actors");
        ImGui::BeginChild("##orpg_actors_editor_actor_list", ImVec2{0, ImGui::GetContentRegionMax().y - 96});
        {
          ImGui::BeginGroup();
          {
            for (int i = 1; i < m_actors.m_actors.size(); ++i) {
              Actor& actor = m_actors.m_actors[i];
              std::string id = "##orpg_actor_editor_unnamed_actor_" + std::to_string(actor.id);
              ImGui::PushID(id.c_str());
              if (ImGui::Selectable(actor.name.empty() ? id.c_str() : actor.name.c_str(), &actor == m_selectedActor)) {
                m_selectedActor = &actor;
              }
              ImGui::PopID();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::Text("Max Actors %zu", m_actors.m_actors.size() - 1);
        ImGui::SameLine();
        if (ImGui::Button("Change Max")) {
          m_changeIntDialogOpen = true;
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_actors_editor_actors_actor_properties");
    {
      if (m_selectedActor) {
        ImGui::BeginChild("##orpg_actors_actor_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            char name[4096];
            strncpy(name, m_selectedActor->name.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_name", "Name:", name, 4096,
                                         (ImGui::GetContentRegionMax().x / 2) - 16)) {
              m_selectedActor->name = name;
            }
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            strncpy(name, m_selectedActor->nickname.c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_actors_editor_actors_actor_nickname", "Nickname:", name, 4096,
                                         (ImGui::GetContentRegionMax().x / 2) - 16)) {
              m_selectedActor->nickname = name;
            }
            ImGui::BeginGroup();
            {
              ImGui::Text("Class:");
              Classes classes = m_parent->classes();
              auto cls = classes.classType(m_selectedActor->classId);
              ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 16);
              char buf[4096];
              snprintf(buf, 4096, "%04i %s", cls->id, cls->name.c_str());
              if (ImGui::BeginCombo("##orpg_actors_editor_class_combo", buf)) {
                for (const auto& c : classes.classes()) {
                  snprintf(buf, 4096, "%04i %s", c.id, c.name.c_str());
                  if (ImGui::Selectable(buf, c.id == m_selectedActor->classId)) {
                    m_selectedActor->classId = c.id;
                  }
                }
                ImGui::EndCombo();
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Initial Level:");
              ImGui::SetNextItemWidth((ImGui::GetCursorPosX() / 2) - 16);
              ImGui::InputInt("##orpg_actors_initial_level_edit", &m_selectedActor->initialLevel);
              m_selectedActor->initialLevel = std::clamp(m_selectedActor->initialLevel, 1, 99);
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Max Level:");
              ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 16);
              ImGui::InputInt("##orpg_actors_max_level_edit", &m_selectedActor->maxLevel);
              m_selectedActor->maxLevel = std::clamp(m_selectedActor->maxLevel, 1, 99);
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Profile:");
              char profile[8192];
              strncpy(profile, m_selectedActor->profile.c_str(), IM_ARRAYSIZE(profile));
              if (ImGui::InputTextMultiline("##orpg_actors_profile", profile, IM_ARRAYSIZE(profile),
                                            ImVec2{ImGui::GetContentRegionMax().x - 16, 0})) {
                m_selectedActor->profile = profile;
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Images");
            ImGui::BeginGroup();
            {
              ImGui::Text("Face:");
              ImGui::Dummy(ImVec2{96, 96});
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("Character:");
              ImGui::Dummy(ImVec2{96, 96});
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
            ImGui::BeginGroup();
            {
              ImGui::Text("[SV] Battler:");
              ImGui::Dummy(ImVec2{96, 96});
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Initial Equipment");
            ImGui::Dummy({100, 100});
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_actors_actor_panel_right");
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Traits");
            ImGui::Dummy(ImVec2{100, 1000});
          }
          ImGui::EndGroup();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedActor->note.c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_actors_note", note, IM_ARRAYSIZE(note),
                                          ImVec2{ImGui::GetContentRegionMax().x - 16, 0})) {
              m_selectedActor->note = note;
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  if (m_changeIntDialogOpen) {
    if (ImGui::Begin("Change Max Actors", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal |
                         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the actor list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_maxActors);
      ImGui::SameLine();
      if (ImGui::Button("Accept")) {
        m_changeConfirmDialogOpen = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_changeIntDialogOpen = false;
      }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImGui::GetWindowSize() / 2);
    if (m_changeConfirmDialogOpen) {
      if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          m_actors.m_actors.resize(m_maxActors + 1);
          m_changeIntDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}