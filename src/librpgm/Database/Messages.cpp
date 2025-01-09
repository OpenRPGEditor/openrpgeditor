#include "Database/Messages.hpp"
Messages::Messages(const Messages& other)
: IModifiable(other)
, m_actionFailure(other.m_actionFailure)
, m_actorDamage(other.m_actorDamage)
, m_actorDrain(other.m_actorDrain)
, m_actorGain(other.m_actorGain)
, m_actorLoss(other.m_actorLoss)
, m_actorNoDamage(other.m_actorNoDamage)
, m_actorNoHit(other.m_actorNoHit)
, m_actorRecovery(other.m_actorRecovery)
, m_alwaysDash(other.m_alwaysDash)
, m_bgmVolume(other.m_bgmVolume)
, m_bgsVolume(other.m_bgsVolume)
, m_buffAdd(other.m_buffAdd)
, m_buffRemove(other.m_buffRemove)
, m_commandRemember(other.m_commandRemember)
, m_counterAttack(other.m_counterAttack)
, m_criticalToActor(other.m_criticalToActor)
, m_criticalToEnemy(other.m_criticalToEnemy)
, m_debuffAdd(other.m_debuffAdd)
, m_defeat(other.m_defeat)
, m_emerge(other.m_emerge)
, m_enemyDamage(other.m_enemyDamage)
, m_enemyDrain(other.m_enemyDrain)
, m_enemyGain(other.m_enemyGain)
, m_enemyLoss(other.m_enemyLoss)
, m_enemyNoDamage(other.m_enemyNoDamage)
, m_enemyNoHit(other.m_enemyNoHit)
, m_enemyRecovery(other.m_enemyRecovery)
, m_escapeFailure(other.m_escapeFailure)
, m_escapeStart(other.m_escapeStart)
, m_evasion(other.m_evasion)
, m_expNext(other.m_expNext)
, m_expTotal(other.m_expTotal)
, m_file(other.m_file)
, m_levelUp(other.m_levelUp)
, m_loadMessage(other.m_loadMessage)
, m_magicEvasion(other.m_magicEvasion)
, m_magicReflection(other.m_magicReflection)
, m_meVolume(other.m_meVolume)
, m_obtainExp(other.m_obtainExp)
, m_obtainGold(other.m_obtainGold)
, m_obtainItem(other.m_obtainItem)
, m_obtainSkill(other.m_obtainSkill)
, m_partyName(other.m_partyName)
, m_possession(other.m_possession)
, m_preemptive(other.m_preemptive)
, m_saveMessage(other.m_saveMessage)
, m_seVolume(other.m_seVolume)
, m_substitute(other.m_substitute)
, m_surprise(other.m_surprise)
, m_useItem(other.m_useItem)
, m_victory(other.m_victory)
, m_oldactionFailure(other.m_actionFailure)
, m_oldactorDamage(other.m_actorDamage)
, m_oldactorDrain(other.m_actorDrain)
, m_oldactorGain(other.m_actorGain)
, m_oldactorLoss(other.m_actorLoss)
, m_oldactorNoDamage(other.m_actorNoDamage)
, m_oldactorNoHit(other.m_actorNoHit)
, m_oldactorRecovery(other.m_actorRecovery)
, m_oldalwaysDash(other.m_alwaysDash)
, m_oldbgmVolume(other.m_bgmVolume)
, m_oldbgsVolume(other.m_bgsVolume)
, m_oldbuffAdd(other.m_buffAdd)
, m_oldbuffRemove(other.m_buffRemove)
, m_oldcommandRemember(other.m_commandRemember)
, m_oldcounterAttack(other.m_counterAttack)
, m_oldcriticalToActor(other.m_criticalToActor)
, m_oldcriticalToEnemy(other.m_criticalToEnemy)
, m_olddebuffAdd(other.m_debuffAdd)
, m_olddefeat(other.m_defeat)
, m_oldemerge(other.m_emerge)
, m_oldenemyDamage(other.m_enemyDamage)
, m_oldenemyDrain(other.m_enemyDrain)
, m_oldenemyGain(other.m_enemyGain)
, m_oldenemyLoss(other.m_enemyLoss)
, m_oldenemyNoDamage(other.m_enemyNoDamage)
, m_oldenemyNoHit(other.m_enemyNoHit)
, m_oldenemyRecovery(other.m_enemyRecovery)
, m_oldescapeFailure(other.m_escapeFailure)
, m_oldescapeStart(other.m_escapeStart)
, m_oldevasion(other.m_evasion)
, m_oldexpNext(other.m_expNext)
, m_oldexpTotal(other.m_expTotal)
, m_oldfile(other.m_file)
, m_oldlevelUp(other.m_levelUp)
, m_oldloadMessage(other.m_loadMessage)
, m_oldmagicEvasion(other.m_magicEvasion)
, m_oldmagicReflection(other.m_magicReflection)
, m_oldmeVolume(other.m_meVolume)
, m_oldobtainExp(other.m_obtainExp)
, m_oldobtainGold(other.m_obtainGold)
, m_oldobtainItem(other.m_obtainItem)
, m_oldobtainSkill(other.m_obtainSkill)
, m_oldpartyName(other.m_partyName)
, m_oldpossession(other.m_possession)
, m_oldpreemptive(other.m_preemptive)
, m_oldsaveMessage(other.m_saveMessage)
, m_oldseVolume(other.m_seVolume)
, m_oldsubstitute(other.m_substitute)
, m_oldsurprise(other.m_surprise)
, m_olduseItem(other.m_useItem)
, m_oldvictory(other.m_victory) {}
Messages& Messages::operator=(const Messages& other) {
  IModifiable::operator=(other);
  m_actionFailure = other.m_actionFailure;
  m_actorDamage = other.m_actorDamage;
  m_actorDrain = other.m_actorDrain;
  m_actorGain = other.m_actorGain;
  m_actorLoss = other.m_actorLoss;
  m_actorNoDamage = other.m_actorNoDamage;
  m_actorNoHit = other.m_actorNoHit;
  m_actorRecovery = other.m_actorRecovery;
  m_alwaysDash = other.m_alwaysDash;
  m_bgmVolume = other.m_bgmVolume;
  m_bgsVolume = other.m_bgsVolume;
  m_buffAdd = other.m_buffAdd;
  m_buffRemove = other.m_buffRemove;
  m_commandRemember = other.m_commandRemember;
  m_counterAttack = other.m_counterAttack;
  m_criticalToActor = other.m_criticalToActor;
  m_criticalToEnemy = other.m_criticalToEnemy;
  m_debuffAdd = other.m_debuffAdd;
  m_defeat = other.m_defeat;
  m_emerge = other.m_emerge;
  m_enemyDamage = other.m_enemyDamage;
  m_enemyDrain = other.m_enemyDrain;
  m_enemyGain = other.m_enemyGain;
  m_enemyLoss = other.m_enemyLoss;
  m_enemyNoDamage = other.m_enemyNoDamage;
  m_enemyNoHit = other.m_enemyNoHit;
  m_enemyRecovery = other.m_enemyRecovery;
  m_escapeFailure = other.m_escapeFailure;
  m_escapeStart = other.m_escapeStart;
  m_evasion = other.m_evasion;
  m_expNext = other.m_expNext;
  m_expTotal = other.m_expTotal;
  m_file = other.m_file;
  m_levelUp = other.m_levelUp;
  m_loadMessage = other.m_loadMessage;
  m_magicEvasion = other.m_magicEvasion;
  m_magicReflection = other.m_magicReflection;
  m_meVolume = other.m_meVolume;
  m_obtainExp = other.m_obtainExp;
  m_obtainGold = other.m_obtainGold;
  m_obtainItem = other.m_obtainItem;
  m_obtainSkill = other.m_obtainSkill;
  m_partyName = other.m_partyName;
  m_possession = other.m_possession;
  m_preemptive = other.m_preemptive;
  m_saveMessage = other.m_saveMessage;
  m_seVolume = other.m_seVolume;
  m_substitute = other.m_substitute;
  m_surprise = other.m_surprise;
  m_useItem = other.m_useItem;
  m_victory = other.m_victory;
  m_oldactionFailure = other.m_oldactionFailure;
  m_oldactorDamage = other.m_oldactorDamage;
  m_oldactorDrain = other.m_oldactorDrain;
  m_oldactorGain = other.m_oldactorGain;
  m_oldactorLoss = other.m_oldactorLoss;
  m_oldactorNoDamage = other.m_oldactorNoDamage;
  m_oldactorNoHit = other.m_oldactorNoHit;
  m_oldactorRecovery = other.m_oldactorRecovery;
  m_oldalwaysDash = other.m_oldalwaysDash;
  m_oldbgmVolume = other.m_oldbgmVolume;
  m_oldbgsVolume = other.m_oldbgsVolume;
  m_oldbuffAdd = other.m_oldbuffAdd;
  m_oldbuffRemove = other.m_oldbuffRemove;
  m_oldcommandRemember = other.m_oldcommandRemember;
  m_oldcounterAttack = other.m_oldcounterAttack;
  m_oldcriticalToActor = other.m_oldcriticalToActor;
  m_oldcriticalToEnemy = other.m_oldcriticalToEnemy;
  m_olddebuffAdd = other.m_olddebuffAdd;
  m_olddefeat = other.m_olddefeat;
  m_oldemerge = other.m_oldemerge;
  m_oldenemyDamage = other.m_oldenemyDamage;
  m_oldenemyDrain = other.m_oldenemyDrain;
  m_oldenemyGain = other.m_oldenemyGain;
  m_oldenemyLoss = other.m_oldenemyLoss;
  m_oldenemyNoDamage = other.m_oldenemyNoDamage;
  m_oldenemyNoHit = other.m_oldenemyNoHit;
  m_oldenemyRecovery = other.m_oldenemyRecovery;
  m_oldescapeFailure = other.m_oldescapeFailure;
  m_oldescapeStart = other.m_oldescapeStart;
  m_oldevasion = other.m_oldevasion;
  m_oldexpNext = other.m_oldexpNext;
  m_oldexpTotal = other.m_oldexpTotal;
  m_oldfile = other.m_oldfile;
  m_oldlevelUp = other.m_oldlevelUp;
  m_oldloadMessage = other.m_oldloadMessage;
  m_oldmagicEvasion = other.m_oldmagicEvasion;
  m_oldmagicReflection = other.m_oldmagicReflection;
  m_oldmeVolume = other.m_oldmeVolume;
  m_oldobtainExp = other.m_oldobtainExp;
  m_oldobtainGold = other.m_oldobtainGold;
  m_oldobtainItem = other.m_oldobtainItem;
  m_oldobtainSkill = other.m_oldobtainSkill;
  m_oldpartyName = other.m_oldpartyName;
  m_oldpossession = other.m_oldpossession;
  m_oldpreemptive = other.m_oldpreemptive;
  m_oldsaveMessage = other.m_oldsaveMessage;
  m_oldseVolume = other.m_oldseVolume;
  m_oldsubstitute = other.m_oldsubstitute;
  m_oldsurprise = other.m_oldsurprise;
  m_olduseItem = other.m_olduseItem;
  m_oldvictory = other.m_oldvictory;
  return *this;
}
Messages::Messages(Messages&& other) noexcept
: IModifiable(std::move(other))
, m_actionFailure(std::move(other.m_actionFailure))
, m_actorDamage(std::move(other.m_actorDamage))
, m_actorDrain(std::move(other.m_actorDrain))
, m_actorGain(std::move(other.m_actorGain))
, m_actorLoss(std::move(other.m_actorLoss))
, m_actorNoDamage(std::move(other.m_actorNoDamage))
, m_actorNoHit(std::move(other.m_actorNoHit))
, m_actorRecovery(std::move(other.m_actorRecovery))
, m_alwaysDash(std::move(other.m_alwaysDash))
, m_bgmVolume(std::move(other.m_bgmVolume))
, m_bgsVolume(std::move(other.m_bgsVolume))
, m_buffAdd(std::move(other.m_buffAdd))
, m_buffRemove(std::move(other.m_buffRemove))
, m_commandRemember(std::move(other.m_commandRemember))
, m_counterAttack(std::move(other.m_counterAttack))
, m_criticalToActor(std::move(other.m_criticalToActor))
, m_criticalToEnemy(std::move(other.m_criticalToEnemy))
, m_debuffAdd(std::move(other.m_debuffAdd))
, m_defeat(std::move(other.m_defeat))
, m_emerge(std::move(other.m_emerge))
, m_enemyDamage(std::move(other.m_enemyDamage))
, m_enemyDrain(std::move(other.m_enemyDrain))
, m_enemyGain(std::move(other.m_enemyGain))
, m_enemyLoss(std::move(other.m_enemyLoss))
, m_enemyNoDamage(std::move(other.m_enemyNoDamage))
, m_enemyNoHit(std::move(other.m_enemyNoHit))
, m_enemyRecovery(std::move(other.m_enemyRecovery))
, m_escapeFailure(std::move(other.m_escapeFailure))
, m_escapeStart(std::move(other.m_escapeStart))
, m_evasion(std::move(other.m_evasion))
, m_expNext(std::move(other.m_expNext))
, m_expTotal(std::move(other.m_expTotal))
, m_file(std::move(other.m_file))
, m_levelUp(std::move(other.m_levelUp))
, m_loadMessage(std::move(other.m_loadMessage))
, m_magicEvasion(std::move(other.m_magicEvasion))
, m_magicReflection(std::move(other.m_magicReflection))
, m_meVolume(std::move(other.m_meVolume))
, m_obtainExp(std::move(other.m_obtainExp))
, m_obtainGold(std::move(other.m_obtainGold))
, m_obtainItem(std::move(other.m_obtainItem))
, m_obtainSkill(std::move(other.m_obtainSkill))
, m_partyName(std::move(other.m_partyName))
, m_possession(std::move(other.m_possession))
, m_preemptive(std::move(other.m_preemptive))
, m_saveMessage(std::move(other.m_saveMessage))
, m_seVolume(std::move(other.m_seVolume))
, m_substitute(std::move(other.m_substitute))
, m_surprise(std::move(other.m_surprise))
, m_useItem(std::move(other.m_useItem))
, m_victory(std::move(other.m_victory))
, m_oldactionFailure(std::move(other.m_actionFailure))
, m_oldactorDamage(std::move(other.m_actorDamage))
, m_oldactorDrain(std::move(other.m_actorDrain))
, m_oldactorGain(std::move(other.m_actorGain))
, m_oldactorLoss(std::move(other.m_actorLoss))
, m_oldactorNoDamage(std::move(other.m_actorNoDamage))
, m_oldactorNoHit(std::move(other.m_actorNoHit))
, m_oldactorRecovery(std::move(other.m_actorRecovery))
, m_oldalwaysDash(std::move(other.m_alwaysDash))
, m_oldbgmVolume(std::move(other.m_bgmVolume))
, m_oldbgsVolume(std::move(other.m_bgsVolume))
, m_oldbuffAdd(std::move(other.m_buffAdd))
, m_oldbuffRemove(std::move(other.m_buffRemove))
, m_oldcommandRemember(std::move(other.m_commandRemember))
, m_oldcounterAttack(std::move(other.m_counterAttack))
, m_oldcriticalToActor(std::move(other.m_criticalToActor))
, m_oldcriticalToEnemy(std::move(other.m_criticalToEnemy))
, m_olddebuffAdd(std::move(other.m_debuffAdd))
, m_olddefeat(std::move(other.m_defeat))
, m_oldemerge(std::move(other.m_emerge))
, m_oldenemyDamage(std::move(other.m_enemyDamage))
, m_oldenemyDrain(std::move(other.m_enemyDrain))
, m_oldenemyGain(std::move(other.m_enemyGain))
, m_oldenemyLoss(std::move(other.m_enemyLoss))
, m_oldenemyNoDamage(std::move(other.m_enemyNoDamage))
, m_oldenemyNoHit(std::move(other.m_enemyNoHit))
, m_oldenemyRecovery(std::move(other.m_enemyRecovery))
, m_oldescapeFailure(std::move(other.m_escapeFailure))
, m_oldescapeStart(std::move(other.m_escapeStart))
, m_oldevasion(std::move(other.m_evasion))
, m_oldexpNext(std::move(other.m_expNext))
, m_oldexpTotal(std::move(other.m_expTotal))
, m_oldfile(std::move(other.m_file))
, m_oldlevelUp(std::move(other.m_levelUp))
, m_oldloadMessage(std::move(other.m_loadMessage))
, m_oldmagicEvasion(std::move(other.m_magicEvasion))
, m_oldmagicReflection(std::move(other.m_magicReflection))
, m_oldmeVolume(std::move(other.m_meVolume))
, m_oldobtainExp(std::move(other.m_obtainExp))
, m_oldobtainGold(std::move(other.m_obtainGold))
, m_oldobtainItem(std::move(other.m_obtainItem))
, m_oldobtainSkill(std::move(other.m_obtainSkill))
, m_oldpartyName(std::move(other.m_partyName))
, m_oldpossession(std::move(other.m_possession))
, m_oldpreemptive(std::move(other.m_preemptive))
, m_oldsaveMessage(std::move(other.m_saveMessage))
, m_oldseVolume(std::move(other.m_seVolume))
, m_oldsubstitute(std::move(other.m_substitute))
, m_oldsurprise(std::move(other.m_surprise))
, m_olduseItem(std::move(other.m_useItem))
, m_oldvictory(std::move(other.m_victory)) {}

Messages& Messages::operator=(Messages&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_actionFailure = std::move(other.m_actionFailure);
  m_actorDamage = std::move(other.m_actorDamage);
  m_actorDrain = std::move(other.m_actorDrain);
  m_actorGain = std::move(other.m_actorGain);
  m_actorLoss = std::move(other.m_actorLoss);
  m_actorNoDamage = std::move(other.m_actorNoDamage);
  m_actorNoHit = std::move(other.m_actorNoHit);
  m_actorRecovery = std::move(other.m_actorRecovery);
  m_alwaysDash = std::move(other.m_alwaysDash);
  m_bgmVolume = std::move(other.m_bgmVolume);
  m_bgsVolume = std::move(other.m_bgsVolume);
  m_buffAdd = std::move(other.m_buffAdd);
  m_buffRemove = std::move(other.m_buffRemove);
  m_commandRemember = std::move(other.m_commandRemember);
  m_counterAttack = std::move(other.m_counterAttack);
  m_criticalToActor = std::move(other.m_criticalToActor);
  m_criticalToEnemy = std::move(other.m_criticalToEnemy);
  m_debuffAdd = std::move(other.m_debuffAdd);
  m_defeat = std::move(other.m_defeat);
  m_emerge = std::move(other.m_emerge);
  m_enemyDamage = std::move(other.m_enemyDamage);
  m_enemyDrain = std::move(other.m_enemyDrain);
  m_enemyGain = std::move(other.m_enemyGain);
  m_enemyLoss = std::move(other.m_enemyLoss);
  m_enemyNoDamage = std::move(other.m_enemyNoDamage);
  m_enemyNoHit = std::move(other.m_enemyNoHit);
  m_enemyRecovery = std::move(other.m_enemyRecovery);
  m_escapeFailure = std::move(other.m_escapeFailure);
  m_escapeStart = std::move(other.m_escapeStart);
  m_evasion = std::move(other.m_evasion);
  m_expNext = std::move(other.m_expNext);
  m_expTotal = std::move(other.m_expTotal);
  m_file = std::move(other.m_file);
  m_levelUp = std::move(other.m_levelUp);
  m_loadMessage = std::move(other.m_loadMessage);
  m_magicEvasion = std::move(other.m_magicEvasion);
  m_magicReflection = std::move(other.m_magicReflection);
  m_meVolume = std::move(other.m_meVolume);
  m_obtainExp = std::move(other.m_obtainExp);
  m_obtainGold = std::move(other.m_obtainGold);
  m_obtainItem = std::move(other.m_obtainItem);
  m_obtainSkill = std::move(other.m_obtainSkill);
  m_partyName = std::move(other.m_partyName);
  m_possession = std::move(other.m_possession);
  m_preemptive = std::move(other.m_preemptive);
  m_saveMessage = std::move(other.m_saveMessage);
  m_seVolume = std::move(other.m_seVolume);
  m_substitute = std::move(other.m_substitute);
  m_surprise = std::move(other.m_surprise);
  m_useItem = std::move(other.m_useItem);
  m_victory = std::move(other.m_victory);
  m_oldactionFailure = std::move(other.m_oldactionFailure);
  m_oldactorDamage = std::move(other.m_oldactorDamage);
  m_oldactorDrain = std::move(other.m_oldactorDrain);
  m_oldactorGain = std::move(other.m_oldactorGain);
  m_oldactorLoss = std::move(other.m_oldactorLoss);
  m_oldactorNoDamage = std::move(other.m_oldactorNoDamage);
  m_oldactorNoHit = std::move(other.m_oldactorNoHit);
  m_oldactorRecovery = std::move(other.m_oldactorRecovery);
  m_oldalwaysDash = std::move(other.m_oldalwaysDash);
  m_oldbgmVolume = std::move(other.m_oldbgmVolume);
  m_oldbgsVolume = std::move(other.m_oldbgsVolume);
  m_oldbuffAdd = std::move(other.m_oldbuffAdd);
  m_oldbuffRemove = std::move(other.m_oldbuffRemove);
  m_oldcommandRemember = std::move(other.m_oldcommandRemember);
  m_oldcounterAttack = std::move(other.m_oldcounterAttack);
  m_oldcriticalToActor = std::move(other.m_oldcriticalToActor);
  m_oldcriticalToEnemy = std::move(other.m_oldcriticalToEnemy);
  m_olddebuffAdd = std::move(other.m_olddebuffAdd);
  m_olddefeat = std::move(other.m_olddefeat);
  m_oldemerge = std::move(other.m_oldemerge);
  m_oldenemyDamage = std::move(other.m_oldenemyDamage);
  m_oldenemyDrain = std::move(other.m_oldenemyDrain);
  m_oldenemyGain = std::move(other.m_oldenemyGain);
  m_oldenemyLoss = std::move(other.m_oldenemyLoss);
  m_oldenemyNoDamage = std::move(other.m_oldenemyNoDamage);
  m_oldenemyNoHit = std::move(other.m_oldenemyNoHit);
  m_oldenemyRecovery = std::move(other.m_oldenemyRecovery);
  m_oldescapeFailure = std::move(other.m_oldescapeFailure);
  m_oldescapeStart = std::move(other.m_oldescapeStart);
  m_oldevasion = std::move(other.m_oldevasion);
  m_oldexpNext = std::move(other.m_oldexpNext);
  m_oldexpTotal = std::move(other.m_oldexpTotal);
  m_oldfile = std::move(other.m_oldfile);
  m_oldlevelUp = std::move(other.m_oldlevelUp);
  m_oldloadMessage = std::move(other.m_oldloadMessage);
  m_oldmagicEvasion = std::move(other.m_oldmagicEvasion);
  m_oldmagicReflection = std::move(other.m_oldmagicReflection);
  m_oldmeVolume = std::move(other.m_oldmeVolume);
  m_oldobtainExp = std::move(other.m_oldobtainExp);
  m_oldobtainGold = std::move(other.m_oldobtainGold);
  m_oldobtainItem = std::move(other.m_oldobtainItem);
  m_oldobtainSkill = std::move(other.m_oldobtainSkill);
  m_oldpartyName = std::move(other.m_oldpartyName);
  m_oldpossession = std::move(other.m_oldpossession);
  m_oldpreemptive = std::move(other.m_oldpreemptive);
  m_oldsaveMessage = std::move(other.m_oldsaveMessage);
  m_oldseVolume = std::move(other.m_oldseVolume);
  m_oldsubstitute = std::move(other.m_oldsubstitute);
  m_oldsurprise = std::move(other.m_oldsurprise);
  m_olduseItem = std::move(other.m_olduseItem);
  m_oldvictory = std::move(other.m_oldvictory);
  return *this;
}

const std::string& Messages::actionFailure() { return m_actionFailure; }
void Messages::setActionFailure(const std::string& value) {
  if (m_actionFailure == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actionFailure);
  if (!signalsDisabled()) {
    actionFailureModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorDamage() { return m_actorDamage; }
void Messages::setActorDamage(const std::string& value) {
  if (m_actorDamage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorDamage);
  m_actorDamage = value;
  if (!signalsDisabled()) {
    actorDamageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorDrain() { return m_actorDrain; }
void Messages::setActorDrain(const std::string& value) {
  if (m_actorDrain == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorDrain);
  m_actorDrain = value;
  if (!signalsDisabled()) {
    actorDrainModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorGain() { return m_actorGain; }
void Messages::setActorGain(const std::string& value) {
  if (m_actorGain == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorGain);
  m_actorGain = value;
  if (!signalsDisabled()) {
    actorGainModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorLoss() { return m_actorLoss; }
void Messages::setActorLoss(const std::string& value) {
  if (m_actorLoss == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorLoss);
  m_actorLoss = value;
  if (!signalsDisabled()) {
    actorLossModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorNoDamage() { return m_actorNoDamage; }
void Messages::setActorNoDamage(const std::string& value) {
  if (m_actorNoDamage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorNoDamage);
  m_actorNoDamage = value;
  if (!signalsDisabled()) {
    actorNoDamageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorNoHit() { return m_actorNoHit; }
void Messages::setActorNoHit(const std::string& value) {
  if (m_actorNoHit == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorNoHit);
  m_actorNoHit = value;
  if (!signalsDisabled()) {
    actorNoHitModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::actorRecovery() { return m_actorRecovery; }
void Messages::setActorRecovery(const std::string& value) {
  if (m_actorRecovery == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(actorRecovery);
  m_actorRecovery = value;
  if (!signalsDisabled()) {
    actorRecoveryModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::alwaysDash() { return m_alwaysDash; }
void Messages::setAlwaysDash(const std::string& value) {
  if (m_alwaysDash == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(alwaysDash);
  m_alwaysDash = value;
  if (!signalsDisabled()) {
    alwaysDashModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::bgmVolume() { return m_bgmVolume; }
void Messages::setBgmVolume(const std::string& value) {
  if (m_bgmVolume == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(bgmVolume);
  m_bgmVolume = value;
  if (!signalsDisabled()) {
    bgmVolumeModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::bgsVolume() { return m_bgsVolume; }
void Messages::setBgsVolume(const std::string& value) {
  if (m_bgsVolume == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(bgsVolume);
  m_bgsVolume = value;
  if (!signalsDisabled()) {
    bgsVolumeModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::buffAdd() { return m_buffAdd; }
void Messages::setBuffAdd(const std::string& value) {
  if (m_buffAdd == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(buffAdd);
  m_buffAdd = value;
  if (!signalsDisabled()) {
    buffAddModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::buffRemove() { return m_buffRemove; }
void Messages::setBuffRemove(const std::string& value) {
  if (m_buffRemove == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(buffRemove);
  m_buffRemove = value;
  if (!signalsDisabled()) {
    buffRemoveModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::commandRemember() { return m_commandRemember; }
void Messages::setCommandRemember(const std::string& value) {
  if (m_commandRemember == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(commandRemember);
  m_commandRemember = value;
  if (!signalsDisabled()) {
    commandRememberModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::counterAttack() { return m_counterAttack; }
void Messages::setCounterAttack(const std::string& value) {
  if (m_counterAttack == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(counterAttack);
  m_counterAttack = value;
  if (!signalsDisabled()) {
    counterAttackModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::criticalToActor() { return m_criticalToActor; }
void Messages::setCriticalToActor(const std::string& value) {
  if (m_criticalToActor == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(criticalToActor);
  m_criticalToActor = value;
  if (!signalsDisabled()) {
    criticalToActorModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::criticalToEnemy() { return m_criticalToEnemy; }
void Messages::setCriticalToEnemy(const std::string& value) {
  if (m_criticalToEnemy == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(criticalToEnemy);
  m_criticalToEnemy = value;
  if (!signalsDisabled()) {
    criticalToEnemyModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::debuffAdd() { return m_debuffAdd; }
void Messages::setDebuffAdd(const std::string& value) {
  if (m_debuffAdd == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(debuffAdd);
  m_debuffAdd = value;
  if (!signalsDisabled()) {
    debuffAddModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::defeat() { return m_defeat; }
void Messages::setDefeat(const std::string& value) {
  if (m_defeat == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(defeat);
  m_defeat = value;
  if (!signalsDisabled()) {
    defeatModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::emerge() { return m_emerge; }
void Messages::setEmerge(const std::string& value) {
  if (m_emerge == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(emerge);
  m_emerge = value;
  if (!signalsDisabled()) {
    emergeModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyDamage() { return m_enemyDamage; }
void Messages::setEnemyDamage(const std::string& value) {
  if (m_enemyDamage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyDamage);
  m_enemyDamage = value;
  if (!signalsDisabled()) {
    enemyDamageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyDrain() { return m_enemyDrain; }
void Messages::setEnemyDrain(const std::string& value) {
  if (m_enemyDrain == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyDrain);
  m_enemyDrain = value;
  if (!signalsDisabled()) {
    enemyDrainModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyGain() { return m_enemyGain; }
void Messages::setEnemyGain(const std::string& value) {
  if (m_enemyGain == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyGain);
  m_enemyGain = value;
  if (!signalsDisabled()) {
    enemyGainModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyLoss() { return m_enemyLoss; }
void Messages::setEnemyLoss(const std::string& value) {
  if (m_enemyLoss == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyLoss);
  m_enemyLoss = value;
  if (!signalsDisabled()) {
    enemyLossModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyNoDamage() { return m_enemyNoDamage; }
void Messages::setEnemyNoDamage(const std::string& value) {
  if (m_enemyNoDamage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyNoDamage);
  m_enemyNoDamage = value;
  if (!signalsDisabled()) {
    enemyNoDamageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyNoHit() { return m_enemyNoHit; }
void Messages::setEnemyNoHit(const std::string& value) {
  if (m_enemyNoHit == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyNoHit);
  m_enemyNoHit = value;
  if (!signalsDisabled()) {
    enemyNoHitModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::enemyRecovery() { return m_enemyRecovery; }
void Messages::setEnemyRecovery(const std::string& value) {
  if (m_enemyRecovery == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(enemyRecovery);
  m_enemyRecovery = value;
  if (!signalsDisabled()) {
    enemyRecoveryModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::escapeFailure() { return m_escapeFailure; }
void Messages::setEscapeFailure(const std::string& value) {
  if (m_escapeFailure == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(escapeFailure);
  m_escapeFailure = value;
  if (!signalsDisabled()) {
    escapeFailureModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::escapeStart() { return m_escapeStart; }
void Messages::setEscapeStart(const std::string& value) {
  if (m_escapeStart == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(escapeStart);
  m_escapeStart = value;
  if (!signalsDisabled()) {
    escapeStartModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::evasion() { return m_evasion; }
void Messages::setEvasion(const std::string& value) {
  if (m_evasion == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(evasion);
  m_evasion = value;
  if (!signalsDisabled()) {
    evasionModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::expNext() { return m_expNext; }
void Messages::setExpNext(const std::string& value) {
  if (m_expNext == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(expNext);
  m_expNext = value;
  if (!signalsDisabled()) {
    expNextModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::expTotal() { return m_expTotal; }
void Messages::setExpTotal(const std::string& value) {
  if (m_expTotal == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(expTotal);
  m_expTotal = value;
  if (!signalsDisabled()) {
    expTotalModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::file() { return m_file; }
void Messages::setFile(const std::string& value) {
  if (m_file == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(file);
  m_file = value;
  if (!signalsDisabled()) {
    fileModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::levelUp() { return m_levelUp; }
void Messages::setLevelUp(const std::string& value) {
  if (m_levelUp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(levelUp);
  m_levelUp = value;
  if (!signalsDisabled()) {
    levelUpModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::loadMessage() { return m_loadMessage; }
void Messages::setLoadMessage(const std::string& value) {
  if (m_loadMessage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(loadMessage);
  m_loadMessage = value;
  if (!signalsDisabled()) {
    loadMessageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::magicEvasion() { return m_magicEvasion; }
void Messages::setMagicEvasion(const std::string& value) {
  if (m_magicEvasion == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(magicEvasion);
  m_magicEvasion = value;
  if (!signalsDisabled()) {
    magicEvasionModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::magicReflection() { return m_magicReflection; }
void Messages::setMagicReflection(const std::string& value) {
  if (m_magicReflection == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(magicReflection);
  m_magicReflection = value;
  if (!signalsDisabled()) {
    magicReflectionModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::meVolume() { return m_meVolume; }
void Messages::setMeVolume(const std::string& value) {
  if (m_meVolume == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(meVolume);
  m_meVolume = value;
  if (!signalsDisabled()) {
    meVolumeModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::obtainExp() { return m_obtainExp; }
void Messages::setObtainExp(const std::string& value) {
  if (m_obtainExp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(obtainExp);
  m_obtainExp = value;
  if (!signalsDisabled()) {
    obtainExpModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::obtainGold() { return m_obtainGold; }
void Messages::setObtainGold(const std::string& value) {
  if (m_obtainGold == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(obtainGold);
  m_obtainGold = value;
  if (!signalsDisabled()) {
    obtainGoldModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::obtainItem() { return m_obtainItem; }
void Messages::setObtainItem(const std::string& value) {
  if (m_obtainItem == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(obtainItem);
  m_obtainItem = value;
  if (!signalsDisabled()) {
    obtainItemModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::obtainSkill() { return m_obtainSkill; }
void Messages::setObtainSkill(const std::string& value) {
  if (m_obtainSkill == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(obtainSkill);
  m_obtainSkill = value;
  if (!signalsDisabled()) {
    obtainSkillModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::partyName() { return m_partyName; }
void Messages::setPartyName(const std::string& value) {
  if (m_partyName == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(partyName);
}
const std::string& Messages::possession() { return m_possession; }
void Messages::setPossession(const std::string& value) {
  if (m_possession == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(possession);
  m_possession = value;
  if (!signalsDisabled()) {
    possessionModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::preemptive() { return m_preemptive; }
void Messages::setPreemptive(const std::string& value) {
  if (m_preemptive == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(preemptive);
  m_preemptive = value;
  if (!signalsDisabled()) {
    preemptiveModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::saveMessage() { return m_saveMessage; }
void Messages::setSaveMessage(const std::string& value) {
  if (m_saveMessage == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(saveMessage);
  m_saveMessage = value;
  if (!signalsDisabled()) {
    saveMessageModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::seVolume() { return m_seVolume; }
void Messages::setSeVolume(const std::string& value) {
  if (m_seVolume == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(seVolume);
  m_seVolume = value;
  if (!signalsDisabled()) {
    seVolumeModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::substitute() { return m_substitute; }
void Messages::setSubstitute(const std::string& value) {
  if (m_substitute == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(substitute);
  m_substitute = value;
  if (!signalsDisabled()) {
    substituteModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::surprise() { return m_surprise; }
void Messages::setSurprise(const std::string& value) {
  if (m_surprise == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(surprise);
  m_surprise = value;
  if (!signalsDisabled()) {
    surpriseModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::useItem() { return m_useItem; }
void Messages::setUseItem(const std::string& value) {
  if (m_useItem == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(useItem);
  m_useItem = value;
  if (!signalsDisabled()) {
    useItemModified().fire(this, value);
  }
  setHasChanges();
}
const std::string& Messages::victory() { return m_victory; }
void Messages::setVictory(const std::string& value) {
  if (m_victory == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(victory);
  m_victory = value;
  if (!signalsDisabled()) {
    victoryModified().fire(this, value);
  }
  setHasChanges();
}

void Messages::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_SET_OLD_VALUE(actionFailure);
  MODIFIABLE_SET_OLD_VALUE(actorDamage);
  MODIFIABLE_SET_OLD_VALUE(actorDrain);
  MODIFIABLE_SET_OLD_VALUE(actorGain);
  MODIFIABLE_SET_OLD_VALUE(actorLoss);
  MODIFIABLE_SET_OLD_VALUE(actorNoDamage);
  MODIFIABLE_SET_OLD_VALUE(actorNoHit);
  MODIFIABLE_SET_OLD_VALUE(actorRecovery);
  MODIFIABLE_SET_OLD_VALUE(alwaysDash);
  MODIFIABLE_SET_OLD_VALUE(bgmVolume);
  MODIFIABLE_SET_OLD_VALUE(bgsVolume);
  MODIFIABLE_SET_OLD_VALUE(buffAdd);
  MODIFIABLE_SET_OLD_VALUE(buffRemove);
  MODIFIABLE_SET_OLD_VALUE(commandRemember);
  MODIFIABLE_SET_OLD_VALUE(counterAttack);
  MODIFIABLE_SET_OLD_VALUE(criticalToActor);
  MODIFIABLE_SET_OLD_VALUE(criticalToEnemy);
  MODIFIABLE_SET_OLD_VALUE(debuffAdd);
  MODIFIABLE_SET_OLD_VALUE(defeat);
  MODIFIABLE_SET_OLD_VALUE(emerge);
  MODIFIABLE_SET_OLD_VALUE(enemyDamage);
  MODIFIABLE_SET_OLD_VALUE(enemyDrain);
  MODIFIABLE_SET_OLD_VALUE(enemyGain);
  MODIFIABLE_SET_OLD_VALUE(enemyLoss);
  MODIFIABLE_SET_OLD_VALUE(enemyNoDamage);
  MODIFIABLE_SET_OLD_VALUE(enemyNoHit);
  MODIFIABLE_SET_OLD_VALUE(enemyRecovery);
  MODIFIABLE_SET_OLD_VALUE(escapeFailure);
  MODIFIABLE_SET_OLD_VALUE(escapeStart);
  MODIFIABLE_SET_OLD_VALUE(evasion);
  MODIFIABLE_SET_OLD_VALUE(expNext);
  MODIFIABLE_SET_OLD_VALUE(expTotal);
  MODIFIABLE_SET_OLD_VALUE(file);
  MODIFIABLE_SET_OLD_VALUE(levelUp);
  MODIFIABLE_SET_OLD_VALUE(loadMessage);
  MODIFIABLE_SET_OLD_VALUE(magicEvasion);
  MODIFIABLE_SET_OLD_VALUE(magicReflection);
  MODIFIABLE_SET_OLD_VALUE(meVolume);
  MODIFIABLE_SET_OLD_VALUE(obtainExp);
  MODIFIABLE_SET_OLD_VALUE(obtainGold);
  MODIFIABLE_SET_OLD_VALUE(obtainItem);
  MODIFIABLE_SET_OLD_VALUE(obtainSkill);
  MODIFIABLE_SET_OLD_VALUE(partyName);
  MODIFIABLE_SET_OLD_VALUE(possession);
  MODIFIABLE_SET_OLD_VALUE(preemptive);
  MODIFIABLE_SET_OLD_VALUE(saveMessage);
  MODIFIABLE_SET_OLD_VALUE(seVolume);
  MODIFIABLE_SET_OLD_VALUE(substitute);
  MODIFIABLE_SET_OLD_VALUE(surprise);
  MODIFIABLE_SET_OLD_VALUE(useItem);
  MODIFIABLE_SET_OLD_VALUE(victory);
}
void Messages::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(actionFailure);
  MODIFIABLE_ACCEPT_VALUE(actorDamage);
  MODIFIABLE_ACCEPT_VALUE(actorDrain);
  MODIFIABLE_ACCEPT_VALUE(actorGain);
  MODIFIABLE_ACCEPT_VALUE(actorLoss);
  MODIFIABLE_ACCEPT_VALUE(actorNoDamage);
  MODIFIABLE_ACCEPT_VALUE(actorNoHit);
  MODIFIABLE_ACCEPT_VALUE(actorRecovery);
  MODIFIABLE_ACCEPT_VALUE(alwaysDash);
  MODIFIABLE_ACCEPT_VALUE(bgmVolume);
  MODIFIABLE_ACCEPT_VALUE(bgsVolume);
  MODIFIABLE_ACCEPT_VALUE(buffAdd);
  MODIFIABLE_ACCEPT_VALUE(buffRemove);
  MODIFIABLE_ACCEPT_VALUE(commandRemember);
  MODIFIABLE_ACCEPT_VALUE(counterAttack);
  MODIFIABLE_ACCEPT_VALUE(criticalToActor);
  MODIFIABLE_ACCEPT_VALUE(criticalToEnemy);
  MODIFIABLE_ACCEPT_VALUE(debuffAdd);
  MODIFIABLE_ACCEPT_VALUE(defeat);
  MODIFIABLE_ACCEPT_VALUE(emerge);
  MODIFIABLE_ACCEPT_VALUE(enemyDamage);
  MODIFIABLE_ACCEPT_VALUE(enemyDrain);
  MODIFIABLE_ACCEPT_VALUE(enemyGain);
  MODIFIABLE_ACCEPT_VALUE(enemyLoss);
  MODIFIABLE_ACCEPT_VALUE(enemyNoDamage);
  MODIFIABLE_ACCEPT_VALUE(enemyNoHit);
  MODIFIABLE_ACCEPT_VALUE(enemyRecovery);
  MODIFIABLE_ACCEPT_VALUE(escapeFailure);
  MODIFIABLE_ACCEPT_VALUE(escapeStart);
  MODIFIABLE_ACCEPT_VALUE(evasion);
  MODIFIABLE_ACCEPT_VALUE(expNext);
  MODIFIABLE_ACCEPT_VALUE(expTotal);
  MODIFIABLE_ACCEPT_VALUE(file);
  MODIFIABLE_ACCEPT_VALUE(levelUp);
  MODIFIABLE_ACCEPT_VALUE(loadMessage);
  MODIFIABLE_ACCEPT_VALUE(magicEvasion);
  MODIFIABLE_ACCEPT_VALUE(magicReflection);
  MODIFIABLE_ACCEPT_VALUE(meVolume);
  MODIFIABLE_ACCEPT_VALUE(obtainExp);
  MODIFIABLE_ACCEPT_VALUE(obtainGold);
  MODIFIABLE_ACCEPT_VALUE(obtainItem);
  MODIFIABLE_ACCEPT_VALUE(obtainSkill);
  MODIFIABLE_ACCEPT_VALUE(partyName);
  MODIFIABLE_ACCEPT_VALUE(possession);
  MODIFIABLE_ACCEPT_VALUE(preemptive);
  MODIFIABLE_ACCEPT_VALUE(saveMessage);
  MODIFIABLE_ACCEPT_VALUE(seVolume);
  MODIFIABLE_ACCEPT_VALUE(substitute);
  MODIFIABLE_ACCEPT_VALUE(surprise);
  MODIFIABLE_ACCEPT_VALUE(useItem);
  MODIFIABLE_ACCEPT_VALUE(victory);
}
nlohmann::ordered_json Messages::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(actionFailure),   MODIFIABLE_SERIALIZE_VALUE(actorDamage),     MODIFIABLE_SERIALIZE_VALUE(actorDrain),
      MODIFIABLE_SERIALIZE_VALUE(actorGain),       MODIFIABLE_SERIALIZE_VALUE(actorLoss),       MODIFIABLE_SERIALIZE_VALUE(actorNoDamage),
      MODIFIABLE_SERIALIZE_VALUE(actorNoHit),      MODIFIABLE_SERIALIZE_VALUE(actorRecovery),   MODIFIABLE_SERIALIZE_VALUE(alwaysDash),
      MODIFIABLE_SERIALIZE_VALUE(bgmVolume),       MODIFIABLE_SERIALIZE_VALUE(bgsVolume),       MODIFIABLE_SERIALIZE_VALUE(buffAdd),
      MODIFIABLE_SERIALIZE_VALUE(buffRemove),      MODIFIABLE_SERIALIZE_VALUE(commandRemember), MODIFIABLE_SERIALIZE_VALUE(counterAttack),
      MODIFIABLE_SERIALIZE_VALUE(criticalToActor), MODIFIABLE_SERIALIZE_VALUE(criticalToEnemy), MODIFIABLE_SERIALIZE_VALUE(debuffAdd),
      MODIFIABLE_SERIALIZE_VALUE(defeat),          MODIFIABLE_SERIALIZE_VALUE(emerge),          MODIFIABLE_SERIALIZE_VALUE(enemyDamage),
      MODIFIABLE_SERIALIZE_VALUE(enemyDrain),      MODIFIABLE_SERIALIZE_VALUE(enemyGain),       MODIFIABLE_SERIALIZE_VALUE(enemyLoss),
      MODIFIABLE_SERIALIZE_VALUE(enemyNoDamage),   MODIFIABLE_SERIALIZE_VALUE(enemyNoHit),      MODIFIABLE_SERIALIZE_VALUE(enemyRecovery),
      MODIFIABLE_SERIALIZE_VALUE(escapeFailure),   MODIFIABLE_SERIALIZE_VALUE(escapeStart),     MODIFIABLE_SERIALIZE_VALUE(evasion),
      MODIFIABLE_SERIALIZE_VALUE(expNext),         MODIFIABLE_SERIALIZE_VALUE(expTotal),        MODIFIABLE_SERIALIZE_VALUE(file),
      MODIFIABLE_SERIALIZE_VALUE(levelUp),         MODIFIABLE_SERIALIZE_VALUE(loadMessage),     MODIFIABLE_SERIALIZE_VALUE(magicEvasion),
      MODIFIABLE_SERIALIZE_VALUE(magicReflection), MODIFIABLE_SERIALIZE_VALUE(meVolume),        MODIFIABLE_SERIALIZE_VALUE(obtainExp),
      MODIFIABLE_SERIALIZE_VALUE(obtainGold),      MODIFIABLE_SERIALIZE_VALUE(obtainItem),      MODIFIABLE_SERIALIZE_VALUE(obtainSkill),
      MODIFIABLE_SERIALIZE_VALUE(partyName),       MODIFIABLE_SERIALIZE_VALUE(possession),      MODIFIABLE_SERIALIZE_VALUE(preemptive),
      MODIFIABLE_SERIALIZE_VALUE(saveMessage),     MODIFIABLE_SERIALIZE_VALUE(seVolume),        MODIFIABLE_SERIALIZE_VALUE(substitute),
      MODIFIABLE_SERIALIZE_VALUE(surprise),        MODIFIABLE_SERIALIZE_VALUE(useItem),         MODIFIABLE_SERIALIZE_VALUE(victory),
  };
}

rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actionFailureModified() {
  if (!m_actionFailureModified) {
    m_actionFailureModified.emplace();
  }
  return *m_actionFailureModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorDamageModified() {
  if (!m_actorDamageModified) {
    m_actorDamageModified.emplace();
  }
  return *m_actorDamageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorDrainModified() {
  if (!m_actorDrainModified) {
    m_actorDrainModified.emplace();
  }
  return *m_actorDrainModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorGainModified() {
  if (!m_actorGainModified) {
    m_actorGainModified.emplace();
  }
  return *m_actorGainModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorLossModified() {
  if (!m_actorLossModified) {
    m_actorLossModified.emplace();
  }
  return *m_actorLossModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorNoDamageModified() {
  if (!m_actorNoDamageModified) {
    m_actorNoDamageModified.emplace();
  }
  return *m_actorNoDamageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorNoHitModified() {
  if (!m_actorNoHitModified) {
    m_actorNoHitModified.emplace();
  }
  return *m_actorNoHitModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::actorRecoveryModified() {
  if (!m_actorRecoveryModified) {
    m_actorRecoveryModified.emplace();
  }
  return *m_actorRecoveryModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::alwaysDashModified() {
  if (!m_alwaysDashModified) {
    m_alwaysDashModified.emplace();
  }
  return *m_alwaysDashModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::bgmVolumeModified() {
  if (!m_bgmVolumeModified) {
    m_bgmVolumeModified.emplace();
  }
  return *m_bgmVolumeModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::bgsVolumeModified() {
  if (!m_bgsVolumeModified) {
    m_bgsVolumeModified.emplace();
  }
  return *m_bgsVolumeModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::buffAddModified() {
  if (!m_buffAddModified) {
    m_buffAddModified.emplace();
  }
  return *m_buffAddModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::buffRemoveModified() {
  if (!m_buffRemoveModified) {
    m_buffRemoveModified.emplace();
  }
  return *m_buffRemoveModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::commandRememberModified() {
  if (!m_commandRememberModified) {
    m_commandRememberModified.emplace();
  }
  return *m_commandRememberModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::counterAttackModified() {
  if (!m_counterAttackModified) {
    m_counterAttackModified.emplace();
  }
  return *m_counterAttackModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::criticalToActorModified() {
  if (!m_criticalToActorModified) {
    m_criticalToActorModified.emplace();
  }
  return *m_criticalToActorModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::criticalToEnemyModified() {
  if (!m_criticalToEnemyModified) {
    m_criticalToEnemyModified.emplace();
  }
  return *m_criticalToEnemyModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::debuffAddModified() {
  if (!m_debuffAddModified) {
    m_debuffAddModified.emplace();
  }
  return *m_debuffAddModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::defeatModified() {
  if (!m_defeatModified) {
    m_defeatModified.emplace();
  }
  return *m_defeatModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::emergeModified() {
  if (!m_emergeModified) {
    m_emergeModified.emplace();
  }
  return *m_emergeModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyDamageModified() {
  if (!m_enemyDamageModified) {
    m_enemyDamageModified.emplace();
  }
  return *m_enemyDamageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyDrainModified() {
  if (!m_enemyDrainModified) {
    m_enemyDrainModified.emplace();
  }
  return *m_enemyDrainModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyGainModified() {
  if (!m_enemyGainModified) {
    m_enemyGainModified.emplace();
  }
  return *m_enemyGainModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyLossModified() {
  if (!m_enemyLossModified) {
    m_enemyLossModified.emplace();
  }
  return *m_enemyLossModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyNoDamageModified() {
  if (!m_enemyNoDamageModified) {
    m_enemyNoDamageModified.emplace();
  }
  return *m_enemyNoDamageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyNoHitModified() {
  if (!m_enemyNoHitModified) {
    m_enemyNoHitModified.emplace();
  }
  return *m_enemyNoHitModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::enemyRecoveryModified() {
  if (!m_enemyRecoveryModified) {
    m_enemyRecoveryModified.emplace();
  }
  return *m_enemyRecoveryModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::escapeFailureModified() {
  if (!m_escapeFailureModified) {
    m_escapeFailureModified.emplace();
  }
  return *m_escapeFailureModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::escapeStartModified() {
  if (!m_escapeStartModified) {
    m_escapeStartModified.emplace();
  }
  return *m_escapeStartModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::evasionModified() {
  if (!m_evasionModified) {
    m_evasionModified.emplace();
  }
  return *m_evasionModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::expNextModified() {
  if (!m_expNextModified) {
    m_expNextModified.emplace();
  }
  return *m_expNextModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::expTotalModified() {
  if (!m_expTotalModified) {
    m_expTotalModified.emplace();
  }
  return *m_expTotalModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::fileModified() {
  if (!m_fileModified) {
    m_fileModified.emplace();
  }
  return *m_fileModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::levelUpModified() {
  if (!m_levelUpModified) {
    m_levelUpModified.emplace();
  }
  return *m_levelUpModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::loadMessageModified() {
  if (!m_loadMessageModified) {
    m_loadMessageModified.emplace();
  }
  return *m_loadMessageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::magicEvasionModified() {
  if (!m_magicEvasionModified) {
    m_magicEvasionModified.emplace();
  }
  return *m_magicEvasionModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::magicReflectionModified() {
  if (!m_magicReflectionModified) {
    m_magicReflectionModified.emplace();
  }
  return *m_magicReflectionModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::meVolumeModified() {
  if (!m_meVolumeModified) {
    m_meVolumeModified.emplace();
  }
  return *m_meVolumeModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::obtainExpModified() {
  if (!m_obtainExpModified) {
    m_obtainExpModified.emplace();
  }
  return *m_obtainExpModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::obtainGoldModified() {
  if (!m_obtainGoldModified) {
    m_obtainGoldModified.emplace();
  }
  return *m_obtainGoldModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::obtainItemModified() {
  if (!m_obtainItemModified) {
    m_obtainItemModified.emplace();
  }
  return *m_obtainItemModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::obtainSkillModified() {
  if (!m_obtainSkillModified) {
    m_obtainSkillModified.emplace();
  }
  return *m_obtainSkillModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::partyNameModified() {
  if (!m_partyNameModified) {
    m_partyNameModified.emplace();
  }
  return *m_partyNameModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::possessionModified() {
  if (!m_possessionModified) {
    m_possessionModified.emplace();
  }
  return *m_possessionModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::preemptiveModified() {
  if (!m_preemptiveModified) {
    m_preemptiveModified.emplace();
  }
  return *m_preemptiveModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::saveMessageModified() {
  if (!m_saveMessageModified) {
    m_saveMessageModified.emplace();
  }
  return *m_saveMessageModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::seVolumeModified() {
  if (!m_seVolumeModified) {
    m_seVolumeModified.emplace();
  }
  return *m_seVolumeModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::substituteModified() {
  if (!m_substituteModified) {
    m_substituteModified.emplace();
  }
  return *m_substituteModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::surpriseModified() {
  if (!m_surpriseModified) {
    m_surpriseModified.emplace();
  }
  return *m_surpriseModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::useItemModified() {
  if (!m_useItemModified) {
    m_useItemModified.emplace();
  }
  return *m_useItemModified;
}
rpgmutils::signal<void(Messages*, const std::string&)>& Messages::victoryModified() {
  if (!m_victoryModified) {
    m_victoryModified.emplace();
  }
  return *m_victoryModified;
}

void to_json(nlohmann::ordered_json& to, const Messages& messages) {
  to = {
      {"actionFailure", messages.m_actionFailure},
      {"actorDamage", messages.m_actorDamage},
      {"actorDrain", messages.m_actorDrain},
      {"actorGain", messages.m_actorGain},
      {"actorLoss", messages.m_actorLoss},
      {"actorNoDamage", messages.m_actorNoDamage},
      {"actorNoHit", messages.m_actorNoHit},
      {"actorRecovery", messages.m_actorRecovery},
      {"alwaysDash", messages.m_alwaysDash},
      {"bgmVolume", messages.m_bgmVolume},
      {"bgsVolume", messages.m_bgsVolume},
      {"buffAdd", messages.m_buffAdd},
      {"buffRemove", messages.m_buffRemove},
      {"commandRemember", messages.m_commandRemember},
      {"counterAttack", messages.m_counterAttack},
      {"criticalToActor", messages.m_criticalToActor},
      {"criticalToEnemy", messages.m_criticalToEnemy},
      {"debuffAdd", messages.m_debuffAdd},
      {"defeat", messages.m_defeat},
      {"emerge", messages.m_emerge},
      {"enemyDamage", messages.m_enemyDamage},
      {"enemyDrain", messages.m_enemyDrain},
      {"enemyGain", messages.m_enemyGain},
      {"enemyLoss", messages.m_enemyLoss},
      {"enemyNoDamage", messages.m_enemyNoDamage},
      {"enemyNoHit", messages.m_enemyNoHit},
      {"enemyRecovery", messages.m_enemyRecovery},
      {"escapeFailure", messages.m_escapeFailure},
      {"escapeStart", messages.m_escapeStart},
      {"evasion", messages.m_evasion},
      {"expNext", messages.m_expNext},
      {"expTotal", messages.m_expTotal},
      {"file", messages.m_file},
      {"levelUp", messages.m_levelUp},
      {"loadMessage", messages.m_loadMessage},
      {"magicEvasion", messages.m_magicEvasion},
      {"magicReflection", messages.m_magicReflection},
      {"meVolume", messages.m_meVolume},
      {"obtainExp", messages.m_obtainExp},
      {"obtainGold", messages.m_obtainGold},
      {"obtainItem", messages.m_obtainItem},
      {"obtainSkill", messages.m_obtainSkill},
      {"partyName", messages.m_partyName},
      {"possession", messages.m_possession},
      {"preemptive", messages.m_preemptive},
      {"saveMessage", messages.m_saveMessage},
      {"seVolume", messages.m_seVolume},
      {"substitute", messages.m_substitute},
      {"surprise", messages.m_surprise},
      {"useItem", messages.m_useItem},
      {"victory", messages.m_victory},
  };
}
void from_json(const nlohmann::ordered_json& from, Messages& messages) {
  messages.m_actionFailure = from.value("actionFailure", messages.m_actionFailure);
  messages.m_actorDamage = from.value("actorDamage", messages.m_actorDamage);
  messages.m_actorNoHit = from.value("actorNoHit", messages.m_actorNoHit);
  messages.m_actorDrain = from.value("actorDrain", messages.m_actorDrain);
  messages.m_actorGain = from.value("actorGain", messages.m_actorGain);
  messages.m_actorLoss = from.value("actorLoss", messages.m_actorLoss);
  messages.m_actorNoDamage = from.value("actorNoDamage", messages.m_actorNoDamage);
  messages.m_actorRecovery = from.value("actorRecovery", messages.m_actorRecovery);
  messages.m_alwaysDash = from.value("alwaysDash", messages.m_alwaysDash);
  messages.m_bgmVolume = from.value("bgmVolume", messages.m_bgmVolume);
  messages.m_bgsVolume = from.value("bgsVolume", messages.m_bgsVolume);
  messages.m_buffAdd = from.value("buffAdd", messages.m_buffAdd);
  messages.m_buffRemove = from.value("buffRemove", messages.m_buffRemove);
  messages.m_commandRemember = from.value("commandRemember", messages.m_commandRemember);
  messages.m_counterAttack = from.value("counterAttack", messages.m_counterAttack);
  messages.m_criticalToActor = from.value("criticalToActor", messages.m_criticalToActor);
  messages.m_criticalToEnemy = from.value("criticalToEnemy", messages.m_criticalToEnemy);
  messages.m_debuffAdd = from.value("debuffAdd", messages.m_debuffAdd);
  messages.m_defeat = from.value("defeat", messages.m_defeat);
  messages.m_emerge = from.value("emerge", messages.m_emerge);
  messages.m_enemyDamage = from.value("enemyDamage", messages.m_enemyDamage);
  messages.m_enemyDrain = from.value("enemyDrain", messages.m_enemyDrain);
  messages.m_enemyGain = from.value("enemyGain", messages.m_enemyGain);
  messages.m_enemyLoss = from.value("enemyLoss", messages.m_enemyLoss);
  messages.m_enemyNoDamage = from.value("enemyNoDamage", messages.m_enemyNoDamage);
  messages.m_enemyNoHit = from.value("enemyNoHit", messages.m_enemyNoHit);
  messages.m_enemyRecovery = from.value("enemyRecovery", messages.m_enemyRecovery);
  messages.m_escapeFailure = from.value("escapeFailure", messages.m_escapeFailure);
  messages.m_escapeStart = from.value("escapeStart", messages.m_escapeStart);
  messages.m_evasion = from.value("evasion", messages.m_evasion);
  messages.m_expNext = from.value("expNext", messages.m_expNext);
  messages.m_expTotal = from.value("expTotal", messages.m_expTotal);
  messages.m_file = from.value("file", messages.m_file);
  messages.m_levelUp = from.value("levelUp", messages.m_levelUp);
  messages.m_loadMessage = from.value("loadMessage", messages.m_loadMessage);
  messages.m_magicEvasion = from.value("magicEvasion", messages.m_magicEvasion);
  messages.m_magicReflection = from.value("magicReflection", messages.m_magicReflection);
  messages.m_meVolume = from.value("meVolume", messages.m_meVolume);
  messages.m_obtainExp = from.value("obtainExp", messages.m_obtainExp);
  messages.m_obtainGold = from.value("obtainGold", messages.m_obtainGold);
  messages.m_obtainItem = from.value("obtainItem", messages.m_obtainItem);
  messages.m_obtainSkill = from.value("obtainSkill", messages.m_obtainSkill);
  messages.m_partyName = from.value("partyName", messages.m_partyName);
  messages.m_possession = from.value("possession", messages.m_possession);
  messages.m_preemptive = from.value("preemptive", messages.m_preemptive);
  messages.m_saveMessage = from.value("saveMessage", messages.m_saveMessage);
  messages.m_seVolume = from.value("seVolume", messages.m_seVolume);
  messages.m_substitute = from.value("substitute", messages.m_substitute);
  messages.m_surprise = from.value("surprise", messages.m_surprise);
  messages.m_useItem = from.value("useItem", messages.m_useItem);
  messages.m_victory = from.value("victory", messages.m_victory);
}
