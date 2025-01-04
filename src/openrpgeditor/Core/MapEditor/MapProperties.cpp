#include "Core/MapEditor/MapProperties.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"

#include <Database/MapInfo.hpp>

#include <imgui.h>
#include <imgui_internal.h>

Map* MapProperties::map() const { return m_mapInfo->map(); }

void MapProperties::resizeMap(const int width, const int height) const {
  if (map()) {
    map()->resize(width, height);
    /*! TODO: IEventEditor in Database with client implementation in Core
     * This will allow us to give Event a pointer to an editor object
     * without having the implementation in Database itself
     * it also gives us the ability to maintain the editor instance *with* the event
     * keeping allowing us to properly implement animations
     */
    // for (auto it = m_eventEditors.begin(); it != m_eventEditors.end();) {
    //   if (map()->event(it->id()) == nullptr) {
    //     it = m_eventEditors.erase(it);
    //   } else {
    //     ++it;
    //   }
    // }
  }
}

std::tuple<bool, bool> MapProperties::draw() {
  if (!m_open || !m_mapInfo || !map()) {
    return {!m_open, m_confirmed};
  }

  if (ImGui::Begin("Map Properties", &m_open)) {
    char buf[4096]{};
    ImGui::BeginGroupPanel();
    {
      ImGui::SeparatorText("General Settings");
      ImGui::BeginGroup();
      {
        ImGui::Text("Name - ID: %.03i", m_mapInfo->id());
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
        strncpy(buf, m_mapInfo->name().c_str(), 4096);
        if (ImGui::InputText("##map_name", buf, 4096)) {
          m_mapInfo->setName(buf);
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        // Move back up a couple couple pixels
        cursorPos.y -= 4.f;
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Display Name");
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
        strncpy(buf, map()->displayName().c_str(), 4096);
        if (ImGui::InputText("##map_display_name", buf, 4096)) {
          map()->setDisplayName(buf);
        }
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::Text("Tileset");
        strncpy(buf, m_mapInfo->name().c_str(), 4096);
        const auto& tilesetName = Database::instance()->tilesets.tileset(map()->tilesetId())->name();
        ImGui::PushID("##map_tileset_button");
        if (ImGui::Button((tilesetName.empty() ? "##map_tileset_button_empty" : tilesetName).c_str(), ImVec2{ImGui::GetContentRegionMax().x / 2 - 15, 0})) {}
        ImGui::PopID();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        // Move back up a couple pixels
        cursorPos.y -= 4.f;
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Width");
        ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
        ImGui::DragInt("##map_width", &m_tempWidth, 0, 0, 256);
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        // Move back up a couple pixels
        cursorPos.y -= 4.f;
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Height");
        ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - 15);
        ImGui::DragInt("##map_height", &m_tempHeight, 0, 0, 256);
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        const auto cursorPos = ImGui::GetCursorPos();
        // Move back up a couple pixels
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Scroll Type");
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
        if (ImGui::BeginCombo("##map_scroll_type", DecodeEnumName(magic_enum::enum_name(map()->scrollType())).c_str())) {
          for (const auto& e : magic_enum::enum_values<ScrollType>()) {
            if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(e)).c_str(), e == map()->scrollType())) {
              map()->setScrollType(e);
            }
            if (e == map()->scrollType()) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        // Move back up a couple pixels
        cursorPos.y -= 4.f;
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Enc. Steps");
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
        auto encounterStep = map()->encounterStep();
        if (ImGui::DragInt("##map_enc_steps", &encounterStep, 0, 0, 999)) {
          map()->setEncounterStep(encounterStep);
        }
      }
      ImGui::EndGroup();
    }
    ImGui::Separator();
    {
      ImGui::BeginGroup();
      {
        bool autoplayBgm = map()->autoplayBgm();
        if (ImGui::Checkbox("Autoplay BGM", &autoplayBgm)) {
          map()->setAutoplayBgm(autoplayBgm);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->autoplayBgm());
        {
          ImGui::PushID("##map_bgm_button");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 30);
          std::string text = map()->bgm().name.empty() ? "##map_bgm_button_empty" : map()->bgm().name;
          if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetContentRegionMax().x / 2) - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        bool autoplayBgs = map()->autoplayBgs();
        if (ImGui::Checkbox("Autoplay BGS", &autoplayBgs)) {
          map()->setAutoplayBgs(autoplayBgs);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->autoplayBgs());
        {
          ImGui::PushID("##map_bgs_button");
          std::string text = map()->bgs().name.empty() ? "##map_bgs_button_empty" : map()->bgs().name;
          if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetContentRegionMax().x / 2) - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        bool specifyBattleback = map()->specifyBattleback();
        if (ImGui::Checkbox("Battleback", &specifyBattleback)) {
          map()->setSpecifyBattleback(specifyBattleback);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->specifyBattleback());
        {
          ImGui::PushID("##map_battleback_button");
          // TODO: Combine battleBack1Name and battleBack2Name
          std::string text = map()->bgs().name.empty() ? "##map_battleback_button_empty" : map()->battleback1Name();
          if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetContentRegionMax().x / 2) - 15, 0})) {}
          ImGui::PopID();
        }
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        bool disableDashing = map()->disableDashing();
        if (ImGui::Checkbox("Disable Dashing", &disableDashing)) {
          map()->setDisableDashing(disableDashing);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->disableDashing());
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
    }
    ImGui::Separator();
    {
      ImGui::BeginGroup();
      {
        ImGui::Text("Parallax Background");
        ImGui::PushID("##map_parallax_button");
        std::string text = map()->parallaxName().empty() ? "##map_parallax_button_empty" : map()->parallaxName();
        if (ImGui::Button(text.c_str(), ImVec2{(ImGui::GetContentRegionMax().x / 2) - 15, 0})) {}
        ImGui::PopID();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        bool parallaxLoopX = map()->parallaxLoopX();
        if (ImGui::Checkbox("Loop Horizontally", &parallaxLoopX)) {
          map()->setParallaxLoopX(parallaxLoopX);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxLoopX());
        {
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          int parallaxSx = map()->parallaxSx();
          if (ImGui::DragInt("##map_parallax_Sx", &parallaxSx, 0, 0, 999)) {
            map()->setParallaxSx(parallaxSx);
          }
        }
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::SameLine();
      ImGui::BeginGroup();
      {
        bool parallaxLoopY = map()->parallaxLoopY();
        if (ImGui::Checkbox("Loop Vertically", &parallaxLoopY)) {
          map()->setParallaxLoopY(parallaxLoopY);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxLoopY());
        {
          int parallaxSy = map()->parallaxSy();
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 15);
          if (ImGui::DragInt("##map_parallax_Sy", &parallaxSy, 0, 0, 999)) {
            map()->setParallaxSy(parallaxSy);
          }
        }
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        bool parallaxShow = map()->parallaxShow();
        if (ImGui::Checkbox("Show in Editor", &parallaxShow)) {
          map()->setParallaxShow(parallaxShow);
        }
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !map()->parallaxShow());
        ImGui::PopItemFlag();
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        constexpr ImGuiInputTextFlags flags = ImGuiInputTextFlags_CtrlEnterForNewLine;
        // Move back up a couple pixels
        cursorPos.y -= 4.f;
        ImGui::SetCursorPos(cursorPos);
        ImGui::Text("Note");

        strncpy(buf, map()->note().c_str(), 4096);
        if (ImGui::InputTextMultiline("##map_note", buf, 2048, ImVec2(ImGui::GetContentRegionMax().x - 15, 400), flags)) {
          map()->setNote(buf);
        }
      }
      ImGui::Dummy(ImVec2(0.0f, 15.0f));
      ImGui::EndGroup();
    }
    ImGui::EndGroupPanel();
  }
  ImGui::End();

  return {!m_open, m_confirmed};
}
