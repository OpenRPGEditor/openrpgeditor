#include "Core/DatabaseEditor/DBTermsTab.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/System.hpp"

using namespace std::string_view_literals;

#include <imgui.h>
#include <imgui_internal.h>
std::tuple<bool, bool> DBTermsTab::drawAbbreviatedString(const float width, const std::string_view label, std::string& full, std::string& abbrv) {
  bool fullModified = false;
  bool abbrvModified = false;
  ImGui::BeginChild(std::format("##orpg_{}_term", label.data()).c_str(), {width * 2 + (ImGui::GetStyle().ItemSpacing.x * 2) + (ImGui::GetStyle().FramePadding.x * 2), 0},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
  {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    ImGui::TextUnformatted(label.data());
    char levelStr[256];
    strncpy(levelStr, full.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_term", label).c_str(), "Full", levelStr, 256, width)) {
      full = levelStr;
      fullModified = true;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
    char abvrStr[256];
    strncpy(abvrStr, abbrv.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_abbreviation_term", label).c_str(), "Abbreviation", abvrStr, 256, width)) {
      abbrv = abvrStr;
      abbrvModified = true;
    }
    ImGui::PopStyleColor();
  }
  ImGui::EndChild();
  return {fullModified, abbrvModified};
}

bool DBTermsTab::drawString(const float width, const std::string_view label, std::string& full) {
  bool result = false;
  ImGui::BeginChild(std::format("##orpg_{}_term", label.data()).c_str(), {width + ImGui::GetStyle().ItemSpacing.x + ImGui::GetStyle().FramePadding.x, 0},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
  {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    char levelStr[256];
    strncpy(levelStr, full.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_term", label).c_str(), label.data(), levelStr, 256, width)) {
      full = levelStr;
      result = true;
    }
    ImGui::PopStyleColor();
  }
  ImGui::EndChild();
  return result;
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

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);

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
          auto temp = m_system.terms.messages.alwaysDash();
          if (drawString(messageWidth, "Always Dash", temp)) {
            m_system.terms.messages.setAlwaysDash(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.commandRemember();
          if (drawString(messageWidth, "Command Remember", temp)) {
            m_system.terms.messages.setCommandRemember(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.bgmVolume();
          if (drawString(messageWidth, "BGM Volume", temp)) {
            m_system.terms.messages.setBgmVolume(temp);
          }
          temp = m_system.terms.messages.bgsVolume();
          if (drawString(messageWidth, "BGS Volume", temp)) {
            m_system.terms.messages.setBgsVolume(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.meVolume();
          if (drawString(messageWidth, "ME Volume", temp)) {
            m_system.terms.messages.setMeVolume(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.seVolume();
          if (drawString(messageWidth, "SE Volume", temp)) {
            m_system.terms.messages.setSeVolume(temp);
          }

          temp = m_system.terms.messages.possession();
          if (drawString(messageWidth, "Possession", temp)) {
            m_system.terms.messages.setPossession(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.expTotal();
          if (drawString(messageWidth, "EXP Total", temp)) {
            m_system.terms.messages.setExpTotal(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.expNext();
          if (drawString(messageWidth, "EXP Next", temp)) {
            m_system.terms.messages.setExpNext(temp);
          }
          temp = m_system.terms.messages.saveMessage();
          if (drawString(messageWidth, "Save Message", temp)) {
            m_system.terms.messages.setSaveMessage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.loadMessage();
          if (drawString(messageWidth, "Load Message", temp)) {
            m_system.terms.messages.setLoadMessage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.file();
          if (drawString(messageWidth, "File", temp)) {
            m_system.terms.messages.setFile(temp);
          }
          temp = m_system.terms.messages.partyName();
          if (drawString(messageWidth, "Party", temp)) {
            m_system.terms.messages.setPartyName(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.emerge();
          if (drawString(messageWidth, "Emerge", temp)) {
            m_system.terms.messages.setEmerge(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.preemptive();
          if (drawString(messageWidth, "Preemptive", temp)) {
            m_system.terms.messages.setPreemptive(temp);
          }
          temp = m_system.terms.messages.surprise();
          if (drawString(messageWidth, "Surprise", temp)) {
            m_system.terms.messages.setSurprise(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.escapeStart();
          if (drawString(messageWidth, "Escape Start", temp)) {
            m_system.terms.messages.setEscapeStart(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.escapeFailure();
          if (drawString(messageWidth, "Escape Failure", temp)) {
            m_system.terms.messages.setEscapeFailure(temp);
          }

          temp = m_system.terms.messages.victory();
          if (drawString(messageWidth, "Victory", temp)) {
            m_system.terms.messages.setVictory(temp);
          }

          ImGui::SameLine();
          temp = m_system.terms.messages.defeat();
          if (drawString(messageWidth, "Defeat", temp)) {
            m_system.terms.messages.setDefeat(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.obtainExp();
          if (drawString(messageWidth, "Obtain EXP", temp)) {
            m_system.terms.messages.setObtainExp(temp);
          }
          temp = m_system.terms.messages.obtainGold();
          if (drawString(messageWidth, "Obtain Gold", temp)) {
            m_system.terms.messages.setObtainGold(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.obtainItem();
          if (drawString(messageWidth, "Obtain Item", temp)) {
            m_system.terms.messages.setObtainItem(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.levelUp();
          if (drawString(messageWidth, "Level Up", temp)) {
            m_system.terms.messages.setLevelUp(temp);
          }
          temp = m_system.terms.messages.obtainSkill();
          if (drawString(messageWidth, "Obtain Skill", temp)) {
            m_system.terms.messages.setObtainSkill(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.useItem();
          if (drawString(messageWidth, "Use Item", temp)) {
            m_system.terms.messages.setUseItem(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms.messages.criticalToEnemy();
          if (drawString(messageWidth, "Critical To Enemy", temp)) {
            m_system.terms.messages.setCriticalToEnemy(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.criticalToActor();
          if (drawString(messageWidth, "Critical To Actor", temp)) {
            m_system.terms.messages.setCriticalToActor(temp);
          }

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms.messages.actorDamage();
          if (drawString(messageWidth, "Actor Damage", temp)) {
            m_system.terms.messages.setActorDamage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.actorRecovery();
          if (drawString(messageWidth, "Actor Recovery", temp)) {
            m_system.terms.messages.setActorRecovery(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.actorGain();
          if (drawString(messageWidth, "Actor Gain", temp)) {
            m_system.terms.messages.setActorGain(temp);
          }
          temp = m_system.terms.messages.actorLoss();
          if (drawString(messageWidth, "Actor Loss", temp)) {
            m_system.terms.messages.setActorLoss(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.actorDrain();
          if (drawString(messageWidth, "Actor Drain", temp)) {
            m_system.terms.messages.setActorDrain(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.actorNoDamage();
          if (drawString(messageWidth, "Actor No Damage", temp)) {
            m_system.terms.messages.setActorNoDamage(temp);
          }
          temp = m_system.terms.messages.actorNoHit();
          if (drawString(messageWidth, "Actor No Hit", temp)) {
            m_system.terms.messages.setActorNoHit(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms.messages.enemyDamage();
          if (drawString(messageWidth, "Enemy Damage", temp)) {
            m_system.terms.messages.setEnemyDamage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.enemyRecovery();
          if (drawString(messageWidth, "Enemy Recovery", temp)) {
            m_system.terms.messages.setEnemyRecovery(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.enemyGain();
          if (drawString(messageWidth, "Enemy Gain", temp)) {
            m_system.terms.messages.setEnemyGain(temp);
          }
          temp = m_system.terms.messages.enemyLoss();
          if (drawString(messageWidth, "Enemy Loss", temp)) {
            m_system.terms.messages.setEnemyLoss(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.enemyDrain();
          if (drawString(messageWidth, "Enemy Drain", temp)) {
            m_system.terms.messages.setEnemyDrain(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.enemyNoDamage();
          if (drawString(messageWidth, "Enemy No Damage", temp)) {
            m_system.terms.messages.setEnemyNoDamage(temp);
          }
          temp = m_system.terms.messages.enemyNoHit();
          if (drawString(messageWidth, "Enemy No Hit", temp)) {
            m_system.terms.messages.setEnemyNoHit(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms.messages.evasion();
          if (drawString(messageWidth, "Evasion", temp)) {
            m_system.terms.messages.setEvasion(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.magicEvasion();
          if (drawString(messageWidth, "Magic Evasion", temp)) {
            m_system.terms.messages.setMagicEvasion(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.magicReflection();
          if (drawString(messageWidth, "Magic Reflection", temp)) {
            m_system.terms.messages.setMagicReflection(temp);
          }
          temp = m_system.terms.messages.counterAttack();
          if (drawString(messageWidth, "Counter Attack", temp)) {
            m_system.terms.messages.setCounterAttack(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.substitute();
          if (drawString(messageWidth, "Substitute", temp)) {
            m_system.terms.messages.setSubstitute(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.buffAdd();
          if (drawString(messageWidth, "Buff Add", temp)) {
            m_system.terms.messages.setBuffAdd(temp);
          }
          temp = m_system.terms.messages.debuffAdd();
          if (drawString(messageWidth, "Debuff Add", temp)) {
            m_system.terms.messages.setDebuffAdd(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.buffRemove();
          if (drawString(messageWidth, "Buff Remove", temp)) {
            m_system.terms.messages.setBuffRemove(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms.messages.actionFailure();
          if (drawString(messageWidth, "Action Failure", temp)) {
            m_system.terms.messages.setActionFailure(temp);
          }
        }
        ImGui::EndChild();
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::EndChild();
}
