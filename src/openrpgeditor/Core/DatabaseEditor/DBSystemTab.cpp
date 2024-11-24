#include "Core/DatabaseEditor/DBSystemTab.hpp"
#include "Database/Actors.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include "imgui.h"

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
  Enemy_Collaps,

};

void DBSystemTab::addAudioRow(Audio& audio, const std::string& type, AudioType audioType) {
  ImGui::TableNextRow();
  {
    ImGui::TableNextColumn();
    if (ImGui::Selectable(type.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {}
    ImGui::TableNextColumn();
    ImGui::Selectable(audio.name.c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick);
  }
}

void DBSystemTab::draw() {
  ImGui::BeginChild("#orpg_system_tab", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
  {
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("#orpg_system_starting_party", ImVec2(0, App::DPIHandler::scale_value(150.f)), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize,
                        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
      {
        ImGui::BeginGroup();
        {
          ImGui::TextUnformatted("Starting Party");
          if (ImGui::BeginTable("##starting_party_list", 1, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY)) {
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
            if (ImGui::Selectable("##orpg_system_tab_party_list_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {}
            ImGui::EndTable();
          }
        }
        ImGui::EndGroup();
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##starting_game_settings", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize,
                        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
      {
        ImGui::BeginGroup();
        {
          char tmp[4096]{};
          strncpy(tmp, m_system.gameTitle.c_str(), 4096);
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_title", "Game Title", tmp, 4096, 0)) {
            m_system.gameTitle = tmp;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          char tmp[4096]{};
          strncpy(tmp, m_system.currencyUnit.c_str(), 4096);
          if (ImGui::LabelOverLineEdit("##orpg_system_tab_game_currency", "Currency", tmp, 4096, 0)) {
            m_system.currencyUnit = tmp;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        { ImGui::TextUnformatted("Vehicle Images"); }
        ImGui::EndGroup();
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginGroup();
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
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      { ImGui::TextUnformatted("[SV] Magic Skills"); }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::BeginGroup();
    {
      ImGui::BeginChild("##orpg_system_audio_files", ImVec2(), ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
      {
        ImGui::TextUnformatted("Music");
        if (ImGui::BeginTable("##system_audio_music_list", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                              ImVec2(0, App::DPIHandler::scale_value(250.f)))) {
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

        ImGui::TextUnformatted("Sounds");
        if (ImGui::BeginTable("##system_audio_sound_list", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                              ImVec2(0, App::DPIHandler::scale_value(250.f)))) {
          ImGui::TableSetupColumn("Type");
          ImGui::TableSetupColumn("File");
          ImGui::TableSetupScrollFreeze(2, 1);
          ImGui::TableHeadersRow();

          // TODO: Implement

          ImGui::EndTable();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndGroup();
  }
  ImGui::EndChild();
}
