#pragma once
#include "Database/Globals.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>
#include <string>

struct Messages {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
      Messages, actionFailure, actorDamage, actorDrain, actorGain, actorLoss, actorNoDamage, actorNoHit, actorRecovery,
      alwaysDash, bgmVolume, bgsVolume, buffAdd, buffRemove, commandRemember, counterAttack, criticalToActor,
      criticalToEnemy, debuffAdd, defeat, emerge, enemyDamage, enemyDrain, enemyGain, enemyLoss, enemyNoDamage,
      enemyNoHit, enemyRecovery, escapeFailure, escapeStart, evasion, expNext, expTotal, file, levelUp, loadMessage,
      magicEvasion, magicReflection, meVolume, obtainExp, btainGold, obtainItem, obtainSkil, partyName, possession,
      preemptive, saveMessage, seVolume, substitute, surprise, useItem, victory);
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
  std::string btainGold;
  std::string obtainItem;
  std::string obtainSkil;
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