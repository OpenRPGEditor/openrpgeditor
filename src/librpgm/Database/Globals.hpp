#pragma once

#include <string>
#include <optional>

#define MAGIC_ENUM_RANGE_MIN -4096
#define MAGIC_ENUM_RANGE_MAX 4096
#include <magic_enum.hpp>

/* DO NOT MODIFY ENUM NAMES
 * They may not seem like they make sense,
 * but they have a purpose.
 * magic_enum is used to get the value of the enum's name
 * We have some special encoding to add in special characters
 *
 * The currently available encoding is the follow:
 * po_ -> (
 * _pc -> )
 * bo_ -> [
 * _bc -> ]
 *
 * After the special characters have been decoded and inserted any remaining
 * underscores are replaced with spaces
 */

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
  _1_Enemy,
  All_Enemies,
  _1_Random_Enemy,
  _2_Random_Enemies,
  _3_Random_Enemies,
  _4_Random_Enemies,
  _1_Ally,
  All_Allies,
  _1_Ally_po_Dead_pc,
  All_Allies_po_Dead_pc,
  The_User,
};

enum class AutoRemovalTiming {
  None,
  Action_End,
  Turn_End,
};

enum class ItemType {
  Regular_Item,
  Key_Item,
  Hidden_Item_A,
  Hidden_Item_B,
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
  HP_Damage,
  MP_Damage,
  HP_Recover,
  MP_Recover,
  HP_Drain,
  MP_Drain,
};

enum class EventCode : int {
  Show_Text = 101,
  Next_Text = 401, // RPGMV will continue appending text until it has consumed all 401 commands

  Show_Choices = 102,
  When_Selected = 402, // Each choice gets a branch as defined here
  When_Cancel = 403,   // Player cancelled their selection

  Input_Number = 103,
  Select_Item = 104,
  Show_Scrolling_Text = 105,
  Next_Scrolling_Text = 405, // RPGMV will continue appending text until it has consumed all 405 commands

  Comment = 108,
  Next_Comment = 408, // RPGMV will continue appending text until it has consumed all 408 commands

  Conditional_Branch = 111,
  Else = 411,

  Loop = 112,
  Break_Loop = 113,

  Exit_Event_Processing = 115,

  Common_Event = 117,
  Jump_To_Label = 119,

  Control_Switches = 121,
  Control_Variables = 122,
  Control_Self_Switch = 123,
  Control_Timer = 124,

  Change_Items = 126,
  Change_Weapons = 127,
  Change_Armors = 128,
  Change_Party_Member = 129,
  Change_Victory_ME = 130,
  Change_Save_Access = 134,
  Change_Menu_Access = 135,
  Change_Encounter_Disable = 136,
  Change_Formation_Access = 137,
  Change_Window_Color = 138,
  Change_Defeat_ME = 139,
  Change_Vechicle_BGM = 140,

  Transfer_Player = 201,
  Set_Vehicle_Location = 202,
  Set_Event_Location = 203,
  Scroll_Map = 204,
  Set_Movement_Route = 205,
  Movement_Route_Step = 505, // The RPG Maker doesn't explicity check this, but the movement type inside

  Get_On_Off_Vehicle = 206,
  Change_Transparency = 211,
  Show_Animation = 212,
  Show_Balloon_Icon = 213,
  Erase_Event = 214,
  Change_Player_Followers = 216,
  Gather_Followers = 217,
  Fade_Out_Screen = 221,
  Fade_In_Screen = 222,
  Tint_Screen = 223,
  Flash_Screen = 224,
  Shake_Screen = 225,
  Wait = 230,
  Show_Picture = 231,
  Move_Picture = 232,
  Rotate_Picture = 233,
  Tint_Picture = 234,
  Erase_Picture = 235,
  Set_Weather_Effect = 236,
  Play_BGM = 241,
  Fade_Out_BGM = 242,
  Save_BGM = 243,
  Resume_BGM = 244,
  Play_BGS = 245,
  Fade_Out_BGS = 246,
  Play_ME = 249,
  Play_SE = 250,
  Stop_SE = 251,
  Play_Move = 261,
  Change_Map_Name_Display = 281,
  Change_Tile_Set = 282,
  Change_Battle_Back = 283,
  Change_Parallax = 284,
  Get_Location_Info = 285,
  Battle_Processing = 301,
  If_Win = 601,
  If_Escape = 602,
  If_Lose = 603,
  Shop_Processing = 302,
  Shop_Processing_Good = 605, // RPGMV will push this into the shop's item stack

  Name_Input_Processing = 303,
  Change_HP = 311,
  Change_MP = 312,
  Change_State = 313,
  Recover_All = 314,
  Change_EXP = 315,
  Change_Level = 316,
  Change_Parameter = 317,
  Change_Skill = 318,
  Change_Equipment = 319,
  Change_Name = 320,
  Change_Class = 321,
  Change_Actor_Images = 322,
  Change_Vehicle_Image = 323,
  Change_Nickname = 324,
  Change_Profile = 325,
  Change_TP = 326,
  Change_Enemy_HP = 331,
  Change_Enemy_MP = 332,
  Change_Enemy_State = 333,
  Enemy_Recover_All = 334,
  Enemy_Appear = 335,
  Enemy_Transform = 336,
  Show_Battle_Animation = 337,
  Force_Action = 339,
  Abort_Battle = 340,
  Change_Enemy_TP = 342,
  Open_Menu_Screen = 351,
  Open_Save_Screen = 352,
  Game_Over = 353,
  Return_To_Title_Screen = 354,
  Script = 355,
  Next_Script = 655, // RPGMV will keep appending text to the script until it consumes all of them
  Plugin_Command = 356,
};

enum class ActionCondition {
  Always,
  Turn,
  HP,
  MP,
  State,
  Party_Level,
  Switch,
};

enum class StateOverlay {
  None,
  Poison,
  Blind,
  Silence,
  Rage,
  Confusion,
  Fascination,
  Sleep,
  Paralyze,
  Curse,
  Fear,
};

enum class StateMotion {
  Normal,
  Abnormal,
  Sleep,
  Dead,
};

enum class StateRestriction {
  None,
  Attack_an_Enemy,
  Attack_anyone,
  Attack_an_ally,
  Cannot_move,
};

std::string DecodeEnumName(std::string_view str);

template <typename E>
static inline std::string DecodeEnumName(E e) {
  return DecodeEnumName(magic_enum::enum_name(e));
}

#include "nlohmann/json.hpp"
// helper boiler plates for json parsing
namespace nlohmann {
template <class T>
void to_json(json& j, const std::optional<T>& opt) {
  if (opt) {
    j = *opt;
  } else {
    j = nullptr;
  }
}

template <class T>
void from_json(const json& j, std::optional<T>& opt) {
  if (j.is_null()) {
    opt = std::nullopt;
  } else {
    opt = j.get<T>();
  }
}
} // namespace nlohmann