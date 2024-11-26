#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Database/Actors.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

enum class SoundType {
  Cursor,
  OK,
  Cancel,
  Buzzer,
  Equip,
  Save,
  Load,
  Battle_Start,
  Escape,
  Enemy_Attack,
  Enemy_Damage,
  Enemy_Collapse,

};

DBSystemTab::DBSystemTab(System& system, DatabaseEditor* parent)
: IDBEditorTab(parent), m_system(system), m_boatSheet(system.boat.characterName), m_airshipSheet(system.airship.characterName), m_shipSheet(system.ship.characterName) {
  // Boat
  {
    m_boatButtonTexture.setSize(144, 144);
    const auto& [uv0, uv1] = m_boatSheet.getRectForCharacter(m_system.boat.characterIndex, 1);
    Point offset{static_cast<int>(uv0.x() * m_boatSheet.texture().width()), static_cast<int>(uv0.y() * m_shipSheet.texture().height())};
    m_boatButtonTexture.setTexturesToComposite({{m_boatSheet.texture(), {m_boatSheet.characterWidth(), m_boatSheet.characterHeight()}, offset}});
  }
  // Ship
  {
    m_shipButtonTexture.setSize(144, 144);
    const auto& [uv0, uv1] = m_shipSheet.getRectForCharacter(m_system.ship.characterIndex, 1);
    const Point offset{static_cast<int>(uv0.x() * m_shipSheet.texture().width()), static_cast<int>(uv0.y() * m_shipSheet.texture().height())};
    m_shipButtonTexture.setTexturesToComposite({{m_shipSheet.texture(), {m_shipSheet.characterWidth(), m_shipSheet.characterHeight()}, offset}});
  }
  // Airship
  {
    m_airshipButtonTexture.setSize(144, 144);
    const auto& [uv0, uv1] = m_airshipSheet.getRectForCharacter(m_system.airship.characterIndex, 1);
    const Point offset{static_cast<int>(uv0.x() * m_airshipSheet.texture().width()), static_cast<int>(uv0.y() * m_airshipSheet.texture().height())};
    m_airshipButtonTexture.setTexturesToComposite({{m_airshipSheet.texture(), {m_airshipSheet.characterWidth(), m_airshipSheet.characterHeight()}, offset}});
  }
}

void DBSystemTab::addAudioRow(const Audio& audio, const std::string& type, AudioType audioType) {
  ImGui::TableNextRow();
  {
    ImGui::TableNextColumn();
    if (ImGui::Selectable(type.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {}
    ImGui::TableNextColumn();
    ImGui::Text("%s", audio.name.c_str());
  }
}

void DBSystemTab::draw() {
  ImGui::BeginChild("#orpg_system_tab", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
  {
    ImGui::BeginGroup();
    {
      auto calc = ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVW");
      calc.y *= 8;
      calc.y += ImGui::GetStyle().ItemSpacing.y * 16;
      ImGui::BeginChild("#orpg_system_starting_party", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize);
      {
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Starting Party");
          if (ImGui::BeginTable("##starting_party_list", 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, calc)) {
            ImGui::TableSetupColumn("Actor");
            ImGui::TableSetupScrollFreeze(1, 1);
            ImGui::TableHeadersRow();

            const auto& actors = m_system.partyMembers;
            for (auto& actor : actors) {
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::Selectable(Database::instance()->actorNameOrId(actor).c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {}
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable("##orpg_system_tab_party_list_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              // m_actorsPicker.emplace()
            }
            ImGui::EndTable();
          }
        }
        ImGui::EndGroup();
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##starting_game_settings", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
      {
        ImGui::BeginChild("##starting_game_title", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          char tmp[4096]{};
          strncpy(tmp, m_system.gameTitle.c_str(), 4096);
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_title", "Game Title", tmp, 4096, 0)) {
            m_system.gameTitle = tmp;
          }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##game_currency", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          char tmp[4096]{};
          strncpy(tmp, m_system.currencyUnit.c_str(), 4096);
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_currency", "Currency", tmp, 4096, ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWXYZ01").x)) {
            m_system.currencyUnit = tmp;
          }
        }
        ImGui::EndChild();
        ImGui::BeginChild("##game_vehicle_images", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          ImGui::TextUnformatted("Vehicle Images");
          if (!m_boatSheet || m_boatSheet.characterName() != m_system.boat.characterName) {
            m_boatSheet = CharacterSheet(m_system.boat.characterName);
          }
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Boat:");
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_boat_image"), m_boatButtonTexture.get(), ImVec2(m_boatButtonTexture.size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f}, {1.f, 1.f}, {},
                                     {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_boatSheet;
              m_characterPicker.setCharacterInfo(m_system.boat.characterName, m_system.boat.characterIndex);
              m_characterPicker.setOpen(true);
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Ship:");
            const auto& [uv0, uv1] = m_shipSheet.getRectForCharacter(m_system.ship.characterIndex, 1);
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_ship_image"), m_shipButtonTexture.get(), ImVec2(m_shipButtonTexture.size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f}, {1.f, 1.f}, {},
                                     {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_shipSheet;
              m_characterPicker.setCharacterInfo(m_system.ship.characterName, m_system.ship.characterIndex);
              m_characterPicker.setOpen(true);
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Airship:");
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_airship_image"), m_airshipButtonTexture.get(), ImVec2(m_airshipButtonTexture.size()) * App::DPIHandler::get_ui_scale(), ImVec2{0.f, 0.f},
                                     ImVec2{1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_airshipSheet;
              m_characterPicker.setCharacterInfo(m_system.airship.characterName, m_system.airship.characterIndex);
              m_characterPicker.setOpen(true);
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##game_window_background", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          ImGui::TextUnformatted("Window Color");
          m_gameWindowBackground.update(m_system.windowTone[1], m_system.windowTone[1], m_system.windowTone[2]);
          if (ImGui::ImageButtonEx(ImGui::GetID("##game_window_background_button"), m_gameWindowBackground,
                                   ImVec2{static_cast<float>(m_gameWindowBackground.width()), static_cast<float>(m_gameWindowBackground.height())} * App::DPIHandler::get_ui_scale(), ImVec2{0.f, 0.f},
                                   ImVec2{1.f, 1}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
            // TODO: GameWindowColorPickerDialog imp
          }
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##system_options", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Options");
        ImGui::Checkbox("Use Side-view Battle", &m_system.optSideView);
        ImGui::Checkbox("Start Transparent", &m_system.optTransparent);
        ImGui::Checkbox("Show Player Followers", &m_system.optFollowers);
        ImGui::Checkbox("Knockout by Slip Damage", &m_system.optSlipDeath);
        ImGui::Checkbox("Knockout by Floor Damage", &m_system.optFloorDeath);
        ImGui::Checkbox("Display TP in Battle", &m_system.optDisplayTp);
        ImGui::Checkbox("EXP for Reserve Members", &m_system.optExtraExp);
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##system_sv_magic_skills", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("[SV] Magic Skills");
        if (ImGui::BeginTable("##starting_magic_skills_list", 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, calc)) {
          ImGui::TableSetupColumn("Skill Type");
          ImGui::TableSetupScrollFreeze(1, 1);
          ImGui::TableHeadersRow();

          const auto& skills = m_system.magicSkills;
          int i = 0;
          for (int skill : skills) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable(Database::instance()->skillTypeNameOrId(skill).c_str(), m_selectedSkill == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              m_selectedSkill = i;
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                m_skillTypePicker.emplace("Skill Types", m_system.skillTypes, i, 0);
                m_skillTypePicker->setOpen(true);
              }
            }
            ++i;
          }
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable("##orpg_system_tab_party_list_dummy", i >= m_system.magicSkills.size(), ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              m_skillTypePicker.emplace("Skill Types", m_system.skillTypes, 1, 0);
              m_skillTypePicker->setOpen(true);
            }
          }
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      auto calc = ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWABCDEFGKLMNOPQRSTUVW");
      calc.y *= 8;
      calc.y += ImGui::GetStyle().ItemSpacing.y * 16;
      ImGui::BeginChild("##orpg_system_music_files", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Music");
        if (ImGui::BeginTable("##system_audio_music_list", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, calc)) {
          ImGui::TableSetupColumn("Type");
          ImGui::TableSetupColumn("File");
          ImGui::TableSetupScrollFreeze(2, 1);
          ImGui::TableHeadersRow();

          addAudioRow(m_system.titleBgm, "Title", AudioType::BackgroundMusic);
          addAudioRow(m_system.battleBgm, "Battle", AudioType::BackgroundMusic);
          addAudioRow(m_system.victoryMe, "Victory", AudioType::Melody);
          addAudioRow(m_system.defeatMe, "Defeat", AudioType::Melody);
          addAudioRow(m_system.gameoverMe, "Game Over", AudioType::Melody);
          addAudioRow(m_system.boat.bgm, "Boat", AudioType::BackgroundMusic);
          addAudioRow(m_system.ship.bgm, "Ship", AudioType::BackgroundMusic);
          addAudioRow(m_system.airship.bgm, "Airship", AudioType::BackgroundMusic);
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
      ImGui::BeginChild("##orpg_system_sound_files", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Sounds");
        if (ImGui::BeginTable("##system_audio_sound_list", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, calc)) {
          ImGui::TableSetupColumn("Type");
          ImGui::TableSetupColumn("File");
          ImGui::TableSetupScrollFreeze(2, 1);
          ImGui::TableHeadersRow();

          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      auto calc = ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWABCDEFGKLMNOPQRSTUVW");
      calc.y *= 13;
      calc.y += ImGui::GetStyle().ItemSpacing.y * 26;
      ImGui::BeginChild("##system_menu_commands", {calc.x + (ImGui::GetStyle().FramePadding.x * 2), 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Menu Commands");
        ImGui::BeginChild("##menu_command_set1", {ImGui::GetContentRegionMax().x / 3, 0}, 0, ImGuiWindowFlags_NoBackground);
        {
          ImGui::Checkbox("Item", &m_system.menuCommands[0]);
          ImGui::Checkbox("Status", &m_system.menuCommands[3]);
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##menu_command_set2", {ImGui::GetContentRegionMax().x / 3, 0}, ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoBackground);
        {
          ImGui::Checkbox("Skill", &m_system.menuCommands[1]);
          ImGui::Checkbox("Formation", &m_system.menuCommands[4]);
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##menu_command_set3", {ImGui::GetContentRegionMax().x / 3, 0}, ImGuiChildFlags_AutoResizeY, ImGuiWindowFlags_NoBackground);
        {
          ImGui::Checkbox("Equip", &m_system.menuCommands[2]);
          ImGui::Checkbox("Save", &m_system.menuCommands[5]);
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();
      ImGui::BeginChild("##system_sv_attack_motions", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_FrameStyle, ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("[SV] Attack Motions");
        if (ImGui::BeginTable("##system_sv_attack_motions_list", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, calc)) {
          ImGui::TableSetupColumn("Type");
          ImGui::TableSetupColumn("Motion");
          ImGui::TableSetupColumn("Image");
          ImGui::TableSetupScrollFreeze(3, 1);
          ImGui::TableHeadersRow();

          const auto& attackMotions = m_system.attackMotions;
          for (int i = 0; const auto& attackMotion : attackMotions) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable(i == 0 ? "Bare Hands" : Database::instance()->weaponTypeNameOrId(i).c_str(), i == m_selectedMotion,
                                  ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              m_selectedMotion = i;
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                // TODO: Implement Motion Type dialog
              }
            }
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("{NEEDS ENUM}");
            ImGui::TableNextColumn();
            ImGui::TextUnformatted("{NEEDS ENUM}");
            ++i;
          }
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::EndChild();

  const auto& [closed, confirmed] = m_characterPicker.draw();
  if (closed) {
    if (confirmed && m_currentSheet != nullptr) {
      if (m_currentSheet == &m_boatSheet) {
        m_system.boat.characterName = m_characterPicker.selectedSheet();
        m_system.boat.characterIndex = m_characterPicker.character();
        m_boatSheet = CharacterSheet(m_system.boat.characterName);
        const auto& [uv0, uv1] = m_boatSheet.getRectForCharacter(m_system.boat.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_boatSheet.texture().width()), static_cast<int>(uv0.y() * m_boatSheet.texture().height())};
        m_boatButtonTexture.setTexturesToComposite({{m_boatSheet.texture(), {m_boatSheet.characterWidth(), m_boatSheet.characterHeight()}, offset}});
      } else if (m_currentSheet == &m_shipSheet) {
        m_system.ship.characterName = m_characterPicker.selectedSheet();
        m_system.ship.characterIndex = m_characterPicker.character();
        m_shipSheet = CharacterSheet(m_system.ship.characterName);
        const auto& [uv0, uv1] = m_shipSheet.getRectForCharacter(m_system.ship.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_shipSheet.texture().width()), static_cast<int>(uv0.y() * m_shipSheet.texture().height())};
        m_shipButtonTexture.setTexturesToComposite({{m_shipSheet.texture(), {m_shipSheet.characterWidth(), m_shipSheet.characterHeight()}, offset}});
      } else if (m_currentSheet == &m_airshipSheet) {
        m_system.airship.characterName = m_characterPicker.selectedSheet();
        m_system.airship.characterIndex = m_characterPicker.character();
        m_airshipSheet = CharacterSheet(m_system.airship.characterName);
        const auto& [uv0, uv1] = m_airshipSheet.getRectForCharacter(m_system.airship.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_airshipSheet.texture().width()), static_cast<int>(uv0.y() * m_airshipSheet.texture().height())};
        m_airshipButtonTexture.setTexturesToComposite({{m_airshipSheet.texture(), {m_airshipSheet.characterWidth(), m_airshipSheet.characterHeight()}, offset}});
      }
      m_currentSheet = nullptr;
    }
    m_characterPicker.setOpen(false);
  }
}
