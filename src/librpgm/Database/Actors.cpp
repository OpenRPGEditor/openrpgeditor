#include "Actors.hpp"

#include <fstream>

#include <imgui.h>

using json = nlohmann::json;

void Actor::draw() {}

Actors::Actors() : m_isOpen(true) {}

Actors Actors::load(std::string_view filename) {
  std::ifstream file(filename.data());
  json data = json::parse(file);
  Actors actors;
  actors.m_actors.reserve(data.size());

  for (const auto& [_, value] : data.items()) {
    if (value == nullptr) {
      continue;
    }

    Actor& actor = actors.m_actors.emplace_back();
    value.get_to(actor);
  }
  return actors;
}

bool Actors::serialize(std::string_view filename) {
  std::ofstream file(filename.data());
  json data;

  for (const Actor& actor : m_actors) {
    data.push_back(actor);
  }

  file << data;
  return true;
}

void Actors::draw() {
  if (!m_isOpen) {
    return;
  }

  if (ImGui::Begin("Actors", &m_isOpen)) {
    ImGui::Columns(3);
    for (auto& actor : m_actors) {
      if (actor.id == 0) {
        continue; // Skip the first actor, this is a test actor and is a duplicate of the designated player character
      }
      if (ImGui::Selectable(actor.name.c_str(), &actor == &*m_selectedActor)) {
        m_selectedActor = actor;
      }
    }

    ImGui::NextColumn();
    if (m_selectedActor) {
      static char buff[4096];

      if (ImGui::BeginChild("##actor")) {
        ImGui::Text("id: %i", m_selectedActor->id);
        ImGui::Text("Battler Name: %s", m_selectedActor->battlerName.c_str());
        ImGui::Text("Character Index: %i", m_selectedActor->characterIndex);
        ImGui::Text("Class ID: %i", m_selectedActor->classId);
        strncpy(buff, m_selectedActor->name.c_str(), 4095);
        if (ImGui::InputText("Name", buff, 4096)) {
          m_selectedActor->name = std::string(buff);
        }
        if (ImGui::BeginChild("##equips")) {
          ImGui::Separator();
          ImGui::Text("Equips:");
          for (const auto& equip : m_selectedActor->equips) {
            ImGui::Text("%i", equip);
          }
          ImGui::Separator();
          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
    } else {
      ImGui::Text("Nothing to see here folks!");
    }
    ImGui::End();
  }
}
