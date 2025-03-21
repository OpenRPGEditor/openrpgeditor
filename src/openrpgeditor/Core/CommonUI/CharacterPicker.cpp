#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/ResourceManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"

#include <IconsFontAwesome6.h>
#include <iostream>

inline int roundUp(int numToRound, int multiple) {
  if (multiple == 0)
    return numToRound;

  int remainder = numToRound % multiple;
  if (remainder == 0)
    return numToRound;

  return numToRound + multiple - remainder;
}

int alignCoord(int value, int size) { return roundUp(value - (value % (static_cast<int>(size))), size); }

CharacterPicker::CharacterPicker(const PickerMode mode, const std::string_view sheetName, const int character, const int pattern, const Direction direction)
: IDialogController("Select an Image##character_picker"), m_pickerMode(mode), m_characterIndex(character), m_checkerboardTexture(4096, 4096), m_pattern(pattern), m_direction(direction) {
  m_charDir.emplace("img/characters/", ".png", static_cast<std::string>(sheetName));
  m_characterSheets = m_charDir.value().getDirectoryContents();
  m_folders = m_charDir.value().getDirectories();

  setCharacterInfo(sheetName, character, pattern, direction);

  // std::string charName = m_charDir->getFileName(static_cast<std::string>(sheetName));
  // for (int i = 0; i < m_characterSheets.size(); ++i) {
  //   if (charName == m_characterSheets[i]) {
  //     m_selectedSheet = i;
  //   }
  // }
}

void CharacterPicker::setCharacterInfo(const std::string_view sheetName, const int character, const int pattern, const Direction direction) {
  m_characterSheet.emplace(sheetName);
  m_characterIndex = character;
  m_pattern = pattern;
  m_direction = direction;

  std::string imageName = m_charDir.value().getFileName(static_cast<std::string>(sheetName));
  if (!imageName.empty()) {
    bool found = false;
    for (int i = 0; i < m_characterSheets.size(); ++i) {
      if (!m_characterSheets[i].compare(imageName)) {
        found = true;
        m_selectedSheet = i;
        break;
      }
    }
    if (!found) {
      m_selectedSheet = -1;
      return;
    }

    const float charX = static_cast<float>((character % (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) * m_characterSheet->characterAtlasWidth());
    const float charY = static_cast<float>((character / (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) * m_characterSheet->characterAtlasHeight());

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
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 3, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings)) {
    const auto calc = ImGui::CalcTextSize("OKCANCEL");
    ImGui::BeginChild("##top_child", {0, ImGui::GetContentRegionAvail().y - (calc.y + (ImGui::GetStyle().ItemSpacing.y * 3) + ImGui::GetStyle().FramePadding.y)});
    {
      ImGui::Text("Selected Sheet: %s", m_selectedSheet == -1 ? "(None)" : m_characterSheets.size() > 0 ? m_characterSheets[m_selectedSheet].c_str() : "(None)");
      ImGui::BeginChild("##character_picker_sheet_list", ImVec2{ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRS").x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y},
                        ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
      {
        if (ImGui::BeginTable("##character_picker.characterlist", 1)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          ImGui::BeginDisabled(m_charDir.value().isParentDirectory());
          if (ImGui::Selectable("\u21B0 ..", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_charDir.value().moveUp();
              m_characterSheets = m_charDir->getDirectoryContents();
              m_folders = m_charDir.value().getDirectories();
              m_characterSheet.reset();
              m_selectedSheet = -1;
              m_selectedFolder = -1;
            }
          }
          ImGui::EndDisabled();
          for (int i = 0; i < m_folders.size(); ++i) {
            const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folders[i]);
            if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              m_selectedFolder = i;
            }
            if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_charDir->setDirectory(i);
              m_folders = m_charDir.value().getDirectories();
              m_characterSheets = m_charDir.value().getDirectoryContents();
              m_selectedSheet = -1;
              m_selectedFolder = -1;
              m_characterSheet.reset();
            }
          }

          if (ImGui::Selectable("(None)", m_selectedSheet < 0, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedSheet = -1;
            m_characterSheet.reset();
          }
          if (m_selectedSheet == -1) {
            ImGui::SetItemDefaultFocus();
          }
          for (int i = 0; i < m_characterSheets.size(); ++i) {
            const auto& sheet = m_characterSheets[i];
            ImGui::TableNextColumn();
            if (ImGui::Selectable(std::format("{0}##sheet_{0}", sheet).c_str(), m_selectedSheet == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
              if (m_selectedSheet != i) {
                m_characterSheet.emplace(m_charDir.value().isParentDirectory() ? sheet : m_charDir.value().getPathPrefix() + '/' + sheet);
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
            if (m_selectedSheet == i && ImGui::IsWindowAppearing()) {
              ImGui::SetScrollHereY();
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
      ImGui::BeginChild("##character_picker_sheet_panel", ImVec2{894, 784}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
      {
        auto win = ImGui::GetCurrentWindow();
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
          auto mouseCursor = ImGui::GetMousePos();
          mouseCursor -= win->ContentRegionRect.Min;
          int x = alignCoord(mouseCursor.x, m_selectionWidth);
          int y = alignCoord(mouseCursor.y, m_selectionHeight);
          if ((x >= 0 && x < m_characterSheet->texture().width()) && (y >= 0 && y < m_characterSheet->texture().height())) {
            m_selectionX = x;
            m_selectionY = y;
            if (m_pickerMode == PickerMode::Character) {
              m_selectionX = std::clamp(m_selectionX, 0, m_characterSheet->texture().width() - m_characterSheet->characterAtlasWidth());
              m_selectionY = std::clamp(m_selectionY, 0, m_characterSheet->texture().height() - m_characterSheet->characterAtlasHeight());
            } else {
              m_selectionX = std::clamp(m_selectionX, 0, m_characterSheet->texture().width() - m_characterSheet->characterWidth());
              m_selectionY = std::clamp(m_selectionY, 0, m_characterSheet->texture().height() - m_characterSheet->characterHeight());
            }

            x = m_selectionX / m_characterSheet->characterAtlasWidth();
            y = m_selectionY / m_characterSheet->characterAtlasHeight();
            m_pattern = (m_selectionX / m_characterSheet->characterWidth()) % m_characterSheet->patternCountForCharacter();
            const int direction = (m_selectionY / m_characterSheet->characterHeight()) % m_characterSheet->directionCountForCharacter();
            m_direction = static_cast<Direction>((direction * 2) + 2);

            m_characterIndex = (y * (m_characterSheet->texture().width() / m_characterSheet->characterAtlasWidth())) + x;
          }
        }
        if (m_characterSheet) {
          ImGui::Dummy(ImVec2{static_cast<float>(m_characterSheet->texture().width()), static_cast<float>(m_characterSheet->texture().height())});
          win->DrawList->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                  win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_characterSheet->texture().width()), static_cast<float>(m_characterSheet->texture().height())}),
                                  ImVec2{0.f, 0.f}, {m_characterSheet->texture().width() / 4096.f, m_characterSheet->texture().height() / 4096.f});
          win->DrawList->AddImage(m_characterSheet->texture(), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                  win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_characterSheet->texture().width()), static_cast<float>(m_characterSheet->texture().height())}));

          win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)}),
                                 win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(m_selectionWidth), static_cast<float>(m_selectionY) + static_cast<float>(m_selectionHeight)}),
                                 0xFF000000, 0.f, 0, 7.f);
          win->DrawList->AddRect(win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_selectionX), static_cast<float>(m_selectionY)}),
                                 win->ContentRegionRect.Min +
                                     (ImVec2{static_cast<float>(m_selectionX) + static_cast<float>(m_selectionWidth), static_cast<float>(m_selectionY) + static_cast<float>(m_selectionHeight)}),
                                 0xFFFFFFFF, 0.f, 0, 3.f);
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##bottom_child");
    {
      ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (calc.x + (ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x));
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
    ImGui::EndChild();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}
