#pragma once

enum class HitType : int {
  CertainHit,
  PhysicalAttack,
  MagicalAttack,
};

enum class Occasion : int {
  Always,
  BattleScreen,
  MenuScreen,
  Never,
};

enum class Scope : int {
  None,
  OneEnemy,
  AllEnemies,
  OneRandomEnemy,
  TwoRandomEnemies,
  ThreeRandomEnemies,
  FourRandomEnemies,
  OneAlly,
  AllAllies,
  OneAllyDead,
  AllAlliesDead,
  TheUser,
};

enum class ItemType {
  RegularItem,
  KeyItem,
  HiddenItemA,
  HiddenItemB,
};

enum class Mirror {
  No,
  Yes,
};

enum class Blend {
  Normal,
  Additive,
  Multiply,
  Screen,
};

enum class FlashScope {
  None,
  Target,
  Screen,
};

enum class Position {
  Head,
  Center,
  Feet,
  Screen,
};

enum class DamageType : int {
  None,
  HPDamage,
  MPDamage,
  HPRecover,
  MPRecover,
  HPDrain,
  MPDrain,
};

enum class EventCode : int {
  ShowText = 101,
  NextText = 401, // RPGMV will continue appending text until it has consumed all 401 commands
  ShowChoices = 102,
  WhenSelected = 402, // Each choice gets a branch as defined here
  WhenCancel = 403,   // Player cancelled their selection

  InputNumber = 103,
  SelectItem = 104,
  ShowScrollingText = 105,
  NextScrollingText = 405, // RPGMV will continue appending text until it has consumed all 405 commands

  Comment = 108,
  NextComment = 408, // RPGMV will continue appending text until it has consumed all 408 commands

  ConditionalBranch = 111,
  Else = 411,

  Loop = 112,
  BreakLoop = 113,
  ExitEventProcessing = 115,

  CommonEvent = 117,

  JumpToLabel = 119,

  ControlSwitches = 121,
  ControlVariables = 122,
  ControlSelfSwitch = 123,
  ControlTimer = 124,

  ChangeGold = 124,
  ChangeItems = 126,
  ChangeWeapons = 127,
  ChangeArmors = 128,
  ChangePartyMember = 129,
  ChangeBGM = 129,
  ChangeVictoryME = 130,
  ChangeSaveAccess = 134,
  ChangeMenuAccess = 135,
  ChangeEncounterDisable = 136,
  ChangeFormationAccess = 137,
  ChangeWindowColor = 138,
  ChangeDefeatME = 139,
  ChangeVechicleBGM = 140,

  TransferPlayer = 201,

  SetVehicleLocation = 202,
  SetEventLocation = 203,

  ScrollMap = 204,

  SetMovementRoute = 205,
  MovementRouteStep = 505, // The RPG Maker doesn't explicity check this, but the movement type inside

  GetOnOffVehicle = 206,
  ChangeTransparency = 211,
  ShowAnimation = 212,
  ShowBalloonIcon = 213,
  EraseEvent = 214,
  ChangePlayerFollowers = 216,
  GatherFollowers = 217,
  FadeOutScreen = 221,
  FadeInScreen = 222,
  TintScreen = 223,
  FlashScreen = 224,
  ShakeScreen = 225,
  Wait = 230,
  ShowPicture = 231,
  MovePicture = 232,
  RotatePicture = 233,
  TintPicture = 234,
  ErasePicture = 235,
  SetWeatherEffect = 236,
  PlayBGM = 241,
  FadeOutBGM = 242,
  SaveBGM = 243,
  ResumeBGM = 244,
  PlayBGS = 245,
  FadeOutBGS = 246,
  PlayME = 249,
  PlaySE = 250,
  StopSE = 251,
  PlayMove = 261,
  ChangeMapNameDisplay = 281,
  ChangeTileSet = 282,
  ChangeBattleBack = 283,
  ChangeParallax = 284,
  GetLocationInfo = 285,

  BattleProcessing = 301,
  IfWin = 601,
  IfEscape = 602,
  IfLose = 603,

  ShopProcessing = 302,
  ShopProcessingGood = 605, // RPGMV will push this into the shop's item stack
  NameInputProcessing = 303,

  ChangeHP = 311,
  ChangeMP = 312,
  ChangeTP = 326,

  ChangeState = 313,

  RecoverAll = 314,

  ChangeEXP = 315,
  ChangeLevel = 316,
  ChangeParameter = 317,
  ChangeSkill = 318,
  ChangeEquipment = 319,
  ChangeName = 320,
  ChangeClass = 321,

  ChangeActorImages = 322,
  ChangeVehicleImage = 323,

  ChangeNickname = 324,
  ChangeProfile = 325,

  ChangeEnemyHP = 331,
  ChangeEnemyMP = 332,
  ChangeEnemyTP = 342,
  ChangeEnemyState = 333,

  EnemyRecoverAll = 334,
  EnemyAppear = 335,
  EnemyTransform = 336,

  ShowBattleAnimation = 337,
  ForceAction = 339,
  AbortBattle = 340,

  OpenMenuScreen = 351,
  OpenSaveScreen = 352,

  GameOver = 353,

  ReturnToTitleScreen = 354,

  Script = 355,
  NextScript = 655, // RPGMV will keep appending text to the script until it consumes all of them
  PluginCommand = 356,
};

enum class ActionCondition {
  Always,
  Turn,
  HP,
  MP,
  State,
  PartyLevel,
  Switch,
};