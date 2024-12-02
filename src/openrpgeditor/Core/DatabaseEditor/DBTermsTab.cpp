#include "Core/DatabaseEditor/DBTermsTab.hpp"

#include "Core/DPIHandler.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/System.hpp"

using namespace std::string_view_literals;

#include <imgui.h>
#include <imgui_internal.h>
void DBTermsTab::drawAbbreviatedString(const float width, const std::string_view label, std::string& full, std::string& abbrv) {
  ImGui::BeginChild(std::format("##orpg_{}_term", label.data()).c_str(), {width * 2 + (ImGui::GetStyle().ItemSpacing.x * 2) + (ImGui::GetStyle().FramePadding.x * 2), 0},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
  {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    ImGui::TextUnformatted(label.data());
    char levelStr[256];
    strncpy(levelStr, full.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_term", label).c_str(), "Full", levelStr, 256, width)) {
      full = levelStr;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
    char abvrStr[256];
    strncpy(abvrStr, abbrv.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_abbreviation_term", label).c_str(), "Abbreviation", abvrStr, 256, width)) {
      abbrv = abvrStr;
    }
    ImGui::PopStyleColor();
  }
  ImGui::EndChild();
}

void DBTermsTab::drawString(const float width, const std::string_view label, std::string& full) {
  ImGui::BeginChild(std::format("##orpg_{}_term", label.data()).c_str(), {width + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x, 0},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
  {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    char levelStr[256];
    strncpy(levelStr, full.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_term", label).c_str(), label.data(), levelStr, 256, width)) {
      full = levelStr;
    }
    ImGui::PopStyleColor();
  }
  ImGui::EndChild();
}

void DBTermsTab::initializeParameterStrings() const {
  if (m_system.terms.basic.empty()) {
    m_system.terms.basic.resize(10);
  }
  if (!m_system.terms.basic[0]) {
    m_system.terms.basic[0] = "Max HP";
  }
  if (!m_system.terms.basic[1]) {
    m_system.terms.basic[1] = "Max MP";
  }
  if (!m_system.terms.basic[2]) {
    m_system.terms.basic[2] = "Attack";
  }
  if (!m_system.terms.basic[3]) {
    m_system.terms.basic[3] = "Defense";
  }
  if (!m_system.terms.basic[4]) {
    m_system.terms.basic[4] = "M.Attack";
  }
  if (!m_system.terms.basic[5]) {
    m_system.terms.basic[5] = "M.Defense";
  }
  if (!m_system.terms.basic[6]) {
    m_system.terms.basic[6] = "Agility";
  }
  if (!m_system.terms.basic[7]) {
    m_system.terms.basic[7] = "Luck";
  }
  if (!m_system.terms.basic[8]) {
    m_system.terms.basic[8] = "Hit";
  }
  if (!m_system.terms.basic[9]) {
    m_system.terms.basic[9] = "Evasion";
  }
}
void DBTermsTab::initializeBasicStrings() const {
  if (m_system.terms.params.empty()) {
    m_system.terms.params.resize(10);
  }
  if (!m_system.terms.params[0]) {
    m_system.terms.params[0] = "Level";
  }
  if (!m_system.terms.params[1]) {
    m_system.terms.params[1] = "Lv";
  }
  if (!m_system.terms.params[2]) {
    m_system.terms.params[2] = "HP";
  }
  if (!m_system.terms.params[3]) {
    m_system.terms.params[3] = "HP";
  }
  if (!m_system.terms.params[4]) {
    m_system.terms.params[4] = "MP";
  }
  if (!m_system.terms.params[5]) {
    m_system.terms.params[5] = "MP";
  }
  if (!m_system.terms.params[6]) {
    m_system.terms.params[6] = "TP";
  }
  if (!m_system.terms.params[7]) {
    m_system.terms.params[7] = "TP";
  }
  if (!m_system.terms.params[8]) {
    m_system.terms.params[8] = "EXP";
  }
  if (!m_system.terms.params[9]) {
    m_system.terms.params[9] = "EXP";
  }
}

void DBTermsTab::initializeCommandStrings() const {
  if (m_system.terms.commands.empty()) {
    m_system.terms.commands.resize(26);
  }

  if (!m_system.terms.commands[0]) {
    m_system.terms.commands[0] = "Fight";
  }
  if (!m_system.terms.commands[1]) {
    m_system.terms.commands[1] = "Escape";
  }
  if (!m_system.terms.commands[2]) {
    m_system.terms.commands[2] = "Attack";
  }
  if (!m_system.terms.commands[3]) {
    m_system.terms.commands[3] = "Guard";
  }
  if (!m_system.terms.commands[4]) {
    m_system.terms.commands[4] = "Item";
  }
  if (!m_system.terms.commands[5]) {
    m_system.terms.commands[5] = "Skill";
  }
  if (!m_system.terms.commands[6]) {
    m_system.terms.commands[6] = "Equip";
  }
  if (!m_system.terms.commands[7]) {
    m_system.terms.commands[7] = "Status";
  }
  if (!m_system.terms.commands[8]) {
    m_system.terms.commands[8] = "Formation";
  }
  if (!m_system.terms.commands[11]) {
    m_system.terms.commands[11] = "Options";
  }
  if (!m_system.terms.commands[9]) {
    m_system.terms.commands[9] = "Save";
  }
  if (!m_system.terms.commands[10]) {
    m_system.terms.commands[10] = "Game End";
  }
  if (!m_system.terms.commands[12]) {
    m_system.terms.commands[12] = "Weapon";
  }
  if (!m_system.terms.commands[13]) {
    m_system.terms.commands[13] = "Armor";
  }
  if (!m_system.terms.commands[14]) {
    m_system.terms.commands[14] = "Key Item";
  }
  if (!m_system.terms.commands[15]) {
    m_system.terms.commands[15] = "Equip";
  }
  if (!m_system.terms.commands[16]) {
    m_system.terms.commands[16] = "Optimize";
  }
  if (!m_system.terms.commands[17]) {
    m_system.terms.commands[17] = "Clear";
  }
  if (!m_system.terms.commands[24]) {
    m_system.terms.commands[24] = "Buy";
  }
  if (!m_system.terms.commands[25]) {
    m_system.terms.commands[25] = "Sell";
  }
  if (!m_system.terms.commands[18]) {
    m_system.terms.commands[18] = "New Game";
  }
  if (!m_system.terms.commands[19]) {
    m_system.terms.commands[19] = "Continue";
  }
  if (!m_system.terms.commands[21]) {
    m_system.terms.commands[21] = "To Title";
  }
  if (!m_system.terms.commands[22]) {
    m_system.terms.commands[22] = "Cancel";
  }
}
void DBTermsTab::draw() {

  initializeBasicStrings();

  initializeParameterStrings();

  initializeCommandStrings();

  ImGui::BeginChild("##orpg_terms");
  {
    const auto width = (ImGui::GetContentRegionAvail().x / 2) - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetStyle().FramePadding.x * 2);
    if (ImGui::BeginTabBar("##orpg_terms_tab_bar")) {
      if (ImGui::BeginTabItem("Basic Statuses")) {
        drawAbbreviatedString(width, "Level"sv, *m_system.terms.basic[0], *m_system.terms.basic[1]);
        drawAbbreviatedString(width, "HP"sv, *m_system.terms.basic[2], *m_system.terms.basic[3]);
        drawAbbreviatedString(width, "MP"sv, *m_system.terms.basic[4], *m_system.terms.basic[5]);
        drawAbbreviatedString(width, "TP"sv, *m_system.terms.basic[6], *m_system.terms.basic[7]);
        drawAbbreviatedString(width, "EXP"sv, *m_system.terms.basic[8], *m_system.terms.basic[9]);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Parameters")) {
        drawString(width, "Max HP", *m_system.terms.params[0]);
        ImGui::SameLine();
        drawString(width, "Max MP", *m_system.terms.params[1]);

        drawString(width, "Attack", *m_system.terms.params[2]);
        ImGui::SameLine();
        drawString(width, "Defense", *m_system.terms.params[3]);

        drawString(width, "M.Attack", *m_system.terms.params[4]);
        ImGui::SameLine();
        drawString(width, "M.Defense", *m_system.terms.params[5]);

        drawString(width, "Agility", *m_system.terms.params[6]);
        ImGui::SameLine();
        drawString(width, "Luck", *m_system.terms.params[7]);

        drawString(width, "Hit Rate", *m_system.terms.params[8]);
        ImGui::SameLine();
        drawString(width, "Evasion", *m_system.terms.params[9]);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Commands")) {
        const auto commandWidth = (ImGui::GetContentRegionAvail().x / 4) - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetStyle().FramePadding.x * 2);
        drawString(commandWidth, "Fight", *m_system.terms.commands[0]);
        ImGui::SameLine();
        drawString(commandWidth, "Escape", *m_system.terms.commands[1]);
        ImGui::SameLine();
        drawString(commandWidth, "Attack", *m_system.terms.commands[2]);
        ImGui::SameLine();
        drawString(commandWidth, "Guard", *m_system.terms.commands[3]);

        drawString(commandWidth, "Item", *m_system.terms.commands[4]);
        ImGui::SameLine();
        drawString(commandWidth, "Skill", *m_system.terms.commands[5]);
        ImGui::SameLine();
        drawString(commandWidth, "Equip (Pause Screen)", *m_system.terms.commands[6]);
        ImGui::SameLine();
        drawString(commandWidth, "Status", *m_system.terms.commands[7]);

        drawString(commandWidth, "Formation", *m_system.terms.commands[8]);
        ImGui::SameLine();
        drawString(commandWidth, "Options", *m_system.terms.commands[11]);
        ImGui::SameLine();
        drawString(commandWidth, "Save", *m_system.terms.commands[9]);
        ImGui::SameLine();
        drawString(commandWidth, "Game End", *m_system.terms.commands[10]);

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, App::DPIHandler::scale_value(2.f));

        drawString(commandWidth, "Weapon", *m_system.terms.commands[12]);
        ImGui::SameLine();
        drawString(commandWidth, "Armor", *m_system.terms.commands[13]);
        ImGui::SameLine();
        drawString(commandWidth, "Key Item", *m_system.terms.commands[14]);
        ImGui::SameLine();
        drawString(commandWidth, "Equip (Equipment Screen)", *m_system.terms.commands[15]);

        drawString(commandWidth, "Optimize", *m_system.terms.commands[16]);
        ImGui::SameLine();
        drawString(commandWidth, "Clear", *m_system.terms.commands[17]);
        ImGui::SameLine();
        drawString(commandWidth, "Buy", *m_system.terms.commands[24]);
        ImGui::SameLine();
        drawString(commandWidth, "Sell", *m_system.terms.commands[25]);

        drawString(commandWidth, "New Game", *m_system.terms.commands[18]);
        ImGui::SameLine();
        drawString(commandWidth, "Continue", *m_system.terms.commands[19]);
        ImGui::SameLine();
        drawString(commandWidth, "To Title", *m_system.terms.commands[21]);
        ImGui::SameLine();
        drawString(commandWidth, "Cancel", *m_system.terms.commands[22]);

        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Messages")) {
        ImGui::BeginChild("##messages_child", {}, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        {
          const auto messageWidth = (ImGui::GetContentRegionAvail().x / 3) - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetStyle().FramePadding.x * 2);
          drawString(messageWidth, "Always Dash", m_system.terms.messages.alwaysDash);
          ImGui::SameLine();
          drawString(messageWidth, "Command Remember", m_system.terms.messages.commandRemember);
          ImGui::SameLine();
          drawString(messageWidth, "BGM Volume", m_system.terms.messages.bgmVolume);

          drawString(messageWidth, "BGS Volume", m_system.terms.messages.bgsVolume);
          ImGui::SameLine();
          drawString(messageWidth, "ME Volume", m_system.terms.messages.meVolume);
          ImGui::SameLine();
          drawString(messageWidth, "SE Volume", m_system.terms.messages.seVolume);

          drawString(messageWidth, "Possession", m_system.terms.messages.possession);
          ImGui::SameLine();
          drawString(messageWidth, "EXP Total", m_system.terms.messages.expTotal);
          ImGui::SameLine();
          drawString(messageWidth, "EXP Next", m_system.terms.messages.expNext);

          drawString(messageWidth, "Save Message", m_system.terms.messages.saveMessage);
          ImGui::SameLine();
          drawString(messageWidth, "Load Message", m_system.terms.messages.loadMessage);
          ImGui::SameLine();
          drawString(messageWidth, "File", m_system.terms.messages.file);

          drawString(messageWidth, "Party", m_system.terms.messages.partyName);
          ImGui::SameLine();
          drawString(messageWidth, "Emerge", m_system.terms.messages.emerge);
          ImGui::SameLine();
          drawString(messageWidth, "Preemptive", m_system.terms.messages.preemptive);

          drawString(messageWidth, "Surprise", m_system.terms.messages.surprise);
          ImGui::SameLine();
          drawString(messageWidth, "Escape Start", m_system.terms.messages.escapeStart);
          ImGui::SameLine();
          drawString(messageWidth, "Escape Failure", m_system.terms.messages.escapeFailure);

          drawString(messageWidth, "Victory", m_system.terms.messages.victory);
          ImGui::SameLine();
          drawString(messageWidth, "Defeat", m_system.terms.messages.defeat);
          ImGui::SameLine();
          drawString(messageWidth, "Obtain EXP", m_system.terms.messages.obtainExp);

          drawString(messageWidth, "Obtain Gold", m_system.terms.messages.obtainGold);
          ImGui::SameLine();
          drawString(messageWidth, "Obtain Item", m_system.terms.messages.obtainItem);
          ImGui::SameLine();
          drawString(messageWidth, "Level Up", m_system.terms.messages.levelUp);

          drawString(messageWidth, "Obtain Skill", m_system.terms.messages.obtainSkill);
          ImGui::SameLine();
          drawString(messageWidth, "Use Item", m_system.terms.messages.useItem);

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, App::DPIHandler::scale_value(2.f));
          drawString(messageWidth, "Critical To Enemy", m_system.terms.messages.criticalToEnemy);
          ImGui::SameLine();
          drawString(messageWidth, "Critical To Actor", m_system.terms.messages.criticalToActor);

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, App::DPIHandler::scale_value(2.f));
          drawString(messageWidth, "Actor Damage", m_system.terms.messages.actorDamage);
          ImGui::SameLine();
          drawString(messageWidth, "Actor Recovery", m_system.terms.messages.actorRecovery);
          ImGui::SameLine();
          drawString(messageWidth, "Actor Gain", m_system.terms.messages.actorGain);

          drawString(messageWidth, "Actor Loss", m_system.terms.messages.actorLoss);
          ImGui::SameLine();
          drawString(messageWidth, "Actor Drain", m_system.terms.messages.actorDrain);
          ImGui::SameLine();
          drawString(messageWidth, "Actor No Damage", m_system.terms.messages.actorNoDamage);

          drawString(messageWidth, "Actor No Hit", m_system.terms.messages.actorNoHit);

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, App::DPIHandler::scale_value(2.f));
          drawString(messageWidth, "Enemy Damage", m_system.terms.messages.enemyDamage);
          ImGui::SameLine();
          drawString(messageWidth, "Enemy Recovery", m_system.terms.messages.enemyRecovery);
          ImGui::SameLine();
          drawString(messageWidth, "Enemy Gain", m_system.terms.messages.enemyGain);

          drawString(messageWidth, "Enemy Loss", m_system.terms.messages.enemyLoss);
          ImGui::SameLine();
          drawString(messageWidth, "Enemy Drain", m_system.terms.messages.enemyDrain);
          ImGui::SameLine();
          drawString(messageWidth, "Enemy No Damage", m_system.terms.messages.enemyNoDamage);

          drawString(messageWidth, "Enemy No Hit", m_system.terms.messages.enemyNoHit);

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, App::DPIHandler::scale_value(2.f));
          drawString(messageWidth, "Evasion", m_system.terms.messages.evasion);
          ImGui::SameLine();
          drawString(messageWidth, "Magic Evasion", m_system.terms.messages.magicEvasion);
          ImGui::SameLine();
          drawString(messageWidth, "Magic Reflection", m_system.terms.messages.magicReflection);

          drawString(messageWidth, "Counter Attack", m_system.terms.messages.counterAttack);
          ImGui::SameLine();
          drawString(messageWidth, "Substitute", m_system.terms.messages.substitute);
          ImGui::SameLine();
          drawString(messageWidth, "Buff Add", m_system.terms.messages.buffAdd);

          drawString(messageWidth, "Debuff Add", m_system.terms.messages.debuffAdd);
          ImGui::SameLine();
          drawString(messageWidth, "Buff Remove", m_system.terms.messages.buffRemove);
          ImGui::SameLine();
          drawString(messageWidth, "Action Failure", m_system.terms.messages.actionFailure);
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::EndChild();
}
