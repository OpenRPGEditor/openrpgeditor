#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Animations.hpp"

#include "imgui.h"

DBAnimationsTab::DBAnimationsTab(Animations& animations, DatabaseEditor* parent) : IDBEditorTab(parent), m_animations(animations) { m_selectedAnimation = m_animations.animation(1); }

void DBAnimationsTab::draw() {
  ImGui::BeginChild("#orpg_animations_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Animations");
        ImGui::BeginChild("##orpg_animations_editor_animations_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& animation : m_animations.animations()) {
              if (animation.id() == 0) {
                continue;
              }
              if (ImGui::Selectable(Database::instance()->animationNameAndId(animation.id()).c_str(), &animation == m_selectedAnimation) ||
                  (ImGui::IsItemFocused() && m_selectedAnimation != &animation)) {
                m_selectedAnimation = &animation;
                // m_traitsEditor.setTraits(&m_selectedClass->traits);
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Animations %i", m_animations.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxAnimations = m_animations.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_animations_editor_animations_animation_properties");
    { ImGui::EndChild(); }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin("Change Max Animations", &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            "Specify an amount to resize the animation list to\n"
            "This cannot be undone!");
        ImGui::InputInt("##value_input", &m_editMaxAnimations);
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

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text("Are you sure?");
          if (ImGui::Button("Yes")) {
            const int tmpId = m_selectedAnimation->id();
            m_animations.resize(m_editMaxAnimations);
            m_selectedAnimation = m_animations.animation(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button("Cancel")) {
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}
