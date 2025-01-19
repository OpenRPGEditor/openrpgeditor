#include "Core/DatabaseEditor/DBAnimationsTab.hpp"
#include "Core/Application.hpp"
#include "Database/Animations.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"

DBAnimationsTab::DBAnimationsTab(Animations& animations, DatabaseEditor* parent) : IDBEditorTab(parent), m_animations(animations) {

  m_selectedAnimation = m_animations.animation(1);

  m_selectedTimings = m_selectedAnimation->timings();
  m_colorPicker.setValues(255, 255, 255, 255);

  m_soundPicker.selectedAudio().nameModified().connect<&DBAnimationsTab::onNameModified>(this);
  m_soundPicker.selectedAudio().volumeModified().connect<&DBAnimationsTab::onVolModified>(this);
  m_soundPicker.selectedAudio().panModified().connect<&DBAnimationsTab::onPanModified>(this);
  m_soundPicker.selectedAudio().pitchModified().connect<&DBAnimationsTab::onPitchModified>(this);
}

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
  m_soundPicker.draw();

  ImGui::BeginChild("#orpg_animations_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_actors_editor_actors", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, 0, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText(trNOOP("Animations"));
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
                m_frameCursor = 0; // Reset cursor on animation change

                m_selectedTimings = m_selectedAnimation->timings();
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SeparatorText(trFormat("Max Animations {}", m_animations.count()).c_str());
        if (ImGui::Button(trNOOP("Change Max"), ImVec2{ImGui::GetContentRegionMax().x - 8, 0})) {
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
            ImGui::SeparatorText(trNOOP("General Settings"));
            // Name
            ImGui::BeginGroup();
            {
              char name[4096];
              strncpy(name, m_selectedAnimation->name().c_str(), 4096);
              if (ImGui::LabelOverLineEdit("##orpg_animations_editor_name", trNOOP("Name:"), name, 4096, 200)) {
                m_selectedAnimation->setName(name);
              }
            }
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 6.f);
            ImGui::BeginGroup();
            {
              ImGui::Text(trNOOP("Images:"));
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
              if (ImGui::Button(trNOOP("Play"), ImVec2{150, 0})) {
                // Play Animation
              }
              ImGui::PopID();
            }
            ImGui::EndGroup();
            ImGui::BeginGroup();
            {
              ImGui::Text(trNOOP("Frame:"));
              ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - 50);
              ImGui::SliderInt("##orpg_animations_frames_slider", m_isApplyingChanges ? &m_selectedFrameNumber : &m_frameCursor, 1, m_selectedAnimation->frames().size());
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
            ImGui::SeparatorText(trNOOP("Sound Effects and Flash Timing"));
            if (ImGui::BeginTable("##orpg_animations_se_flash_timings", 3,
                                  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollY)) {
              ImGui::TableSetupColumn(trNOOP("Frame #"));
              ImGui::TableSetupColumn(trNOOP("Sound Effect"));
              ImGui::TableSetupColumn(trNOOP("Flash"));
              ImGui::TableHeadersRow();

              int timingIndex{0};
              for (int i = 0; i < m_selectedAnimation->timings().size(); ++i) {
                ImGui::PushID(&m_selectedAnimation->timings().at(i));
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn()) {
                  if (ImGui::Selectable(
                          std::format("#{:03}##timing_{}", m_isApplyingChanges && i == m_selectedTiming ? m_selectedFrameNumber + 1 : m_selectedAnimation->timing(i).frame() + 1, timingIndex).c_str(),
                          i == m_selectedTiming, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                    if (!m_isApplyingChanges) {
                      m_selectedTiming = timingIndex;
                      if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                        m_isApplyingChanges = true;
                        m_selectedAudio = *m_selectedAnimation->timing(m_selectedTiming).se();
                        m_selectedColor = m_selectedAnimation->timing(m_selectedTiming).flashColor();
                        m_selectedScope = static_cast<int>(m_selectedAnimation->timing(m_selectedTiming).flashScope());
                        m_frameType = m_selectedScope;
                        m_selectedDuration = m_selectedAnimation->timing(m_selectedTiming).flashDuration();
                        m_selectedFrameNumber = m_selectedAnimation->timing(m_selectedTiming).frame();

                        m_colorPicker.setValues(m_selectedColor.r(), m_selectedColor.g(), m_selectedColor.b(), m_selectedColor.intensity());
                      }
                    }
                  }
                  timingIndex++;
                }
                if (ImGui::TableNextColumn()) {
                  if (m_isApplyingChanges && m_selectedTiming == i) {
                    ImGui::Text("%s", m_selectedAudio.name().c_str());
                  } else {
                    ImGui::Text("%s", m_selectedAnimation->timing(i).se()->name().c_str());
                  }
                }
                if (ImGui::TableNextColumn()) {
                  if (m_selectedAnimation->timing(i).flashScope() == FlashScope::HideTarget) {
                    if (m_isApplyingChanges && m_selectedTiming == i) {
                      ImGui::Text("%s, %i frames", DecodeEnumName(static_cast<FlashScope>(m_selectedScope)).c_str(), m_selectedFrameNumber);
                    } else {
                      ImGui::Text("%s, %i frames", DecodeEnumName(m_selectedAnimation->timing(i).flashScope()).c_str(), m_selectedAnimation->timing(i).frame());
                    }
                  } else {
                    int red = m_selectedAnimation->timing(i).flashColor().r();
                    int green = m_selectedAnimation->timing(i).flashColor().g();
                    int blue = m_selectedAnimation->timing(i).flashColor().b();
                    int intensity = m_selectedAnimation->timing(i).flashColor().intensity();

                    if (m_isApplyingChanges && m_selectedTiming == i) {
                      ImGui::Text("%s(%i,%i,%i,%i), %i frames", DecodeEnumName(static_cast<FlashScope>(m_selectedScope)).c_str(), m_selectedColor.r(), m_selectedColor.g(), m_selectedColor.b(),
                                  m_selectedColor.intensity(), m_selectedDuration);
                    } else {
                      ImGui::Text("%s(%i,%i,%i,%i), %i frames", DecodeEnumName(m_selectedAnimation->timing(i).flashScope()).c_str(), red, green, blue, intensity,
                                  m_selectedAnimation->timing(i).flashDuration());
                    }
                  }
                }
                ImGui::PopID();
              }

              ImGui::TableNextRow();
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
          ImGui::BeginDisabled(m_isApplyingChanges == true);
          ImGui::PushID("##orpg_animations_timings_add");
          if (ImGui::Button("+", ImVec2{50, 0})) {
            Animation::Timing timing;
            Audio SE;
            Animation::Color color;

            SE.setName("");
            SE.setVolume(100);
            SE.setPan(50);
            SE.setPitch(100);

            color.setR(255);
            color.setG(255);
            color.setB(255);
            color.setIntensity(255);

            timing.setFrame(m_frameCursor - 1);
            timing.setSe(SE);
            timing.setFlashColor(color);
            timing.setFlashScope(static_cast<FlashScope>(m_frameType));
            timing.setFlashDuration(m_duration);

            m_selectedTimings.emplace_back(timing);

            std::sort(m_selectedTimings.begin(), m_selectedTimings.end(), [](const auto& a, const auto& b) { return a.frame() < b.frame(); });

            m_selectedAnimation->setTimings(m_selectedTimings);
          }
          ImGui::PopID();
          ImGui::EndDisabled();
          ImGui::BeginDisabled(m_selectedAnimation->timings().size() < 1 || m_isApplyingChanges == true);
          ImGui::PushID("##orpg_animations_timings_remove");
          if (ImGui::Button("-", ImVec2{50, 0})) {
            m_selectedTimings.erase(m_selectedTimings.begin() + m_selectedTiming, m_selectedTimings.begin() + m_selectedTiming + 1);
            m_selectedAnimation->setTimings(m_selectedTimings);
            if (m_selectedTiming > 0) {
              m_selectedTiming--;
            }
          }
          ImGui::PopID();
          ImGui::EndDisabled();
          ImGui::BeginDisabled(m_isApplyingChanges == false);
          ImGui::PushID("##orpg_animations_timings_apply");
          if (ImGui::Button("✓", ImVec2{50, 0})) {
            m_selectedAnimation->setTiming(m_selectedTiming, getTiming(m_selectedFrameNumber, m_selectedAudio, m_selectedColor, static_cast<FlashScope>(m_selectedScope), m_selectedDuration));
            m_isApplyingChanges = false;
          }
          ImGui::PopID();
          ImGui::EndDisabled();
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
              ImGui::Text(trNOOP("Frame:"));
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
            ImGui::Text(trNOOP("Sound Effect:"));
            ImGui::PushID("##orpg_animations_timings_soundeffect");
            if (ImGui::Button(m_isApplyingChanges ? m_selectedAnimation->timing(m_selectedTiming).se()->name().c_str() : m_selectedAudio.name().c_str(), ImVec2{200, 0})) {
              m_soundPicker.show(SEType::SE);
            }
            ImGui::PopID();
            ImGui::EndGroup();
          }
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Flash Type");
            if (ImGui::RadioButton(trNOOP("None"), m_isApplyingChanges ? m_selectedScope == 0 : m_frameType == 0)) {

              m_selectedScope = 0;
              m_frameType = 0;
              if (m_isApplyingChanges) {
              } else {
              }
            }
            ImGui::SameLine();
            if (ImGui::RadioButton(trNOOP("Target"), m_isApplyingChanges ? m_selectedScope == 1 : m_frameType == 1)) {

              m_selectedScope = 1;
              m_frameType = 1;
              if (m_isApplyingChanges) {
              } else {
              }
            }
            ImGui::SameLine();
            if (ImGui::RadioButton(trNOOP("Screen"), m_isApplyingChanges ? m_selectedScope == 2 : m_frameType == 2)) {

              m_selectedScope = 2;
              m_frameType = 2;
              if (m_isApplyingChanges) {
              } else {
              }
            }
            ImGui::SameLine();
            if (ImGui::RadioButton(trNOOP("Hide Target"), m_isApplyingChanges ? m_selectedScope == 3 : m_frameType == 3)) {

              m_selectedScope = 3;
              m_frameType = 3;
              if (m_isApplyingChanges) {
              } else {
              }
            }

            ImGui::EndGroup(); // End Group: Flash
          }
          ImGui::BeginDisabled((m_frameType == static_cast<int>(FlashScope::None) || m_frameType == static_cast<int>(FlashScope::Hide_Target)) && m_isApplyingChanges == false);

          // Color Picker
          m_colorPicker.draw();
          ImGui::SeparatorText(trNOOP("Duration"));
          ImGui::SetNextItemWidth(100);
          if (ImGui::InputInt("##orpg_animations_timings_duration", m_isApplyingChanges ? &m_selectedDuration : &m_duration)) {

            if (m_isApplyingChanges) {
              m_selectedDuration = std::clamp(m_selectedDuration, 1, 255);
            } else {
              m_duration = std::clamp(m_duration, 1, 255);
            }
          }
          ImGui::SameLine();
          ImGui::Text(trNOOP("frames 1/60 sec"));

          ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
          ImGui::PushItemWidth(390);
          if (ImGui::BeginCombo("##orpg_animations_timings_presets", "Future presets go in this list")) {
            // TODO: Template system for tint screen
            ImGui::EndCombo();
          }
          ImGui::EndDisabled();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();

    if (m_changeIntDialogOpen) {
      if (ImGui::Begin(trNOOP("Change Max Animations"), &m_changeIntDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text(
            trNOOP("Specify an amount to resize the animation list to\n"
                   "This cannot be undone!"));
        ImGui::InputInt("##value_input", &m_editMaxAnimations);
        ImGui::SameLine();
        if (ImGui::Button(trNOOP("Accept"))) {
          m_changeConfirmDialogOpen = true;
        }
        ImGui::SameLine();
        if (ImGui::Button(trNOOP("Cancel"))) {
          m_changeIntDialogOpen = false;
        }
      }
      ImGui::End();

      if (m_changeConfirmDialogOpen) {
        if (ImGui::Begin(trNOOP("Confirm Change"), &m_changeConfirmDialogOpen,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
          ImGui::Text(trNOOP("Are you sure?"));
          if (ImGui::Button(trNOOP("Yes"))) {
            const int tmpId = m_selectedAnimation->id();
            m_animations.resize(m_editMaxAnimations);
            m_selectedAnimation = m_animations.animation(tmpId);
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
          ImGui::SameLine();
          if (ImGui::Button(trNOOP("Cancel"))) {
            m_changeIntDialogOpen = false;
            m_changeConfirmDialogOpen = false;
          }
        }
        ImGui::End();
      }
    }
  }
}
Animation::Timing DBAnimationsTab::getTiming(int frame, Audio audio, Animation::Color color, FlashScope scope, int duration) {
  Animation::Timing m_timing;

  color.setR(m_colorPicker.r());
  color.setG(m_colorPicker.g());
  color.setB(m_colorPicker.b());
  color.setIntensity(m_colorPicker.intensity());

  m_timing.setFrame(frame);
  m_timing.setSe(audio);
  m_timing.setFlashColor(color);
  m_timing.setFlashScope(scope);
  m_timing.setFlashDuration(duration);
  return m_timing;
}
