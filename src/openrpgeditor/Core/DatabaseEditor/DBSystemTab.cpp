#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Database/Actors.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include "imgui.h"
#include "imgui_internal.h"

using namespace std::string_view_literals;

DBSystemTab::DBSystemTab(System& system, DatabaseEditor* parent) : IDBEditorTab(parent), m_system(system) {}

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
  if (!m_characterPicker) {
    m_characterPicker.emplace();
  }

  if (!m_boatButtonTexture) {
    m_boatButtonTexture.emplace();
    m_boatButtonTexture->setSize(144, 144);
    if (!m_boatSheet) {
      m_boatSheet.emplace(m_system.boat.characterName);
    }
    const auto& [uv0, uv1] = m_boatSheet->getRectForCharacter(m_system.boat.characterIndex, 1);
    Point offset{static_cast<int>(uv0.x() * m_boatSheet->texture().width()), static_cast<int>(uv0.y() * m_shipSheet->texture().height())};
    m_boatButtonTexture->setTexturesToComposite({{m_boatSheet->texture(), {m_boatSheet->characterWidth(), m_boatSheet->characterHeight()}, offset}});
  }
  if (!m_shipButtonTexture) {
    m_shipButtonTexture.emplace();
    m_shipButtonTexture->setSize(144, 144);
    if (!m_shipSheet) {
      m_shipSheet.emplace(m_system.ship.characterName);
    }
    const auto& [uv0, uv1] = m_shipSheet->getRectForCharacter(m_system.ship.characterIndex, 1);
    const Point offset{static_cast<int>(uv0.x() * m_shipSheet->texture().width()), static_cast<int>(uv0.y() * m_shipSheet->texture().height())};
    m_shipButtonTexture->setTexturesToComposite({{m_shipSheet->texture(), {m_shipSheet->characterWidth(), m_shipSheet->characterHeight()}, offset}});
  }
  if (!m_airshipButtonTexture) {
    m_airshipButtonTexture.emplace();
    m_airshipButtonTexture->setSize(144, 144);
    if (!m_airshipSheet) {
      m_airshipSheet.emplace(m_system.airship.characterName);
    }
    const auto& [uv0, uv1] = m_airshipSheet->getRectForCharacter(m_system.airship.characterIndex, 1);
    const Point offset{static_cast<int>(uv0.x() * m_airshipSheet->texture().width()), static_cast<int>(uv0.y() * m_airshipSheet->texture().height())};
    m_airshipButtonTexture->setTexturesToComposite({{m_airshipSheet->texture(), {m_airshipSheet->characterWidth(), m_airshipSheet->characterHeight()}, offset}});
  }
  ImGui::BeginChild("#orpg_system_tab", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
  if (!m_gameWindowBackground) {
    m_gameWindowBackground.emplace();
  }

  if (m_gameWindowBackground->width() != oRound(App::DPIHandler::scale_value(164)) || m_gameWindowBackground->height() != oRound(App::DPIHandler::scale_value(164))) {
    m_gameWindowBackground->setSize(oRound(App::DPIHandler::scale_value(164)), oRound(App::DPIHandler::scale_value(164)));
  }
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
            for (int i = 0; auto& actor : actors) {
              ImGui::TableNextRow();
              ImGui::TableNextColumn();
              if (ImGui::Selectable(Database::instance()->actorNameOrId(actor).c_str(), m_selectedActor == i, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                m_selectedActor = i;
                if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                  if (!m_actorsPicker) {
                    m_actorsPicker.emplace("Party Member"sv, Database::instance()->actors.actorList(), m_selectedActor);
                  }
                  m_actorsPicker->setOpen(true);
                }
              }
              ++i;
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            if (ImGui::Selectable("##orpg_system_tab_party_list_dummy", m_selectedActor == -1, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              m_selectedActor = -1;
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                if (!m_actorsPicker) {
                  m_actorsPicker.emplace("Party Member"sv, Database::instance()->actors.actorList(), 1);
                }
                m_actorsPicker->setOpen(true);
              }
            }
            if (ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused() && m_selectedActor != -1) {
              m_system.partyMembers.erase(m_system.partyMembers.begin() + m_selectedActor);
              --m_selectedActor;
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
          ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_title", "Game Title", tmp, 4096, 0)) {
            m_system.gameTitle = tmp;
          }
          ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##game_currency", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          char tmp[4096]{};
          strncpy(tmp, m_system.currencyUnit.c_str(), 4096);
          ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_currency", "Currency", tmp, 4096, ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWXYZ01").x)) {
            m_system.currencyUnit = tmp;
          }
          ImGui::PopStyleColor();
        }
        ImGui::EndChild();
        ImGui::BeginChild("##game_vehicle_images", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                          ImGuiWindowFlags_NoResize);
        {
          ImGui::TextUnformatted("Vehicle Images");
          if (!m_boatSheet || m_boatSheet->characterName() != m_system.boat.characterName) {
            m_boatSheet = CharacterSheet(m_system.boat.characterName);
          }
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Boat:");
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_boat_image"), m_boatButtonTexture->get(), ImVec2(m_boatButtonTexture->size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f}, {1.f, 1.f}, {},
                                     {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_boatSheet.value();
              m_characterPicker->setCharacterInfo(m_system.boat.characterName, m_system.boat.characterIndex);
              m_characterPicker->setOpen(true);
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Ship:");
            const auto& [uv0, uv1] = m_shipSheet->getRectForCharacter(m_system.ship.characterIndex, 1);
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_ship_image"), m_shipButtonTexture->get(), ImVec2(m_shipButtonTexture->size()) * App::DPIHandler::get_ui_scale(), {0.f, 0.f}, {1.f, 1.f}, {},
                                     {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_shipSheet.value();
              m_characterPicker->setCharacterInfo(m_system.ship.characterName, m_system.ship.characterIndex);
              m_characterPicker->setOpen(true);
            }
          }
          ImGui::EndGroup();
          ImGui::SameLine();
          ImGui::BeginGroup();
          {
            ImGui::TextUnformatted("Airship:");
            if (ImGui::ImageButtonEx(ImGui::GetID("##system_airship_image"), m_airshipButtonTexture->get(), ImVec2(m_airshipButtonTexture->size()) * App::DPIHandler::get_ui_scale(), ImVec2{0.f, 0.f},
                                     ImVec2{1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_currentSheet = &m_airshipSheet.value();
              m_characterPicker->setCharacterInfo(m_system.airship.characterName, m_system.airship.characterIndex);
              m_characterPicker->setOpen(true);
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
          m_gameWindowBackground->update(m_system.windowTone[0], m_system.windowTone[1], m_system.windowTone[2]);
          if (ImGui::ImageButtonEx(ImGui::GetID("##game_window_background_button"), *m_gameWindowBackground,
                                   ImVec2{static_cast<float>(m_gameWindowBackground->width()), static_cast<float>(m_gameWindowBackground->height())}, ImVec2{0.f, 0.f}, ImVec2{1.f, 1.f}, {},
                                   {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
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
                if (!m_skillTypePicker) {
                  m_skillTypePicker.emplace("Skill Type", m_system.skillTypes, m_selectedSkill);
                }
                m_skillTypePicker->setOpen(true);
              }
            }
            ++i;
          }
          ImGui::TableNextRow();
          ImGui::TableNextColumn();
          if (ImGui::Selectable("##orpg_system_tab_party_list_dummy", m_selectedSkill == -1, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
            m_selectedSkill = -1;
            if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
              if (!m_skillTypePicker) {
                m_skillTypePicker.emplace("Skill Type", m_system.skillTypes, 0);
              }
              m_skillTypePicker->setOpen(true);
            }
          }

          if (ImGui::IsKeyPressed(ImGuiKey_Delete) && ImGui::IsWindowFocused() && m_selectedSkill != -1) {
            m_system.magicSkills.erase(m_system.magicSkills.begin() + m_selectedSkill);
            m_selectedSkill--;
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

          for (int type = 0; const auto& sound : m_system.sounds) {
            addAudioRow(sound, DecodeEnumName(static_cast<SoundType>(type)), AudioType::SoundEffect);
            ++type;
          }
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
        ImGui::NewLine();
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
            ImGui::TextUnformatted(DecodeEnumName(attackMotion.type).c_str());
            ImGui::TableNextColumn();
            ImGui::TextUnformatted(DecodeEnumName(attackMotion.weaponImageId).c_str());
            ++i;
          }
          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      auto calc = ImGui::CalcTextSize("ABCDEFGKLMNOPQRSTUVWXYZABCDEFGKLMNOPQRSTUVWX").x;
      ImGui::BeginChild("##orpg_system_start_positions", {calc, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Starting Position");
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Player");
          const std::string label =
              std::format("{} ({},{})##player_start_button", m_system.startMapId == 0 ? "None" : Database::instance()->mapNameOrId(m_system.startMapId), m_system.startX, m_system.startY);
          if (ImGui::Button(label.c_str(), {ImGui::GetContentRegionAvail().x, 0})) {
            // TODO: Map position picker
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Boat");
          const std::string label = std::format("{} ({},{})##boat_start_button", m_system.boat.startMapId == 0 ? "None" : Database::instance()->mapNameOrId(m_system.boat.startMapId),
                                                m_system.boat.startX, m_system.boat.startY);
          if (ImGui::Button(label.c_str(), {ImGui::GetContentRegionAvail().x, 0})) {
            // TODO: Map position picker
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Ship");
          const std::string label = std::format("{} ({},{})##ship_start_button", m_system.ship.startMapId == 0 ? "None" : Database::instance()->mapNameOrId(m_system.ship.startMapId),
                                                m_system.ship.startX, m_system.ship.startY);
          if (ImGui::Button(label.c_str(), {ImGui::GetContentRegionAvail().x, 0})) {
            // TODO: Map position picker
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Airship");
          const std::string label = std::format("{} ({},{})##airship_start_button", m_system.airship.startMapId == 0 ? "None" : Database::instance()->mapNameOrId(m_system.airship.startMapId),
                                                m_system.airship.startX, m_system.airship.startY);
          if (ImGui::Button(label.c_str(), {ImGui::GetContentRegionAvail().x, 0})) {
            // TODO: Map position picker
          }
        }
        ImGui::EndGroup();
      }
      ImGui::EndChild();
      ImGui::BeginChild("##orpg_system_title_image", {calc, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle,
                        ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Title Screen");
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Images");
          const std::string label = std::format("{}##title_images_button", Database::dualImageText(m_system.title1Name, m_system.title2Name));
          if (ImGui::Button(label.c_str(), {ImGui::GetContentRegionAvail().x, 0})) {
            if (!m_titleImagePicker) {
              m_titleImagePicker.emplace(ImagePicker::PickerMode::Title, m_system.title1Name, m_system.title2Name);
            } else {
              m_titleImagePicker->setImageInfo(m_system.title1Name, m_system.title2Name);
            }
            m_titleImagePicker->setOpen(true);
          }
        }
        ImGui::EndGroup();
        ImGui::Checkbox("Draw Game Title", &m_system.optDrawTitle);
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::EndChild();

  if (m_actorsPicker) {
    if (const auto [closed, confirmed] = m_actorsPicker->draw(); closed && confirmed) {
      if (m_selectedActor == -1) {
        int actor = m_actorsPicker->selection();
        // We only want on instance of each actor in the party, so check to make sure this actor isn't already in the party
        if (std::ranges::find_if(m_system.partyMembers, [actor](const auto& partyMember) { return partyMember == actor; }) == m_system.partyMembers.end()) {
          m_system.partyMembers.emplace_back(actor);
          m_selectedActor = static_cast<int>(m_system.partyMembers.size() - 1);
        }
      } else {
        m_system.partyMembers[m_selectedActor] = m_actorsPicker->selection();
      }
      m_actorsPicker->accept();
    }
  }

  if (m_skillTypePicker) {
    if (const auto [closed, confirmed] = m_skillTypePicker->draw(); closed && confirmed) {
      if (m_selectedSkill == -1) {
        int magicSkill = m_skillTypePicker->selection();
        // We only want on instance of each skill type in the skills list, so check to make sure this type isn't already in the skills list
        if (std::ranges::find_if(m_system.magicSkills, [magicSkill](const auto& skill) { return skill == magicSkill; }) == m_system.magicSkills.end()) {
          m_system.magicSkills.emplace_back(magicSkill);
          m_selectedSkill = static_cast<int>(m_system.magicSkills.size() - 1);
        }
      } else {
        m_system.magicSkills[m_selectedSkill] = m_skillTypePicker->selection();
      }
      m_skillTypePicker->accept();
    }
  }

  if (m_titleImagePicker) {
    if (const auto& [closed, confirmed] = m_titleImagePicker->draw(); closed && confirmed) {
      m_system.title1Name = m_titleImagePicker->selectedImage();
      m_system.title2Name = m_titleImagePicker->selectedImage2();
      m_titleImagePicker->accept();
    }
  }

  if (const auto& [closed, confirmed] = m_characterPicker->draw(); closed) {
    if (confirmed && m_currentSheet != nullptr) {
      if (m_currentSheet == &m_boatSheet.value()) {
        m_system.boat.characterName = m_characterPicker->selectedSheet();
        m_system.boat.characterIndex = m_characterPicker->character();
        m_boatSheet = CharacterSheet(m_system.boat.characterName);
        const auto& [uv0, uv1] = m_boatSheet->getRectForCharacter(m_system.boat.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_boatSheet->texture().width()), static_cast<int>(uv0.y() * m_boatSheet->texture().height())};
        m_boatButtonTexture->setTexturesToComposite({{m_boatSheet->texture(), {m_boatSheet->characterWidth(), m_boatSheet->characterHeight()}, offset}});
      } else if (m_currentSheet == &m_shipSheet.value()) {
        m_system.ship.characterName = m_characterPicker->selectedSheet();
        m_system.ship.characterIndex = m_characterPicker->character();
        m_shipSheet = CharacterSheet(m_system.ship.characterName);
        const auto& [uv0, uv1] = m_shipSheet->getRectForCharacter(m_system.ship.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_shipSheet->texture().width()), static_cast<int>(uv0.y() * m_shipSheet->texture().height())};
        m_shipButtonTexture->setTexturesToComposite({{m_shipSheet->texture(), {m_shipSheet->characterWidth(), m_shipSheet->characterHeight()}, offset}});
      } else if (m_currentSheet == &m_airshipSheet.value()) {
        m_system.airship.characterName = m_characterPicker->selectedSheet();
        m_system.airship.characterIndex = m_characterPicker->character();
        m_airshipSheet = CharacterSheet(m_system.airship.characterName);
        const auto& [uv0, uv1] = m_airshipSheet->getRectForCharacter(m_system.airship.characterIndex, 1);
        const Point offset{static_cast<int>(uv0.x() * m_airshipSheet->texture().width()), static_cast<int>(uv0.y() * m_airshipSheet->texture().height())};
        m_airshipButtonTexture->setTexturesToComposite({{m_airshipSheet->texture(), {m_airshipSheet->characterWidth(), m_airshipSheet->characterHeight()}, offset}});
      }
      m_currentSheet = nullptr;
    }
    m_characterPicker->setOpen(false);
  }
}
