#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Animations.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

DBAnimationsTab::DBAnimationsTab(Animations& animations, DatabaseEditor* parent) : IDBEditorTab(parent), m_animations(animations) { m_selectedAnimation = m_animations.animation(1); }

void DBAnimationsTab::draw() {
  if (m_selectedAnimation) {
    if (!m_imagePicker) {
      m_imagePicker.emplace(ImagePicker::PickerMode::Picture, "", "");
    }
  }
  if (const auto [closed, confirmed] = m_imagePicker->draw(); closed) {
    if (confirmed) {
      m_imagePicker->accept();
      if (m_pickerSelection == 0) {
        m_selectedAnimation->setAnimation1Name(m_imagePicker->selectedImage());
      } else {
        m_selectedAnimation->setAnimation2Name(m_imagePicker->selectedImage());
      }
    }
  }

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
    {
      if (m_selectedAnimation) {
        ImGui::BeginChild("##orpg_animations_animation_panel_topleft", ImVec2{ImGui::GetContentRegionMax().x / 4, ImGui::GetContentRegionMax().x / 4});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedAnimation->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_animations_editor_name", "Name:", name, 4096, ImGui::GetContentRegionMax().x / 2 - 16)) {
                m_selectedAnimation->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Images:");
              ImGui::PushID("##orpg_animations_name1_selection");
              if (ImGui::Button(m_selectedAnimation->animation1Name().data(), ImVec2{(170), 0})) {
                m_pickerSelection = 0;
                m_imagePicker->setImageInfo(m_selectedAnimation->animation1Name(), "");
                m_imagePicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::PushID("##orpg_animations_name2_selection");
              if (ImGui::Button(m_selectedAnimation->animation2Name().data(), ImVec2{(170), 0})) {
                m_pickerSelection = 1;
                m_imagePicker->setImageInfo(m_selectedAnimation->animation2Name(), "");
                m_imagePicker->setOpen(true);
              }
              ImGui::PopID();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
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
