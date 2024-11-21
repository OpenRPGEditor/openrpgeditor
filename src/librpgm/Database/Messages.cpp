#include "Database/Messages.hpp"

void to_json(nlohmann::ordered_json& to, const Messages& messages) {
  to = {
      {"actionFailure", messages.actionFailure},
      {"actorDamage", messages.actorDamage},
      {"actorDrain", messages.actorDrain},
      {"actorGain", messages.actorGain},
      {"actorLoss", messages.actorLoss},
      {"actorNoDamage", messages.actorNoDamage},
      {"actorNoHit", messages.actorNoHit},
      {"actorRecovery", messages.actorRecovery},
      {"alwaysDash", messages.alwaysDash},
      {"bgmVolume", messages.bgmVolume},
      {"bgsVolume", messages.bgsVolume},
      {"buffAdd", messages.buffAdd},
      {"buffRemove", messages.buffRemove},
      {"commandRemember", messages.commandRemember},
      {"counterAttack", messages.counterAttack},
      {"criticalToActor", messages.criticalToActor},
      {"criticalToEnemy", messages.criticalToEnemy},
      {"debuffAdd", messages.debuffAdd},
      {"defeat", messages.defeat},
      {"emerge", messages.emerge},
      {"enemyDamage", messages.enemyDamage},
      {"enemyDrain", messages.enemyDrain},
      {"enemyGain", messages.enemyGain},
      {"enemyLoss", messages.enemyLoss},
      {"enemyNoDamage", messages.enemyNoDamage},
      {"enemyNoHit", messages.enemyNoHit},
      {"enemyRecovery", messages.enemyRecovery},
      {"escapeFailure", messages.escapeFailure},
      {"escapeStart", messages.escapeStart},
      {"evasion", messages.evasion},
      {"expNext", messages.expNext},
      {"expTotal", messages.expTotal},
      {"file", messages.file},
      {"levelUp", messages.levelUp},
      {"loadMessage", messages.loadMessage},
      {"magicEvasion", messages.magicEvasion},
      {"magicReflection", messages.magicReflection},
      {"meVolume", messages.meVolume},
      {"obtainExp", messages.obtainExp},
      {"obtainGold", messages.obtainGold},
      {"obtainItem", messages.obtainItem},
      {"obtainSkill", messages.obtainSkill},
      {"partyName", messages.partyName},
      {"possession", messages.possession},
      {"preemptive", messages.preemptive},
      {"saveMessage", messages.saveMessage},
      {"seVolume", messages.seVolume},
      {"substitute", messages.substitute},
      {"surprise", messages.surprise},
      {"useItem", messages.useItem},
      {"victory", messages.victory},
  };
}

void from_json(const nlohmann::ordered_json& from, Messages& messages){
  messages.actionFailure = from.value("actionFailure", messages.actionFailure);
  messages.actorDamage = from.value("actorDamage", messages.actorDamage);
  messages.actorDrain = from.value("actorDrain", messages.actorDrain);
  messages.actorGain = from.value("actorGain", messages.actorGain);
  messages.actorLoss = from.value("actorLoss", messages.actorLoss);
  messages.actorNoDamage = from.value("actorNoDamage", messages.actorNoDamage);
  messages.actorRecovery = from.value("actorRecovery", messages.actorRecovery);
  messages.alwaysDash = from.value("alwaysDash", messages.alwaysDash);
  messages.bgmVolume = from.value("bgmVolume", messages.bgmVolume);
  messages.bgsVolume = from.value("bgsVolume", messages.bgsVolume);
  messages.buffAdd = from.value("buffAdd", messages.buffAdd);
  messages.buffRemove = from.value("buffRemove", messages.buffRemove);
  messages.commandRemember = from.value("commandRemember", messages.commandRemember);
  messages.counterAttack = from.value("counterAttack", messages.counterAttack);
  messages.criticalToActor = from.value("criticalToActor", messages.criticalToActor);
  messages.criticalToEnemy = from.value("criticalToEnemy", messages.criticalToEnemy);
  messages.debuffAdd = from.value("debuffAdd", messages.debuffAdd);
  messages.defeat = from.value("defeat", messages.defeat);
  messages.emerge = from.value("emerge", messages.emerge);
  messages.enemyDamage = from.value("enemyDamage", messages.enemyDamage);
  messages.enemyDrain = from.value("enemyDrain", messages.enemyDrain);
  messages.enemyGain = from.value("enemyGain", messages.enemyGain);
  messages.enemyLoss = from.value("enemyLoss", messages.enemyLoss);
  messages.enemyNoDamage = from.value("enemyNoDamage", messages.enemyNoDamage);
  messages.enemyNoHit = from.value("enemyNoHit", messages.enemyNoHit);
  messages.enemyRecovery = from.value("enemyRecovery", messages.enemyRecovery),
  messages.escapeFailure = from.value("escapeFailure", messages.escapeFailure),
  messages.escapeStart = from.value("escapeStart", messages.escapeStart);
  messages.evasion = from.value("evasion", messages.evasion);
  messages.expNext = from.value("expNext", messages.expNext);
  messages.expTotal = from.value("expTotal", messages.expTotal);
  messages.file = from.value("file", messages.file);
  messages.levelUp = from.value("levelUp", messages.levelUp);
  messages.loadMessage = from.value("loadMessage", messages.loadMessage);
  messages.magicEvasion = from.value("magicEvasion", messages.magicEvasion);
  messages.magicReflection = from.value("magicReflection", messages.magicReflection);
  messages.meVolume = from.value("meVolume", messages.meVolume);
  messages.obtainExp = from.value("obtainExp", messages.obtainExp);
  messages.obtainGold = from.value("obtainGold", messages.obtainGold);
  messages.obtainItem = from.value("obtainItem", messages.obtainItem);
  messages.obtainSkill = from.value("obtainSkill", messages.obtainSkill);
  messages.partyName = from.value("partyName", messages.partyName);
  messages.possession = from.value("possession", messages.possession);
  messages.preemptive = from.value("preemptive", messages.preemptive);
  messages.saveMessage = from.value("saveMessage", messages.saveMessage);
  messages.seVolume = from.value("seVolume", messages.seVolume);
  messages.substitute = from.value("substitute", messages.substitute);
  messages.surprise = from.value("surprise", messages.surprise);
  messages.useItem = from.value("useItem", messages.useItem);
  messages.victory = from.value("victory", messages.victory);
}
