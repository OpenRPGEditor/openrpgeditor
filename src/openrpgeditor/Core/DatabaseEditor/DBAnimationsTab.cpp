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
        ImGui::BeginChild("##orpg_animations_animation_panel_topleft", ImVec2{ImGui::GetContentRegionMax().x, ImGui::GetContentRegionMax().y / 5});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedAnimation->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_animations_editor_name", "Name:", name, 4096, 200)) {
                m_selectedAnimation->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text("Images:");
              ImGui::PushID("##orpg_animations_name1_selection");
              if (ImGui::Button(m_selectedAnimation->animation1Name().data(), ImVec2{200, 0})) {
                m_pickerSelection = 0;
                m_imagePicker->setImageInfo(m_selectedAnimation->animation1Name(), "");
                m_imagePicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::SameLine();
              ImGui::PushID("##orpg_animations_name2_selection");
              if (ImGui::Button(m_selectedAnimation->animation2Name().data(), ImVec2{200, 0})) {
                m_pickerSelection = 1;
                m_imagePicker->setImageInfo(m_selectedAnimation->animation2Name(), "");
                m_imagePicker->setOpen(true);
              }
              ImGui::PopID();
              ImGui::SameLine();
              ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40.f);
              ImGui::PushID("##orpg_animations_play");
              if (ImGui::Button("Play", ImVec2{150, 0})) {
                // Play Animation
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text("Frame:");
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - 50);
              ImGui::SliderInt("##orpg_animations_frames_slider", &m_frameCursor, 1, m_selectedAnimation->frames().size());
              ImGui::EndGroup();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();

        ImGui::BeginChild("##orpg_animations_animation_panel_middle", ImVec2{ImGui::GetContentRegionMax().x, 300});
        { ImGui::Dummy(ImVec2{ImGui::GetContentRegionMax().x, 300}); }
        ImGui::EndChild();

        ImGui::BeginChild("##orpg_animations_animation_panel_middleleft", ImVec2{ImGui::GetContentRegionMax().x / 2 - 50, 360});
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Sound Effects and Flash Timing");
            if (ImGui::BeginTable("##orpg_animations_se_flash_timings", 3,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY)) {
              ImGui::TableSetupColumn("Frame #");
              ImGui::TableSetupColumn("Sound Effect");
              ImGui::TableSetupColumn("Flash");
              ImGui::TableHeadersRow();

              int timingIndex{0};
              for (int i = 0; i < m_selectedAnimation->timings().size(); ++i) {
                ImGui::PushID(&m_selectedAnimation->timings().at(i));
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn()) {
                  if (ImGui::Selectable(std::format("#{:03}##timing_{}", m_selectedAnimation->timing(i).frame() + 1, timingIndex).c_str(), i == timingIndex,
                                        ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {}
                  // if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_selectedEffect == &effect && m_deletingEffect == nullptr) {}
                  timingIndex++;
                }
                if (ImGui::TableNextColumn()) {
                  ImGui::Text("%s", m_selectedAnimation->timing(i).se()->name().c_str());
                }
                if (ImGui::TableNextColumn()) {
                  if (m_selectedAnimation->timing(i).flashScope() == FlashScope::HideTarget) {
                    ImGui::Text("%s, %i frames", DecodeEnumName(m_selectedAnimation->timing(i).flashScope()), m_selectedAnimation->timing(i).frame());
                  } else {
                    int red = m_selectedAnimation->timing(i).flashColor().r();
                    int green = m_selectedAnimation->timing(i).flashColor().g();
                    int blue = m_selectedAnimation->timing(i).flashColor().b();
                    int intensity = m_selectedAnimation->timing(i).flashColor().intensity();

                    ImGui::Text("%s(%i,%i,%i,%i), %i frames", DecodeEnumName(m_selectedAnimation->timing(i).flashScope()).c_str(), red, green, blue, intensity, m_selectedAnimation->timing(i).frame());
                  }
                }
                ImGui::PopID();
              }

              ImGui::TableNextRow();
              /*
              if (ImGui::TableNextColumn()) {
                if (ImGui::Selectable("##effects_editor_effect_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    m_tempEffect = Effect();
                    m_isNewEntry = true;
                    m_updateTab = true;
                    m_tempEffectAssigned = false;
                    m_selectedEffect = &m_tempEffect;
                    ImGui::OpenPopup(EffectsEditorPopupId.data());
                  }
                }
              }
              */
              ImGui::EndTable();
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
        ImGui::BeginChild("##orpg_animations_animation_panel_middle_buttons", ImVec2{50, 320});
        {
          ImGui::Dummy(ImVec2{0, 140});
          ImGui::PushID("##orpg_animations_timings_add");
          if (ImGui::Button("+", ImVec2{50, 0})) {
            // Add timings
          }
          ImGui::PopID();
          ImGui::PushID("##orpg_animations_timings_remove");
          if (ImGui::Button("-", ImVec2{50, 0})) {
            // Remove timings
          }
          ImGui::PopID();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 8.f);
        ImGui::BeginChild("##orpg_animations_animation_panel_middleright", ImVec2{ImGui::GetContentRegionMax().x, 0});
        {
          ImGui::BeginGroup();
          {
            ImGui::BeginGroup();
            {
              ImGui::Text("Frame:");
              ImGui::EndGroup();
            }
            ImGui::BeginGroup();
            {
              ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 3.f);
              ImGui::Text(std::string("\t" + std::to_string(m_frameCursor)).c_str());
              ImGui::EndGroup();
            }
            ImGui::EndGroup();
          }
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::Text("Sound Effect:");
            ImGui::PushID("##orpg_animations_timings_soundeffect");
            if (ImGui::Button(m_timingSE == 0 ? "" : Database::instance()->system.sounds().at(m_timingSE).name().c_str(), ImVec2{200, 0})) {
              // Select SE
            }
            ImGui::PopID();
            ImGui::EndGroup();
          }
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Flash Type");
            if (ImGui::RadioButton("None##orpg_animations_timings_frametype_none", m_frameType == 0)) {
              m_frameType = 0;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Target##orpg_animations_timings_frametype_target", m_frameType == 1)) {
              m_frameType = 1;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Screen##orpg_animations_timings_frametype_screen", m_frameType == 2)) {
              m_frameType = 2;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Hide Target##orpg_animations_timings_frametype_hidetarget", m_frameType == 3)) {
              m_frameType = 3;
            }

            ImGui::EndGroup(); // End Group: Flash
          }
          ImGui::BeginGroup();
          {
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 5.f);
            ImGui::Text("Red:");
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
            ImGui::Text("Green:");
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 14.f);
            ImGui::Text("Blue:");
            ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 12.f);
            ImGui::Text("Intensity:");
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::SetNextItemWidth(150);
            ImGui::SliderInt("##orpg_animations_timings_red", &r, 0, 255, "", ImGuiSliderFlags_NoInput);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            if (ImGui::InputInt("##orpg_animations_timings_int_red", &r, 1, 100)) {
              if (r < 0)
                r = 0;
              if (r > 255)
                r = 255;
            }

            ImGui::SetNextItemWidth(150);
            ImGui::SliderInt("##orpg_animations_timings_green", &g, 0, 255, "", ImGuiSliderFlags_NoInput);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            if (ImGui::InputInt("##orpg_animations_timings_int_green", &g, 1, 100)) {
              if (g < 0)
                g = 0;
              if (g > 255)
                g = 255;
            }

            ImGui::SetNextItemWidth(150);
            ImGui::SliderInt("##orpg_animations_timings_blue", &b, 0, 255, "", ImGuiSliderFlags_NoInput);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            if (ImGui::InputInt("##orpg_animations_timings_int_blue", &b, 1, 100)) {
              if (b < 0)
                b = 0;
              if (b > 255)
                b = 255;
            }

            ImGui::SetNextItemWidth(150);
            ImGui::SliderInt("##orpg_animations_timings_gray", &m_intensity, 0, 255, "", ImGuiSliderFlags_NoInput);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(120);
            if (ImGui::InputInt("##orpg_animations_timings_int_gray", &m_intensity, 1, 100)) {
              if (m_intensity < 1)
                m_intensity = 1;
              if (m_intensity > 255)
                m_intensity = 255;
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();

          ImGui::ColorButton("##orpg_animations_timings_square", ImVec4{static_cast<float>(r * (1.0f / 255.0f)), static_cast<float>(g * (1.0f / 255.0f)), static_cast<float>(b * (1.0f / 255.0f)), 1},
                             0, ImVec2{100, 100});

          ImGui::SeparatorText("Duration");
          ImGui::SetNextItemWidth(100);
          if (ImGui::InputInt("##orpg_animations_timings_duration", &m_duration)) {
            if (m_duration < 1)
              m_duration = 1;
            if (m_duration > 999)
              m_duration = 999;
          }
          ImGui::SameLine();
          ImGui::Text("frames 1/60 sec");

          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
          ImGui::PushItemWidth(390);
          if (ImGui::BeginCombo("##orpg_animations_timings_presets", "Future presets go in this list")) {
            // TODO: Template system for tint screen
            ImGui::EndCombo();
          }
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
