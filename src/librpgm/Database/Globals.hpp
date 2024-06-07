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
 * _pe_ -> .
 * _del_ -> useful placeholder for name collisions inserts a null and truncats the strings
 * _deg -> °
 *
 * After the special characters have been decoded and inserted any remaining
 * underscores are replaced with spaces
 */

enum class Direction : int {
  Retain = 0,
  Down = 2,
  Left = 4,
  Right = 6,
  Up = 8,
};

enum class Button : int {
  OK,
  Cancel,
  Shift,
  Down,
  Left,
  Right,
  Up,
  Pageup,
  Pagedown,
};

enum class HitType : int {
  Certain_Hit,
  Physical_Attack,
  Magical_Attack,
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

enum class TimerComparisonType : int { Less_than_or_Equal_to, Greater_than_or_Equal_to };

enum class ConditionType : int {
  Switch,
  Variable,
  Self_Switch,
  Timer,
  Actor,
  Enemy,
  Character,
  Gold,
  Item,
  Weapon,
  Armor,
  Button,
  Script,
  Vehicle,
};

enum class ActorConditionType : int {
  In_The_Party,
  Name,
  Class,
  Skill,
  Weapon,
  Armor,
  State,
};

enum class EnemyConditionType : int {
  Appeared,
  State,
};

enum class VariableComparisonSource : int {
  Constant,
  Variable,
};

enum class VariableComparisonType : int {
  Equal_To,
  Greater_than_or_Equal_to,
  Less_than_or_Equal_to,
  Greater_than,
  Less_than,
  Not_Equal_to
};

enum class GoldComaprisonType : int {
  Equal_To,
  Greater_than_or_Equal_to,
  Less_than_or_Equal_to,
  Less_than,
};

enum class SwitchControl : int {
  ON,
  OFF,
};

enum class VariableControlOperation {
  Set,
  Add,
  Sub,
  Mul,
  Div,
  Mod,
};

enum class VariableControlOperand {
  Constant,
  Variable,
  Random,
  Game_Data,
  Script,
};

enum class GameDataSource {
  Item,
  Weapon,
  Armor,
  Actor,
  Enemy,
  Character,
  Party,
  Other,
};

enum class ActorDataSource {
  Level,
  EXP,
  HP,
  MP,
  Max_HP,
  Max_MP,
  Attack,
  Defense,
  M__pe_Attack,
  M__pe_Defense,
  Agility,
  Luck
};

enum class EnemyDataSource {
  HP,
  MP,
  Max_HP,
  Max_MP,
  Attack,
  Defense,
  M__pe_Attack,
  M__pe_Defense,
  Agility,
  Luck,
};

enum class CharacterDataSource {
  Map_X,
  Map_Y,
  Direction,
  Screen_X,
  Screen_Y,
};

enum class OtherDataSource {
  Map_ID,
  Party_Members,
  Gold,
  Steps,
  Play_Time,
  Timer,
  Save_Count,
  Battle_Count,
  Win_Count,
  Escape_Count,
};

enum class PartyMemberOperation {
  Add,
  Remove,
};

enum class AccessMode {
  Disable,
  Enable,
};

enum class TransferMode {
  Direct,
  Variable_Designation,
  Exchange_With_Another_Event,
};

enum class EventCode : int {
  Event_Dummy = 0, // The ◆ character
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
  End = 412,
  Loop = 112,
  Repeat_Above = 413,
  Break_Loop = 113,

  Exit_Event_Processing = 115,

  Common_Event = 117,
  Label = 118,
  Jump_To_Label = 119,

  Control_Switches = 121,
  Control_Variables = 122,
  Control_Self_Switch = 123,
  Control_Timer = 124,

  Change_Gold = 125,
  Change_Items = 126,
  Change_Weapons = 127,
  Change_Armors = 128,
  Change_Party_Member = 129,
  Change_Battle_BGM = 132,
  Change_Victory_ME = 133,
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
  Movement_Route_Step = 505, // This is only used to display the event parameters in the event editor, this has no
                             // bearing on the actual value, however they do need to be kept in sync

  Get_On_Off_Vehicle = 206,
  Change_Transparency = 211,
  Show_Animation = 212,
  Show_Balloon_Icon = 213,
  Erase_Event = 214,
  // Left off here
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

  // MovementRoute commands
  Move_Down = 1,
  Move_Left = 2,
  Move_Right = 3,
  Move_Up = 4,
  Move_Lower_Left = 5,
  Move_Lower_Right = 6,
  Move_Upper_Left = 7,
  Move_Upper_Right = 8,
  Move_at_Random = 9,
  Move_toward_Player = 10,
  Move_away_from_Player = 11,
  _1_Step_Forward = 12,
  _1_Step_Backward = 13,
  Jump = 14,
  Wait_del_ = 15,
  Turn_Down = 16,
  Turn_Left = 17,
  Turn_Right = 18,
  Turn_Up = 19,
  Turn_90_deg_Right = 20,
  Turn_90_deg_Left = 21,
  Turn_180_deg = 22,
  Turn_90_deg_Left_or_Right = 23,
  Turn_at_Random = 24,
  Turn_toward_Player = 25,
  Turn_away_from_Player = 26,
  Switch_ON = 27,
  Switch_OFF = 28,
  Speed = 29,
  Frequency = 30,
  Walking_Animation_ON = 31,
  Walking_Animation_OFF = 32,
  Stepping_Animation_ON = 33,
  Stepping_Animation_OFF = 34,
  Direction_Fix_ON = 35,
  Direction_Fix_OFF = 36,
  Through_ON = 37,
  Through_OFF = 38,
  Transparent_ON = 39,
  Transparent_OFF = 40,
  Change_Image = 41,
  Change_Opacity = 42,
  Change_Blend_Mode = 43,
  Play_SE_de_Movement = 44,
  Script_de_Movement = 45,
};

enum class QuantityChangeOp {
  Increase,
  Decrease,
};

enum class QuantityChangeSource {
  Constant,
  Variable,
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

enum class ScrollType {
  No_Loop,
  Loop_Vertically,
  Loop_Horizontally,
  Loop_Both,
};

enum class TextBackground {
  Window,
  Dim,
  Transparent,
};

enum class TextWindowPosition {
  Top,
  Bottom,
  Middle,
};
enum class ChoiceWindowPosition {
  Left,
  Middle,
  Right,
};

enum class MovementSpeed {
  x8_Slower,
  x4_Slower,
  x2_Slower,
  Normal,
  x2_Faster,
  x4_Faster,
  x8_Faster,
};

enum class MoveType {
  Fixed,
  Random,
  Approach,
  Custom,
};

enum class MovementFrequency {
  Lowest,
  Lower,
  Normal,
  Higher,
  Highest,
};

enum class EventPriority {
  Below_character,
  Same_as_characters,
  Above_characters,
};

enum class EventTriggerType {
  Action_Button,
  Player_Touch,
  Event_Touch,
  Autorun,
  Parallel,
};

enum CommonEventTriggerType {
  None,
  Autorun,
  Parallel,
};

enum class TimerControl { Start, Stop };
std::string DecodeEnumName(std::string_view str);

template <typename E>
static inline std::string DecodeEnumName(E e) {
  return DecodeEnumName(magic_enum::enum_name(e));
}

#include "nlohmann/json.hpp"
// helper boiler plates for json parsing

namespace nlohmann {
template <typename T>
void to_json(json& j, const std::optional<T>& opt) {
  if (opt) {
    j = *opt;
  } else {
    j = nullptr;
  }
}

template <typename T>
void from_json(const json& j, std::optional<T>& opt) {
  if (j.is_null()) {
    opt = std::nullopt;
  } else {
    opt = j.get<T>();
  }
}
} // namespace nlohmann