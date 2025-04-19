#include "Core/CommonUI/CharacterPicker.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Core/ResourceManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include <IconsFontAwesome6.h>
#include <iostream>

#include "Database/Database.hpp"

static bool ContainsCaseInsensitive(std::string_view str, std::string_view val) {
  return std::search(str.begin(), str.end(), val.begin(), val.end(), [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }) != str.end();
}

static int makeTileId(const int n, const int x, const int y) { return n * 256 + floor(x / 8) * 128 + y * 8 + x % 8; }
CharacterPicker::CharacterPicker(const PickerMode mode, const bool useTileset, const int tileId, const std::string_view sheetName, const int character, const int pattern, const Direction direction)
: IDialogController("Select an Image##character_picker")
, m_pickerMode(mode)
, m_characterIndex(character)
, m_pattern(pattern)
, m_direction(direction) {
  m_charDir.emplace("img/characters/", ".png", static_cast<std::string>(sheetName));
  m_characterSheets = m_charDir.value().getDirectoryContents();
  if (useTileset) {
    int mapId = Database::instance()->mapInfos.currentMap()->id();
    m_tileSheets = Database::instance()->tilesets.tileset(Database::instance()->mapInfos.map(mapId)->map()->tilesetId())->tilesetNames();
    // m_palette.setPageIndex(5);
    // m_palette.setTilesetNames(m_tileSheets);
  }
  m_folders = m_charDir.value().getDirectories();

  if (useTileset) {
    setTileId(tileId);
  } else {
    setCharacterInfo(sheetName, character, pattern, direction);
  }
}

void CharacterPicker::setCharacterInfo(const std::string_view sheetName, const int character, const int pattern, const Direction direction) {
  m_characterSheet.emplace(sheetName);
  if (m_characterSheet->texture()) {
    m_characterIndex = character;
    m_pattern = pattern;
    m_direction = direction;

    std::string imageName = m_charDir.value().getFileName(static_cast<std::string>(sheetName));
    if (!imageName.empty()) {
      m_selectedSheet = sheetIndexOf(imageName);

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
      m_selectedSheet = -5;
    }
    m_checkerboardTexture.setSize(m_characterSheet->texture().width(), m_characterSheet->texture().height());
  }
}

void CharacterPicker::setTileId(int tileId) {
  if (m_tileSheets.empty()) {
    int mapId = Database::instance()->mapInfos.currentMap()->id();
    m_tileSheets = Database::instance()->tilesets.tileset(Database::instance()->mapInfos.map(mapId)->map()->tilesetId())->tilesetNames();
  }
  m_tileId = tileId;
  if (m_tileId > 0) {
    m_isTile = true;
  }
  m_selectedSheet = -5 + floor(m_tileId / 256);
  std::string tileSheet = m_tileSheets[m_selectedSheet + 9];
  setCursorPos(setCursorByTile(m_tileId));
  m_selectedSheet = m_tileId < 256 ? -4 : m_tileId < 512 ? -3 : m_tileId < 768 ? -2 : -1;
  m_characterSheet.emplace(tileSheet, true, m_tileId);
  m_characterIndex = 0;
  m_pattern = 0;
  m_direction = Direction::Retain;
  m_selectionWidth = Database::instance()->system.tileSize();
  m_selectionHeight = Database::instance()->system.tileSize();

  m_checkerboardTexture.setSize(m_characterSheet->texture().width(), m_characterSheet->texture().height());
}

void CharacterPicker::setCursorPos(Point pos) {
  m_selectionX = pos.x() * 48;
  m_selectionY = pos.y() * 48;
}

int CharacterPicker::sheetIndexOf(std::string& str) {
  bool found = false;
  for (int i = 0; i < m_characterSheets.size(); ++i) {
    if (!m_characterSheets[i].compare(str)) {
      found = true;
      m_selectedSheet = i;
    }
  }
  if (!found) {
    m_characterSheet.reset();
    m_selectedSheet = -5;
    return -4;
  }
  return m_selectedSheet;
}

std::tuple<bool, bool> CharacterPicker::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }

  int index{0};
  if (m_sortRequest) {
    m_sortedIndexes.clear();
    for (auto& str : m_characterSheets) {
      if (!m_filter.empty()) {
        if (ContainsCaseInsensitive(str, m_filter)) {
          m_sortedIndexes.insert(std::make_pair(index, ""));
          // m_sortedList.push_back(str);
        }
      } else {
        m_sortedIndexes.insert(std::make_pair(index, ""));
        // m_sortedList.push_back(str);
      }
      index++;
    }
    m_sortRequest = false;
  }

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSizeConstraints(ImGui::GetDPIScaledSize(320, 320), {FLT_MAX, FLT_MAX});
  ImGui::SetNextWindowSize(ImGui::GetDPIScaledSize(320, 320), ImGuiCond_Once);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open)) {
    const auto calc = ImGui::CalcTextSize("OKCANCEL");
    ImGui::BeginChild("##top_child", {0, ImGui::GetContentRegionAvail().y - (calc.y + (ImGui::GetStyle().ItemSpacing.y * 2) + ImGui::GetStyle().FramePadding.y)});
    {
      ImGui::BeginVertical("selection_status_layout");
      {
        ImGui::BeginHorizontal("filter_layout");
        {
          ImGui::Text("Filter");
          if (ImGui::InputText("##object_selection_filter_input", &m_filter)) {
            m_selectedSheet = -5;
            if (m_filter.empty()) {
              m_sortedIndexes.clear();
            } else {
              m_sortRequest = true;
            }
          }
          if (ImGui::Button("Clear")) {
            m_filter.clear();
            m_sortedIndexes.clear();
          }
        }
        ImGui::EndHorizontal();
        std::string selectedTileSheet = m_selectedSheet == -5 ? "(None)" : m_selectedSheet < 0 ? m_tileSheets[m_selectedSheet + 9] : "(None)";
        std::string selectedSheet = m_selectedSheet == -5 ? "(None)" : m_characterSheets.size() > 0 && m_selectedSheet > -1 ? m_characterSheets[m_selectedSheet] : "(None)";
        ImGui::Text("Selected Sheet: %s", selectedSheet.c_str());
      }
      ImGui::EndVertical();
      ImGui::BeginChild("##character_picker_sheet_list", ImVec2{ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRS").x, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y},
                        ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
      {
        if (ImGui::BeginTable("##character_picker.characterlist", 1, ImGuiTableFlags_Resizable)) {
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          if (ImGui::Selectable("(None)", m_selectedSheet == -5, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            m_selectedSheet = -5;
            m_isTile = false;
            m_characterSheet.reset();
          }
          if (m_selectedSheet == -5) {
            ImGui::SetItemDefaultFocus();
          }

          if (m_isTile) {
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 4);
          }
          for (int z = -4; z < 0; ++z) {
            auto& sheet = m_tileSheets[z + 9];
            if (!sheet.empty()) {
              if (ImGui::Selectable(std::format("Tileset {}", static_cast<char>('A' + (z + 5))).c_str(), m_selectedSheet == z, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                if (m_selectedSheet != z) {
                  m_selectedSheet = z;
                  m_selectionWidth = Database::instance()->system.tileSize();
                  m_selectionHeight = Database::instance()->system.tileSize();
                  m_tileId = makeTileId(m_selectedSheet + 4, m_selectionX / m_selectionWidth, m_selectionY / m_selectionHeight);
                  m_characterSheet.emplace(sheet, true, m_tileId);
                  if (m_characterSheet->texture()) {
                    m_checkerboardTexture.setSize(m_characterSheet->texture().width(), m_characterSheet->texture().height());
                  }
                  m_isTile = true;
                }
              }
              if (m_selectedSheet == z && ImGui::IsWindowAppearing()) {
                ImGui::SetScrollHereY();
              }
            }
          }

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 4);
          ImGui::BeginDisabled(m_charDir.value().isParentDirectory());
          if (ImGui::Selectable("\u21B0 ..", false, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_charDir.value().moveUp();
              m_characterSheets = m_charDir->getDirectoryContents();
              m_folders = m_charDir.value().getDirectories();
              m_selectedSheet = -5;
              m_selectedFolder = -1;
              m_isTile = false;
            }
          }
          ImGui::EndDisabled();
          ImGui::BeginChild("##dir_list", {0, (ImGui::CalcTextSize("A").y * 5) + (ImGui::GetStyle().ItemSpacing.y * 2)}, ImGuiChildFlags_ResizeY,
                            ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
          {
            for (int i = 0; i < m_folders.size(); ++i) {
              const auto& folderName = std::format("{} {}", ICON_FA_FOLDER_OPEN, m_folders[i]);
              if (ImGui::Selectable(folderName.c_str(), m_selectedFolder == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                m_selectedFolder = i;
              }
              if (m_selectedFolder == i && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_charDir->setDirectory(i);
                m_folders = m_charDir.value().getDirectories();
                m_characterSheets = m_charDir.value().getDirectoryContents();
                m_selectedSheet = -5;
                m_selectedFolder = -1;
                m_isTile = false;
              }
            }
          }
          ImGui::EndChild();
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 4);
          ImGui::BeginChild("##sheet_list", {}, 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
          {
            for (int i = 0; i < m_characterSheets.size(); ++i) {
              if (m_filter.empty() == false && m_sortedIndexes.contains(i) == false) {
                continue;
              }
              const auto& sheet = m_characterSheets[i];
              ImGui::TableNextColumn();
              if (ImGui::Selectable(std::format("{0}##sheet_{0}", sheet).c_str(), m_selectedSheet == i, ImGuiSelectableFlags_SelectOnNav | ImGuiSelectableFlags_SelectOnClick)) {
                if (m_selectedSheet != i) {
                  m_characterSheet.emplace(m_charDir.value().isParentDirectory() ? sheet : m_charDir.value().getPathPrefix() + '/' + sheet);
                  if (m_characterSheet->texture()) {
                    m_checkerboardTexture.setSize(m_characterSheet->texture().width(), m_characterSheet->texture().height());
                  }
                  if (m_pickerMode == PickerMode::Character) {
                    m_selectionWidth = m_characterSheet->characterAtlasWidth();
                    m_selectionHeight = m_characterSheet->characterAtlasHeight();
                  } else if (m_pickerMode == PickerMode::PatternAndDirection) {
                    m_selectionWidth = m_characterSheet->characterWidth();
                    m_selectionHeight = m_characterSheet->characterHeight();
                  }
                }
                m_isTile = false;
                m_selectedSheet = i;
              }
              if (m_selectedSheet == i && ImGui::IsWindowAppearing()) {
                ImGui::SetScrollHereY();
              }
              if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", sheet.c_str());
              }
            }
          }
          ImGui::EndChild();
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##character_picker_sheet_panel", {}, ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
      {
        auto win = ImGui::GetCurrentWindow();
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()) {
          auto mouseCursor = ImGui::GetMousePos();
          mouseCursor -= win->ContentRegionRect.Min;
          int x = alignValue(mouseCursor.x, m_selectionWidth);
          int y = alignValue(mouseCursor.y, m_selectionHeight);
          if ((x >= 0 && x < m_characterSheet->texture().width()) && (y >= 0 && y < m_characterSheet->texture().height())) {
            m_selectionX = x;
            m_selectionY = y;
            if (m_isTile) {
              // On selection, set the tileId
              m_tileId = makeTileId(m_selectedSheet + 4, x / Database::instance()->system.tileSize(), y / Database::instance()->system.tileSize());
            } else {
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
        }
        if (m_characterSheet) {
          ImGui::Dummy(ImVec2{static_cast<float>(m_characterSheet->texture().width()), static_cast<float>(m_characterSheet->texture().height())});
          win->DrawList->AddImage(static_cast<ImTextureID>(m_checkerboardTexture), win->ContentRegionRect.Min + ImVec2{0.f, 0.f},
                                  win->ContentRegionRect.Min + (ImVec2{static_cast<float>(m_checkerboardTexture.width()), static_cast<float>(m_checkerboardTexture.height())}));
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
    ImGui::BeginHorizontal(std::format("##buttons_character_picker{}", reinterpret_cast<uintptr_t>(this)).c_str(), ImGui::GetContentRegionAvail());
    {
      ImGui::Spring();
      const int ret = ImGui::ButtonGroup(std::format("##ok_cancel_character_picker{}", reinterpret_cast<uintptr_t>(this)).c_str(), {
                                                                                                                                       trNOOP("OK"),
                                                                                                                                       trNOOP("Cancel"),
                                                                                                                                   });
      if (ret == 0) { // OK
        if (m_selectedSheet == -5 || m_selectedSheet >= 0) {
          m_tileId = 0;
          m_isTile = false;
        }
        m_filter.clear();
        m_sortedIndexes.clear();
        m_confirmed = true;
        m_open = false;
        ImGui::CloseCurrentPopup();
      } else if (ret == 1) { // Cancel
        m_filter.clear();
        m_sortedIndexes.clear();
        m_confirmed = false;
        m_open = false;
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndHorizontal();
    ImGui::EndPopup();
  }

  return {!m_open, m_confirmed};
}

Point CharacterPicker::setCursorByTile(const int tileId) {
  if (tileId > 0) {
    int tileX = floor(tileId % 256 / 128) * 8 + tileId % 8;
    int tileY = floor(tileId % 128 / 8);
    return {tileX, tileY};
  }
  return {0, 0};
}
