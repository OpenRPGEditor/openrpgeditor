#include "Core/DatabaseEditor/DBTermsTab.hpp"

#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/System.hpp"

using namespace std::string_view_literals;

#include <imgui.h>
#include <imgui_internal.h>
std::tuple<bool, bool> DBTermsTab::drawAbbreviatedString(const float width, std::string_view label, std::string& full, std::string& abbrv) {
  bool fullModified = false;
  bool abbrvModified = false;
  ImGui::BeginChild(std::format("##orpg_{}_term", label).c_str(), {width * 2 + (ImGui::GetStyle().ItemSpacing.x * 2) + (ImGui::GetStyle().FramePadding.x * 2), 0},
                    ImGuiChildFlags_FrameStyle | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize);
  {
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ChildBg));
    ImGui::TextUnformatted(label.data());
    char levelStr[256];
    strncpy(levelStr, full.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_term", label).c_str(), trNOOP("Full"), levelStr, 256, width)) {
      full = levelStr;
      fullModified = true;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetStyle().ItemSpacing.y);
    char abvrStr[256];
    strncpy(abvrStr, abbrv.c_str(), 256);
    if (ImGui::LabelOverLineEdit(std::format("##{}_abbreviation_term", label).c_str(), trNOOP("Abbreviation"), abvrStr, 256, width)) {
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
  if (m_system.terms().basic().empty()) {
    m_system.terms().basic().resize(10);
  }
  if (!m_system.terms().basic()[0]) {
    m_system.terms().basic()[0] = trNOOP("Max HP");
  }
  if (!m_system.terms().basic()[1]) {
    m_system.terms().basic()[1] = trNOOP("Max MP");
  }
  if (!m_system.terms().basic()[2]) {
    m_system.terms().basic()[2] = trNOOP("Attack");
  }
  if (!m_system.terms().basic()[3]) {
    m_system.terms().basic()[3] = trNOOP("Defense");
  }
  if (!m_system.terms().basic()[4]) {
    m_system.terms().basic()[4] = trNOOP("M.Attack");
  }
  if (!m_system.terms().basic()[5]) {
    m_system.terms().basic()[5] = trNOOP("M.Defense");
  }
  if (!m_system.terms().basic()[6]) {
    m_system.terms().basic()[6] = trNOOP("Agility");
  }
  if (!m_system.terms().basic()[7]) {
    m_system.terms().basic()[7] = trNOOP("Luck");
  }
  if (!m_system.terms().basic()[8]) {
    m_system.terms().basic()[8] = trNOOP("Hit");
  }
  if (!m_system.terms().basic()[9]) {
    m_system.terms().basic()[9] = trNOOP("Evasion");
  }
}
void DBTermsTab::initializeBasicStrings() const {
  if (m_system.terms().params().empty()) {
    m_system.terms().params().resize(10);
  }
  if (!m_system.terms().params()[0]) {
    m_system.terms().params()[0] = trNOOP("Level");
  }
  if (!m_system.terms().params()[1]) {
    m_system.terms().params()[1] = trNOOP("Lv");
  }
  if (!m_system.terms().params()[2]) {
    m_system.terms().params()[2] = trNOOP("HP");
  }
  if (!m_system.terms().params()[3]) {
    m_system.terms().params()[3] = trNOOP("HP");
  }
  if (!m_system.terms().params()[4]) {
    m_system.terms().params()[4] = trNOOP("MP");
  }
  if (!m_system.terms().params()[5]) {
    m_system.terms().params()[5] = trNOOP("MP");
  }
  if (!m_system.terms().params()[6]) {
    m_system.terms().params()[6] = trNOOP("TP");
  }
  if (!m_system.terms().params()[7]) {
    m_system.terms().params()[7] = trNOOP("TP");
  }
  if (!m_system.terms().params()[8]) {
    m_system.terms().params()[8] = trNOOP("EXP");
  }
  if (!m_system.terms().params()[9]) {
    m_system.terms().params()[9] = trNOOP("EXP");
  }
}

void DBTermsTab::initializeCommandStrings() const {
  if (m_system.terms().commands().empty()) {
    m_system.terms().commands().resize(26);
  }

  if (!m_system.terms().commands()[0]) {
    m_system.terms().commands()[0] = trNOOP("Fight");
  }
  if (!m_system.terms().commands()[1]) {
    m_system.terms().commands()[1] = trNOOP("Escape");
  }
  if (!m_system.terms().commands()[2]) {
    m_system.terms().commands()[2] = trNOOP("Attack");
  }
  if (!m_system.terms().commands()[3]) {
    m_system.terms().commands()[3] = trNOOP("Guard");
  }
  if (!m_system.terms().commands()[4]) {
    m_system.terms().commands()[4] = trNOOP("Item");
  }
  if (!m_system.terms().commands()[5]) {
    m_system.terms().commands()[5] = trNOOP("Skill");
  }
  if (!m_system.terms().commands()[6]) {
    m_system.terms().commands()[6] = trNOOP("Equip");
  }
  if (!m_system.terms().commands()[7]) {
    m_system.terms().commands()[7] = trNOOP("Status");
  }
  if (!m_system.terms().commands()[8]) {
    m_system.terms().commands()[8] = trNOOP("Formation");
  }
  if (!m_system.terms().commands()[11]) {
    m_system.terms().commands()[11] = trNOOP("Options");
  }
  if (!m_system.terms().commands()[9]) {
    m_system.terms().commands()[9] = trNOOP("Save");
  }
  if (!m_system.terms().commands()[10]) {
    m_system.terms().commands()[10] = trNOOP("Game End");
  }
  if (!m_system.terms().commands()[12]) {
    m_system.terms().commands()[12] = trNOOP("Weapon");
  }
  if (!m_system.terms().commands()[13]) {
    m_system.terms().commands()[13] = trNOOP("Armor");
  }
  if (!m_system.terms().commands()[14]) {
    m_system.terms().commands()[14] = trNOOP("Key Item");
  }
  if (!m_system.terms().commands()[15]) {
    m_system.terms().commands()[15] = trNOOP("Equip");
  }
  if (!m_system.terms().commands()[16]) {
    m_system.terms().commands()[16] = trNOOP("Optimize");
  }
  if (!m_system.terms().commands()[17]) {
    m_system.terms().commands()[17] = trNOOP("Clear");
  }
  if (!m_system.terms().commands()[24]) {
    m_system.terms().commands()[24] = trNOOP("Buy");
  }
  if (!m_system.terms().commands()[25]) {
    m_system.terms().commands()[25] = trNOOP("Sell");
  }
  if (!m_system.terms().commands()[18]) {
    m_system.terms().commands()[18] = trNOOP("New Game");
  }
  if (!m_system.terms().commands()[19]) {
    m_system.terms().commands()[19] = trNOOP("Continue");
  }
  if (!m_system.terms().commands()[21]) {
    m_system.terms().commands()[21] = trNOOP("To Title");
  }
  if (!m_system.terms().commands()[22]) {
    m_system.terms().commands()[22] = trNOOP("Cancel");
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
      if (ImGui::BeginTabItem(trNOOP("Basic Statuses"))) {
        drawAbbreviatedString(width, trNOOP("Level"), *m_system.terms().basic()[0], *m_system.terms().basic()[1]);
        drawAbbreviatedString(width, trNOOP("HP"), *m_system.terms().basic()[2], *m_system.terms().basic()[3]);
        drawAbbreviatedString(width, trNOOP("MP"), *m_system.terms().basic()[4], *m_system.terms().basic()[5]);
        drawAbbreviatedString(width, trNOOP("TP"), *m_system.terms().basic()[6], *m_system.terms().basic()[7]);
        drawAbbreviatedString(width, trNOOP("EXP"), *m_system.terms().basic()[8], *m_system.terms().basic()[9]);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem(trNOOP("Parameters"))) {
        drawString(width, trNOOP("Max HP"), *m_system.terms().params()[0]);
        ImGui::SameLine();
        drawString(width, trNOOP("Max MP"), *m_system.terms().params()[1]);

        drawString(width, trNOOP("Attack"), *m_system.terms().params()[2]);
        ImGui::SameLine();
        drawString(width, trNOOP("Defense"), *m_system.terms().params()[3]);

        drawString(width, trNOOP("M.Attack"), *m_system.terms().params()[4]);
        ImGui::SameLine();
        drawString(width, trNOOP("M.Defense"), *m_system.terms().params()[5]);

        drawString(width, trNOOP("Agility"), *m_system.terms().params()[6]);
        ImGui::SameLine();
        drawString(width, trNOOP("Luck"), *m_system.terms().params()[7]);

        drawString(width, trNOOP("Hit Rate"), *m_system.terms().params()[8]);
        ImGui::SameLine();
        drawString(width, trNOOP("Evasion"), *m_system.terms().params()[9]);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem(trNOOP("Commands"))) {
        const auto commandWidth = (ImGui::GetContentRegionAvail().x / 4) - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetStyle().FramePadding.x * 2);
        drawString(commandWidth, trNOOP("Fight"), *m_system.terms().commands()[0]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Escape"), *m_system.terms().commands()[1]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Attack"), *m_system.terms().commands()[2]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Guard"), *m_system.terms().commands()[3]);

        drawString(commandWidth, trNOOP("Item"), *m_system.terms().commands()[4]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Skill"), *m_system.terms().commands()[5]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Equip (Pause Screen)"), *m_system.terms().commands()[6]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Status"), *m_system.terms().commands()[7]);

        drawString(commandWidth, trNOOP("Formation"), *m_system.terms().commands()[8]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Options"), *m_system.terms().commands()[11]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Save"), *m_system.terms().commands()[9]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Game End"), *m_system.terms().commands()[10]);

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);

        drawString(commandWidth, trNOOP("Weapon"), *m_system.terms().commands()[12]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Armor"), *m_system.terms().commands()[13]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Key Item"), *m_system.terms().commands()[14]);
        ImGui::SameLine();
        drawString(commandWidth, "Equip (Equipment Screen)", *m_system.terms().commands()[15]);

        drawString(commandWidth, trNOOP("Optimize"), *m_system.terms().commands()[16]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Clear"), *m_system.terms().commands()[17]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Buy"), *m_system.terms().commands()[24]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Sell"), *m_system.terms().commands()[25]);

        drawString(commandWidth, trNOOP("New Game"), *m_system.terms().commands()[18]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Continue"), *m_system.terms().commands()[19]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("To Title"), *m_system.terms().commands()[21]);
        ImGui::SameLine();
        drawString(commandWidth, trNOOP("Cancel"), *m_system.terms().commands()[22]);

        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem(trNOOP("Messages"))) {
        ImGui::BeginChild("##messages_child", {}, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
        {
          const auto messageWidth = (ImGui::GetContentRegionAvail().x / 3) - ImGui::GetStyle().ItemSpacing.x - (ImGui::GetStyle().FramePadding.x * 2);
          auto temp = m_system.terms().messages().alwaysDash();
          if (drawString(messageWidth, trNOOP("Always Dash"), temp)) {
            m_system.terms().messages().setAlwaysDash(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().commandRemember();
          if (drawString(messageWidth, trNOOP("Command Remember"), temp)) {
            m_system.terms().messages().setCommandRemember(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().bgmVolume();
          if (drawString(messageWidth, trNOOP("BGM Volume"), temp)) {
            m_system.terms().messages().setBgmVolume(temp);
          }
          temp = m_system.terms().messages().bgsVolume();
          if (drawString(messageWidth, trNOOP("BGS Volume"), temp)) {
            m_system.terms().messages().setBgsVolume(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().meVolume();
          if (drawString(messageWidth, trNOOP("ME Volume"), temp)) {
            m_system.terms().messages().setMeVolume(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().seVolume();
          if (drawString(messageWidth, trNOOP("SE Volume"), temp)) {
            m_system.terms().messages().setSeVolume(temp);
          }

          temp = m_system.terms().messages().possession();
          if (drawString(messageWidth, trNOOP("Possession"), temp)) {
            m_system.terms().messages().setPossession(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().expTotal();
          if (drawString(messageWidth, trNOOP("EXP Total"), temp)) {
            m_system.terms().messages().setExpTotal(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().expNext();
          if (drawString(messageWidth, trNOOP("EXP Next"), temp)) {
            m_system.terms().messages().setExpNext(temp);
          }
          temp = m_system.terms().messages().saveMessage();
          if (drawString(messageWidth, trNOOP("Save Message"), temp)) {
            m_system.terms().messages().setSaveMessage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().loadMessage();
          if (drawString(messageWidth, trNOOP("Load Message"), temp)) {
            m_system.terms().messages().setLoadMessage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().file();
          if (drawString(messageWidth, trNOOP("File"), temp)) {
            m_system.terms().messages().setFile(temp);
          }
          temp = m_system.terms().messages().partyName();
          if (drawString(messageWidth, trNOOP("Party"), temp)) {
            m_system.terms().messages().setPartyName(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().emerge();
          if (drawString(messageWidth, trNOOP("Emerge"), temp)) {
            m_system.terms().messages().setEmerge(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().preemptive();
          if (drawString(messageWidth, trNOOP("Preemptive"), temp)) {
            m_system.terms().messages().setPreemptive(temp);
          }
          temp = m_system.terms().messages().surprise();
          if (drawString(messageWidth, trNOOP("Surprise"), temp)) {
            m_system.terms().messages().setSurprise(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().escapeStart();
          if (drawString(messageWidth, trNOOP("Escape Start"), temp)) {
            m_system.terms().messages().setEscapeStart(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().escapeFailure();
          if (drawString(messageWidth, trNOOP("Escape Failure"), temp)) {
            m_system.terms().messages().setEscapeFailure(temp);
          }

          temp = m_system.terms().messages().victory();
          if (drawString(messageWidth, trNOOP("Victory"), temp)) {
            m_system.terms().messages().setVictory(temp);
          }

          ImGui::SameLine();
          temp = m_system.terms().messages().defeat();
          if (drawString(messageWidth, trNOOP("Defeat"), temp)) {
            m_system.terms().messages().setDefeat(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().obtainExp();
          if (drawString(messageWidth, trNOOP("Obtain EXP"), temp)) {
            m_system.terms().messages().setObtainExp(temp);
          }
          temp = m_system.terms().messages().obtainGold();
          if (drawString(messageWidth, trNOOP("Obtain Gold"), temp)) {
            m_system.terms().messages().setObtainGold(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().obtainItem();
          if (drawString(messageWidth, trNOOP("Obtain Item"), temp)) {
            m_system.terms().messages().setObtainItem(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().levelUp();
          if (drawString(messageWidth, trNOOP("Level Up"), temp)) {
            m_system.terms().messages().setLevelUp(temp);
          }
          temp = m_system.terms().messages().obtainSkill();
          if (drawString(messageWidth, trNOOP("Obtain Skill"), temp)) {
            m_system.terms().messages().setObtainSkill(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().useItem();
          if (drawString(messageWidth, trNOOP("Use Item"), temp)) {
            m_system.terms().messages().setUseItem(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms().messages().criticalToEnemy();
          if (drawString(messageWidth, trNOOP("Critical To Enemy"), temp)) {
            m_system.terms().messages().setCriticalToEnemy(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().criticalToActor();
          if (drawString(messageWidth, trNOOP("Critical To Actor"), temp)) {
            m_system.terms().messages().setCriticalToActor(temp);
          }

          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms().messages().actorDamage();
          if (drawString(messageWidth, trNOOP("Actor Damage"), temp)) {
            m_system.terms().messages().setActorDamage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().actorRecovery();
          if (drawString(messageWidth, trNOOP("Actor Recovery"), temp)) {
            m_system.terms().messages().setActorRecovery(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().actorGain();
          if (drawString(messageWidth, trNOOP("Actor Gain"), temp)) {
            m_system.terms().messages().setActorGain(temp);
          }
          temp = m_system.terms().messages().actorLoss();
          if (drawString(messageWidth, trNOOP("Actor Loss"), temp)) {
            m_system.terms().messages().setActorLoss(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().actorDrain();
          if (drawString(messageWidth, trNOOP("Actor Drain"), temp)) {
            m_system.terms().messages().setActorDrain(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().actorNoDamage();
          if (drawString(messageWidth, trNOOP("Actor No Damage"), temp)) {
            m_system.terms().messages().setActorNoDamage(temp);
          }
          temp = m_system.terms().messages().actorNoHit();
          if (drawString(messageWidth, trNOOP("Actor No Hit"), temp)) {
            m_system.terms().messages().setActorNoHit(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms().messages().enemyDamage();
          if (drawString(messageWidth, trNOOP("Enemy Damage"), temp)) {
            m_system.terms().messages().setEnemyDamage(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().enemyRecovery();
          if (drawString(messageWidth, trNOOP("Enemy Recovery"), temp)) {
            m_system.terms().messages().setEnemyRecovery(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().enemyGain();
          if (drawString(messageWidth, trNOOP("Enemy Gain"), temp)) {
            m_system.terms().messages().setEnemyGain(temp);
          }
          temp = m_system.terms().messages().enemyLoss();
          if (drawString(messageWidth, trNOOP("Enemy Loss"), temp)) {
            m_system.terms().messages().setEnemyLoss(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().enemyDrain();
          if (drawString(messageWidth, trNOOP("Enemy Drain"), temp)) {
            m_system.terms().messages().setEnemyDrain(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().enemyNoDamage();
          if (drawString(messageWidth, trNOOP("Enemy No Damage"), temp)) {
            m_system.terms().messages().setEnemyNoDamage(temp);
          }
          temp = m_system.terms().messages().enemyNoHit();
          if (drawString(messageWidth, trNOOP("Enemy No Hit"), temp)) {
            m_system.terms().messages().setEnemyNoHit(temp);
          }
          ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 2.f);
          temp = m_system.terms().messages().evasion();
          if (drawString(messageWidth, trNOOP("Evasion"), temp)) {
            m_system.terms().messages().setEvasion(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().magicEvasion();
          if (drawString(messageWidth, trNOOP("Magic Evasion"), temp)) {
            m_system.terms().messages().setMagicEvasion(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().magicReflection();
          if (drawString(messageWidth, trNOOP("Magic Reflection"), temp)) {
            m_system.terms().messages().setMagicReflection(temp);
          }
          temp = m_system.terms().messages().counterAttack();
          if (drawString(messageWidth, trNOOP("Counter Attack"), temp)) {
            m_system.terms().messages().setCounterAttack(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().substitute();
          if (drawString(messageWidth, trNOOP("Substitute"), temp)) {
            m_system.terms().messages().setSubstitute(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().buffAdd();
          if (drawString(messageWidth, trNOOP("Buff Add"), temp)) {
            m_system.terms().messages().setBuffAdd(temp);
          }
          temp = m_system.terms().messages().debuffAdd();
          if (drawString(messageWidth, trNOOP("Debuff Add"), temp)) {
            m_system.terms().messages().setDebuffAdd(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().buffRemove();
          if (drawString(messageWidth, trNOOP("Buff Remove"), temp)) {
            m_system.terms().messages().setBuffRemove(temp);
          }
          ImGui::SameLine();
          temp = m_system.terms().messages().actionFailure();
          if (drawString(messageWidth, trNOOP("Action Failure"), temp)) {
            m_system.terms().messages().setActionFailure(temp);
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
