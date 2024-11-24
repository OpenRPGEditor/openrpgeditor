#pragma once
#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Messages {
  friend void to_json(nlohmann::ordered_json& to, const Messages& messages);
  friend void from_json(const nlohmann::ordered_json& from, Messages& messages);

  std::string actionFailure;
  std::string actorDamage;
  std::string actorDrain;
  std::string actorGain;
  std::string actorLoss;
  std::string actorNoDamage;
  std::string actorNoHit;
  std::string actorRecovery;
  std::string alwaysDash;
  std::string bgmVolume;
  std::string bgsVolume;
  std::string buffAdd;
  std::string buffRemove;
  std::string commandRemember;
  std::string counterAttack;
  std::string criticalToActor;
  std::string criticalToEnemy;
  std::string debuffAdd;
  std::string defeat;
  std::string emerge;
  std::string enemyDamage;
  std::string enemyDrain;
  std::string enemyGain;
  std::string enemyLoss;
  std::string enemyNoDamage;
  std::string enemyNoHit;
  std::string enemyRecovery;
  std::string escapeFailure;
  std::string escapeStart;
  std::string evasion;
  std::string expNext;
  std::string expTotal;
  std::string file;
  std::string levelUp;
  std::string loadMessage;
  std::string magicEvasion;
  std::string magicReflection;
  std::string meVolume;
  std::string obtainExp;
  std::string obtainGold;
  std::string obtainItem;
  std::string obtainSkill;
  std::string partyName;
  std::string possession;
  std::string preemptive;
  std::string saveMessage;
  std::string seVolume;
  std::string substitute;
  std::string surprise;
  std::string useItem;
  std::string victory;
};

void to_json(nlohmann::ordered_json& to, const Messages& messages);
void from_json(const nlohmann::ordered_json& from, Messages& messages);