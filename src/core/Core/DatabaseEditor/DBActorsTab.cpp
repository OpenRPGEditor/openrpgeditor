#include "Core/DatabaseEditor/DBActorsTab.hpp"
#include "Database/Actors.hpp"

#include "imgui.h"

DBActorsTab::DBActorsTab(Actors& actors) : m_actors(actors) { m_selectedActor = &m_actors.m_actors[1]; }

void DBActorsTab::draw() {
  ImGui::BeginChild("##orpg_actors_editor");
  {
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{300.f, 0});
    {
      ImGui::BeginChild("##orpg_actors_editor_actor_list", ImVec2{0, ImGui::GetContentRegionMax().y - 64});
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
      ImGui::EndChild();

      ImGui::Text("Max Actors %zu", m_actors.m_actors.size() - 1);
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##orpg_actors_editor_actors_actor_properties");
      {
        if (m_selectedActor) {
          static char buff[4096];

          ImGui::BeginChild("##actor");
          {
            ImGui::Text("id: %i", m_selectedActor->id);
            ImGui::Text("Battler Name: %s", m_selectedActor->battlerName.c_str());
            ImGui::Text("Character Index: %i", m_selectedActor->characterIndex);
            ImGui::Text("Class ID: %i", m_selectedActor->classId);
            strncpy(buff, m_selectedActor->name.c_str(), 4095);
            if (ImGui::InputText("Name", buff, 4096)) {
              m_selectedActor->name = std::string(buff);
            }
            ImGui::BeginChild("##equips");
            {
              ImGui::Separator();
              ImGui::Text("Equips:");
              for (const auto& equip : m_selectedActor->equips) {
                ImGui::Text("%i", equip);
              }
              ImGui::Separator();
            }
            ImGui::EndChild();
          }
          ImGui::EndChild();
        }
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();
}