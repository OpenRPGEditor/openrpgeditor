#include "Core/CommonUI/CharacterPicker.hpp"
#include <iostream>
#include "imgui.h"
#include "imgui_internal.h"
#include "Core/DPIHandler.hpp"
#include "Core/ResourceManager.hpp"

inline int roundUp(int numToRound, int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = numToRound % multiple;
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

int alignCoord(int value, int size) { return roundUp(value - (value % (static_cast<int>(size))), size); }

CharacterPicker::CharacterPicker(const PickerMode mode, const std::string_view sheetName, const int character,
                                 const int pattern, const Direction direction)
: IDialogController("Select an Image##character_picker")
, m_pickerMode(mode)
, m_characterIndex(character)
, m_checkerboardTexture(4096, 4096)
, m_pattern(pattern)
, m_direction(direction) {
  m_characterSheets = ResourceManager::instance()->getDirectoryContents("img/characters/", ".png");
  setCharacterInfo(sheetName, character, pattern, direction);
}

void CharacterPicker::setCharacterInfo(const std::string_view sheetName, const int character, const int pattern,
                                       const Direction direction) {
  m_characterSheet.emplace(sheetName);
  m_characterIndex = character;
  m_pattern = pattern;
  m_direction = direction;
  if (!sheetName.empty()) {
    bool found = false;
    for (int i = 0; i < m_characterSheets.size(); ++i) {
      if (!m_characterSheets[i].compare(sheetName)) {
        found = true;
        m_selectedSheet = i;
        break;
      }
    }
    if (!found) {
      m_selectedSheet = -1;
      return;
    }

    const float charX = static_cast<float>(
        (character % (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) *
        m_characterSheet->characterAtlasWidth());
    const float charY = static_cast<float>(
        (character / (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) *
        m_characterSheet->characterAtlasHeight());

    if (m_pickerMode == PickerMode::PatternAndDirection) {
      m_selectionX = charX + (pattern * m_characterSheet->characterWidth());
      m_selectionY = charY + (((static_cast<int>(m_direction) - 2) / 2) * m_characterSheet->characterHeight());
      m_selectionWidth = m_characterSheet->characterWidth();
      m_selectionHeight = m_characterSheet->characterHeight();
    } else {
      m_selectionX = charX;
      m_selectionY = charY;
      m_selectionWidth = m_characterSheet->characterAtlasWidth();
      m_selectionHeight = m_characterSheet->characterAtlasHeight();
    }
    // m_selectionX =
    //     std::clamp(m_selectionX, 0, m_characterSheet->texture().width() - m_characterSheet->characterWidth());
    // m_selectionY =
    //     std::clamp(m_selectionY, 0, m_characterSheet->texture().height() - m_characterSheet->characterHeight());
  } else {
    m_selectedSheet = -1;
  }
}

std::tuple<bool, bool> CharacterPicker::draw() {
  if (IsOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{894, 768} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open,
                             ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##character_picker_sheet_list",
                        ImVec2{App::DPIHandler::scale_value(200), App::DPIHandler::scale_value(768)},
                        ImGuiChildFlags_None, ImGuiWindowFlags_NoBackground);
      {
        if (ImGui::BeginTable("##character_picker.characterlist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable("(None)", m_selectedSheet < 0,
                                ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedSheet = -1;
            m_characterSheet.reset();
          }
          for (int i = 0; i < m_characterSheets.size(); ++i) {
            const auto& sheet = m_characterSheets[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(sheet.c_str(), m_selectedSheet == i,
                                  ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              if (m_selectedSheet != i) {
                m_characterSheet.emplace(sheet);
                if (m_pickerMode == PickerMode::Character) {
                  m_selectionWidth = m_characterSheet->characterAtlasWidth();
                  m_selectionHeight = m_characterSheet->characterAtlasHeight();
                } else {
                  m_selectionWidth = m_characterSheet->characterWidth();
                  m_selectionHeight = m_characterSheet->characterHeight();
                }
                m_selectionX = 0;
                m_selectionY = 0;
              }
              m_selectedSheet = i;
            }
            if (ImGui::IsItemHovered()) {
              ImGui::SetTooltip("%s", sheet.c_str());
            }
          }
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##character_picker_sheet_panel",
                        ImVec2{App::DPIHandler::scale_value(894), App::DPIHandler::scale_value(784)},
                        ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
      {
        auto win = ImGui::GetCurrentWindow();
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
          auto mouseCursor = ImGui::GetMousePos();
          mouseCursor -= win->ContentRegionRect.Min;
          int x = alignCoord(mouseCursor.x / App::DPIHandler::get_ui_scale(), m_selectionWidth);
          int y = alignCoord(mouseCursor.y / App::DPIHandler::get_ui_scale(), m_selectionHeight);
          if ((x >= 0 && x < m_characterSheet->texture().width()) &&
              (y >= 0 && y < m_characterSheet->texture().height())) {
            m_selectionX = x;
            m_selectionY = y;
            if (m_pickerMode == PickerMode::Character) {
              m_selectionX = std::clamp(m_selectionX, 0,
                                        m_characterSheet->texture().width() - m_characterSheet->characterAtlasWidth());
              m_selectionY = std::clamp(
                  m_selectionY, 0, m_characterSheet->texture().height() - m_characterSheet->characterAtlasHeight());
            } else {
              m_selectionX =
                  std::clamp(m_selectionX, 0, m_characterSheet->texture().width() - m_characterSheet->characterWidth());
              m_selectionY = std::clamp(m_selectionY, 0,
                                        m_characterSheet->texture().height() - m_characterSheet->characterHeight());
            }

            x = m_selectionX / m_characterSheet->characterAtlasWidth();
            y = m_selectionY / m_characterSheet->characterAtlasHeight();
            m_pattern =
                (m_selectionX / m_characterSheet->characterWidth()) % m_characterSheet->patternCountForCharacter();
            const int direction =
                (m_selectionY / m_characterSheet->characterHeight()) % m_characterSheet->directionCountForCharacter();
            m_direction = static_cast<Direction>((direction * 2) + 2);

            m_characterIndex =
                (y * (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) + x;
          }
        }
        if (m_characterSheet) {
          ImGui::Dummy(ImVec2{static_cast<float>(m_characterSheet->texture().width()),
                              static_cast<float>(m_characterSheet->texture().height())} *
                       App::DPIHandler::get_ui_scale());
          win->DrawList->AddImage(
              m_checkerboardTexture.get(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
              win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_characterSheet->texture().width()),
                                                   static_cast<float>(m_characterSheet->texture().height())} *
                                            App::DPIHandler::get_ui_scale()),
              ImVec2{0.f, 0.f},
              {m_characterSheet->texture().width() / 4096.f, m_characterSheet->texture().height() / 4096.f});
          win->DrawList->AddImage(m_characterSheet->texture().get(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                  win->ContentRegionRect.Min +
                                      (ImVec2{static_cast<float>(m_characterSheet->texture().width()),
                                              static_cast<float>(m_characterSheet->texture().height())} *
                                       App::DPIHandler::get_ui_scale()));

          win->DrawList->AddRect(win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)} *
                                      App::DPIHandler::get_ui_scale()),
                                 win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(m_selectionWidth),
                                             static_cast<float>(m_selectionY) + static_cast<float>(m_selectionHeight)} *
                                      App::DPIHandler::get_ui_scale()),
                                 0xFF000000, 0.f, 0, 7.f);
          win->DrawList->AddRect(win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)} *
                                      App::DPIHandler::get_ui_scale()),
                                 win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(m_selectionWidth),
                                             static_cast<float>(m_selectionY) + static_cast<float>(m_selectionHeight)} *
                                      App::DPIHandler::get_ui_scale()),
                                 0xFFFFFFFF, 0.f, 0, 3.f);
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_confirmed = false;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}
