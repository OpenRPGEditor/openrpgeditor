#pragma once
#include "Database/Log.hpp"
#include <orei18n.hpp>

#include <optional>
#include <string>

#define MAGIC_ENUM_RANGE_MIN (-4096)
#define MAGIC_ENUM_RANGE_MAX (4096)
#include <magic_enum/magic_enum.hpp>

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
 * _da_ -> -=
 * _daa_ -> -
 * _pl_ -> +=
 * _plu_ -> +
 *
 * After the special characters have been decoded and inserted any remaining
 * underscores are replaced with spaces
 */

/* May be something else so this is prelim */
enum class ParameterSource {
  trENUM("Max HP", Max_HP),
  trENUM("Max MP", Max_MP),
  trENUM("Attack", Attack),
  trENUM("Defense", Defense),
  trENUM("M.Attack", trENUM("MAttack", M_pe_Attack)),
  trENUM("M.Defense", trENUM("MDefense", M_pe_Defense)),
  trENUM("Agility", Agility),
  trENUM("Luck", Luck),
};

enum class DebuffSource {
  trENUM("Max HP", Max_HP),
  trENUM("Max MP", Max_MP),
  trENUM("Attack", Attack),
  trENUM("Defense", Defense),
  trENUM("M.Attack", trENUM("M Attack", M_pe_Attack)),
  trENUM("M.Defense", trENUM("M Defense", M_pe_Defense)),
  trENUM("Agility", Agility),
  trENUM("Luck", Luck),
};

enum class EXParameterSource {
  trENUM("Hit Rate", Hit_Rate),
  trENUM("Evasion Rate", Evasion_Rate),
  trENUM("Critical Rate", Critical_Rate),
  trENUM("Critical Evasion", Critical_Evasion),
  trENUM("Magic Evasion", Magic_Evasion),
  trENUM("Magic Reflection", Magic_Reflection),
  trENUM("Counter Attack", Counter_Attack),
  trENUM("HP Regeneration", HP_Regeneration),
  trENUM("MP Regeneration", MP_Regeneration),
  trENUM("TP Regeneration", TP_Regeneration),
};

enum class SPParameterSource {
  trENUM("Target Rate", Target_Rate),
  trENUM("Guard Effect", Guard_Effect),
  trENUM("Recovery Effect", Recovery_Effect),
  trENUM("Pharmacology", Pharmacology),
  trENUM("MP Cost Rate", MP_Cost_Rate),
  trENUM("TP Charge Rate", TP_Charge_Rate),
  trENUM("Physical Damage", Physical_Damage),
  trENUM("Magical Damage", Magical_Damage),
  trENUM("Floor Damage", Floor_Damage),
  trENUM("Experience", Experience),
};

enum class SlotType {
  trENUM("Normal", Normal),
  trENUM("Dual Wield", Dual_Wield),
};

enum class SpecialFlag {
  trENUM("Auto Battle", Auto_Battle),
  trENUM("Guard", Guard),
  trENUM("Substitute", Substitute),
  trENUM("Preserve TP", Preserve_TP),
};

enum class CollapseEffect {
  trENUM("Normal", Normal),
  trENUM("Boss", Boss),
  trENUM("Instant", Instant),
  trENUM("No Disappear", No_Disappear),
};

enum class PartyAbility {
  trENUM("Encounter Half", Encounter_Half),
  trENUM("Encounter None", Encounter_None),
  trENUM("Cancel Surprise", Cancel_Surprise),
  trENUM("Raise Preemptive", Raise_Preemptive),
  trENUM("Gold Double", Gold_Double),
  trENUM("Drop Item Double", Drop_Item_Double),
};
enum class EffectCode {
  trENUM("Recover HP", Recover_HP) = 11,
  trENUM("Recover MP", Recover_MP) = 12,
  trENUM("Gain TP", Gain_TP) = 13,
  trENUM("Add State", Add_State) = 21,
  trENUM("Remove State", Remove_State) = 22,
  trENUM("Add Buff", Add_Buff) = 31,
  trENUM("Add Debuff", Add_Debuff) = 32,
  trENUM("Remove Buff", Remove_Buff) = 33,
  trENUM("Remove Debuff", Remove_Debuff) = 34,
  trENUM("Special Effect", Special_Effect) = 41,
  trENUM("Grow", Grow) = 42,
  trENUM("Learn Skill", Learn_Skill) = 43,
  trENUM("Common Event", Common_Event) = 44,
};

enum class TraitCode {
  trENUM("Element Rate", Element_Rate) = 11,
  trENUM("Debuff Rate", Debuff_Rate) = 12,
  trENUM("State Rate", State_Rate) = 13,
  trENUM("State Resist", State_Resist) = 14,
  trENUM("Parameter", Parameter) = 21,
  trENUM("Ex-Parameter", trENUM("ExParameter", Ex_daa_Parameter)) = 22,
  trENUM("Sp-Parameter", trENUM("SpParameter", Sp_daa_Parameter)) = 23,
  trENUM("Attack Element", Attack_Element) = 31,
  trENUM("Attack State", Attack_State) = 32,
  trENUM("Attack Speed", Attack_Speed) = 33,
  trENUM("Attack Times +", trENUM("Attack Times ", Attack_Times__plu_)) = 34,
  trENUM("Add Skill Type", Add_Skill_Type) = 41,
  trENUM("Seal Skill Type", Seal_Skill_Type) = 42,
  trENUM("Add Skill", Add_Skill) = 43,
  trENUM("Seal Skill", Seal_Skill) = 44,
  trENUM("Equip Weapon", Equip_Weapon) = 51,
  trENUM("Equip Armor", Equip_Armor) = 52,
  trENUM("Lock Equip", Lock_Equip) = 53,
  trENUM("Seal Equip", Seal_Equip) = 54,
  trENUM("Slot Type", Slot_Type) = 55,
  trENUM("Action Times +", trENUM("Action Times ", Action_Times__plu_)) = 61,
  trENUM("Special Flag", Special_Flag) = 62,
  trENUM("Collapse Effect", Collapse_Effect) = 63,
  trENUM("Party Ability", Party_Ability) = 64,
};

enum class Direction {
  trENUM("Retain", Retain) = 0,
  trENUM("Down", Down) = 2,
  trENUM("Left", Left) = 4,
  trENUM("Right", Right) = 6,
  trENUM("Up", Up) = 8,
};

enum class Button {
  trENUM("OK", OK),
  trENUM("Cancel", Cancel),
  trENUM("Shift", Shift),
  trENUM("Down", Down),
  trENUM("Left", Left),
  trENUM("Right", Right),
  trENUM("Up", Up),
  trENUM("Pageup", Pageup),
  trENUM("Pagedown", Pagedown),
};

enum class HitType {
  trENUM("Certain Hit", Certain_Hit),
  trENUM("Physical Attack", Physical_Attack),
  trENUM("Magical Attack", Magical_Attack),
};

enum class Occasion {
  trENUM("Always", Always),
  trENUM("BattleScreen", BattleScreen),
  trENUM("MenuScreen", MenuScreen),
  trENUM("Never", Never),
};

enum class Scope {
  trENUM("None", None),
  trENUM("1 Enemy", _1_Enemy),
  trENUM("All Enemies", All_Enemies),
  trENUM("1 Random Enemy", _1_Random_Enemy),
  trENUM("2 Random Enemies", _2_Random_Enemies),
  trENUM("3 Random Enemies", _3_Random_Enemies),
  trENUM("4 Random Enemies", _4_Random_Enemies),
  trENUM("1 Ally", _1_Ally),
  trENUM("All Allies", All_Allies),
  trENUM("1 Ally (Dead)", trENUM("1 Ally Dead", _1_Ally_po_Dead_pc)),
  trENUM("All Allies (Dead)", trENUM("All Allies Dead", All_Allies_po_Dead_pc)),
  trENUM("The User", The_User),
};

enum class AutoRemovalTiming {
  trENUM("None", None),
  trENUM("Action End", Action_End),
  trENUM("Turn End", Turn_End),
};

enum class ItemType {
  trENUM("Regular Item", Regular_Item),
  trENUM("Key Item", Key_Item),
  trENUM("Hidden Item A", Hidden_Item_A),
  trENUM("Hidden Item B", Hidden_Item_B),
};

enum class Mirror {
  trENUM("No", No),
  trENUM("Yes", Yes),
};

enum class Blend {
  trENUM("Normal", Normal),
  trENUM("Additive", Additive),
  trENUM("Multiply", Multiply),
  trENUM("Screen", Screen),
};

enum class FlashScope {
  trENUM("None", None),
  trENUM("Target", Target),
  trENUM("Screen", Screen),
};

enum class Position {
  trENUM("Head", Head),
  trENUM("Center", Center),
  trENUM("Feet", Feet),
  trENUM("Screen", Screen),
};

enum class DamageType {
  trENUM("None", None),
  trENUM("HP Damage", HP_Damage),
  trENUM("MP Damage", MP_Damage),
  trENUM("HP Recover", HP_Recover),
  trENUM("MP Recover", MP_Recover),
  trENUM("HP Drain", HP_Drain),
  trENUM("MP Drain", MP_Drain),
};

enum class TimerComparisonType {
  trENUM("≥", trENUM("Greater than or Equal to", _gteq__del_Greater_than_or_Equal_to)) = 0,
  trENUM("≤", trENUM("Less than or Equal to", _lteq__del_Less_than_or_Equal_to)) = 1,
};

enum class ConditionType {
  trENUM("Switch", Switch),
  trENUM("Variable", Variable),
  trENUM("Self Switch", Self_Switch),
  trENUM("Timer", Timer),
  trENUM("Actor", Actor),
  trENUM("Enemy", Enemy),
  trENUM("Character", Character),
  trENUM("Gold", Gold),
  trENUM("Item", Item),
  trENUM("Weapon", Weapon),
  trENUM("Armor", Armor),
  trENUM("Button", Button),
  trENUM("Script", Script),
  trENUM("Vehicle", Vehicle),
};

enum class ActorConditionType {
  trENUM("In The Party", In_The_Party),
  trENUM("Name", Name),
  trENUM("Class", Class),
  trENUM("Skill", Skill),
  trENUM("Weapon", Weapon),
  trENUM("Armor", Armor),
  trENUM("State", State),
};

enum class EnemyConditionType {
  trENUM("Appeared", Appeared),
  trENUM("State", State),
};

enum class VariableComparisonSource {
  trENUM("Constant", Constant),
  trENUM("Variable", Variable),
};

enum class ActorComparisonSource {
  trENUM("Fixed", Fixed),
  trENUM("Variable", Variable),
};

enum class SubjectComparisonSource {
  trENUM("Enemy", Enemy),
  trENUM("Actor", Actor),
};

enum class VariableComparisonType {
  trENUM("=", trENUM("Equal To", _set__del_Equal_To)) = 0,
  trENUM("≥", trENUM("Greater than or Equal to", _gteq__del_Greater_than_or_Equal_to)) = 1,
  trENUM("≤", trENUM("Less than or Equal to", _lteq__del_Less_than_or_Equal_to)) = 2,
  trENUM(">", trENUM("Greater than", _gt__del_Greater_than)) = 3,
  trENUM("<", trENUM("Less than", _lt__del_Less_than)) = 4,
  trENUM("!=", trENUM("Not Equal to", _not__set__del_Not_Equal_to)) = 5,
};

enum class GoldComparisonType {
  trENUM("≥", trENUM("Greater than or Equal to", _gteq__del_Greater_than_or_Equal_to)) = 0,
  trENUM("≤", trENUM("Less than or Equal to", _lteq__del_Less_than_or_Equal_to)) = 1,
  trENUM("<", trENUM("Less than", _lt__del_Less_than)) = 2,
};

enum class ValueControl {
  trENUM("ON", ON),
  trENUM("OFF", OFF),
};

enum class VariableControlOperation {
  trENUM("=", trENUM("Set", _set__del_Set)),
  trENUM("+=", trENUM("Add", _pl__del_Add)),
  trENUM("-=", trENUM("Sub", _da__del_Sub)),
  trENUM("*=", trENUM("Mul", _mul__del_Mul)),
  trENUM("/=", trENUM("Div", _div__del_Div)),
  trENUM("%=", trENUM("Mod", _mod__del_Mod)),
};

enum class VariableControlOperand {
  trENUM("Constant", Constant),
  trENUM("Variable", Variable),
  trENUM("Random", Random),
  trENUM("Game Data", Game_Data),
  trENUM("Script", Script),
};

enum class GameDataType {
  trENUM("Item", Item),
  trENUM("Weapon", Weapon),
  trENUM("Armor", Armor),
  trENUM("Actor", Actor),
  trENUM("Enemy", Enemy),
  trENUM("Character", Character),
  trENUM("Party", Party),
  trENUM("Other", Other),
};

enum class ActorDataSource {
  trENUM("Level", Level),
  trENUM("EXP", EXP),
  trENUM("HP", HP),
  trENUM("MP", MP),
  trENUM("Max HP", Max_HP),
  trENUM("Max MP", Max_MP),
  trENUM("Attack", Attack),
  trENUM("Defense", Defense),
  trENUM("M.Attack", trENUM("MAttack", M_pe_Attack)),
  trENUM("M.Defense", trENUM("MDefense", M_pe_Defense)),
  trENUM("Agility", Agility),
  trENUM("Luck", Luck),
};

enum class EnemyDataSource {
  trENUM("HP", HP),
  trENUM("MP", MP),
  trENUM("Max HP", Max_HP),
  trENUM("Max MP", Max_MP),
  trENUM("Attack", Attack),
  trENUM("Defense", Defense),
  trENUM("M.Attack", trENUM("MAttack", M_pe_Attack)),
  trENUM("M.Defense", trENUM("MDefense", M_pe_Defense)),
  trENUM("Agility", Agility),
  trENUM("Luck", Luck),
};

enum class CharacterDataSource {
  trENUM("Map X", Map_X),
  trENUM("Map Y", Map_Y),
  trENUM("Direction", Direction),
  trENUM("Screen X", Screen_X),
  trENUM("Screen Y", Screen_Y),
};

enum class OtherDataSource {
  trENUM("Map ID", Map_ID),
  trENUM("Party Members", Party_Members),
  trENUM("Gold", Gold),
  trENUM("Steps", Steps),
  trENUM("Play Time", Play_Time),
  trENUM("Timer", Timer),
  trENUM("Save Count", Save_Count),
  trENUM("Battle Count", Battle_Count),
  trENUM("Win Count", Win_Count),
  trENUM("Escape Count", Escape_Count),
};

enum class SpecialEffectSource {
  trENUM("Escape", Escape),
};

enum class PartyMemberOperation {
  trENUM("+", trENUM("Add", _plu__del_Add)),
  trENUM("-", trENUM("Remove", _daa__del_Remove)),
};

enum class ColorTone {
  trENUM("Red", Red),
  trENUM("Green", Green),
  trENUM("Blue", Blue),
  trENUM("Gray", Gray),
};

enum class SkillOperation {
  trENUM("+", trENUM("Learn", _plu__del_Learn)),
  trENUM("-", trENUM("Forget", _daa__del_Forget)),
};

enum class AccessMode {
  trENUM("Disable", Disable),
  trENUM("Enable", Enable),
};

enum class TransferMode {
  trENUM("Direct", Direct),
  trENUM("Variable Designation", Variable_Designation),
  trENUM("Exchange With Another Event", Exchange_With_Another_Event),
};

enum class Fade {
  trENUM("Black", Black),
  trENUM("White", White),
  trENUM("None", None),
};

enum class BalloonIcon {
  trENUM("Exclamation", Exclamation),
  trENUM("Question", Question),
  trENUM("Music Note", Music_Note),
  trENUM("Heart", Heart),
  trENUM("Anger", Anger),
  trENUM("Sweat", Sweat),
  trENUM("Cobweb", Cobweb),
  trENUM("Silence", Silence),
  trENUM("Light Bulb", Light_Bulb),
  trENUM("Zzz", Zzz),
  trENUM("User-defined 1", trENUM("Userdefined 1", User_daa_defined_1)),
  trENUM("User-defined 2", trENUM("Userdefined 2", User_daa_defined_2)),
  trENUM("User-defined 3", trENUM("Userdefined 3", User_daa_defined_3)),
  trENUM("User-defined 4", trENUM("Userdefined 4", User_daa_defined_4)),
  trENUM("User-defined 5", trENUM("Userdefined 5", User_daa_defined_5)),
};

enum class PictureOrigin {
  trENUM("Upper Left", Upper_Left),
  trENUM("Center", Center),
};

enum class WeatherEffect {
  trENUM("None", None),
  trENUM("Rain", Rain),
  trENUM("Storm", Storm),
  trENUM("Snow", Snow),
};

enum class PictureDesignationSource {
  trENUM("Direct designation", Direct_designation),
  trENUM("Designation with variables", Designation_with_variables),
};

enum class EventCode : int {
  Event_Dummy = 0,
  trENUM("Move Down", Move_Down) = 1,
  trENUM("Move Left", Move_Left) = 2,
  trENUM("Move Right", Move_Right) = 3,
  trENUM("Move Up", Move_Up) = 4,
  trENUM("Move Lower Left", Move_Lower_Left) = 5,
  trENUM("Move Lower Right", Move_Lower_Right) = 6,
  trENUM("Move Upper Left", Move_Upper_Left) = 7,
  trENUM("Move Upper Right", Move_Upper_Right) = 8,
  trENUM("Move at Random", Move_at_Random) = 9,
  trENUM("Move toward Player", Move_toward_Player) = 10,
  trENUM("Move away from Player", Move_away_from_Player) = 11,
  trENUM("1 Step Forward", _1_Step_Forward) = 12,
  trENUM("1 Step Backward", _1_Step_Backward) = 13,
  trENUM("Jump", Jump) = 14,
  trENUM("Wait", Wait_del_Movement) = 15,
  trENUM("Turn Down", Turn_Down) = 16,
  trENUM("Turn Left", Turn_Left) = 17,
  trENUM("Turn Right", Turn_Right) = 18,
  trENUM("Turn Up", Turn_Up) = 19,
  trENUM("Turn 90° Right", trENUM("Turn 90 Right", Turn_90_deg_Right)) = 20,
  trENUM("Turn 90° Left", trENUM("Turn 90 Left", Turn_90_deg_Left)) = 21,
  trENUM("Turn 180°", trENUM("Turn 180", Turn_180_deg)) = 22,
  trENUM("Turn 90° Left or Right", trENUM("Turn 90 Left or Right", Turn_90_deg_Left_or_Right)) = 23,
  trENUM("Turn at Random", Turn_at_Random) = 24,
  trENUM("Turn toward Player", Turn_toward_Player) = 25,
  trENUM("Turn away from Player", Turn_away_from_Player) = 26,
  trENUM("Switch ON", Switch_ON) = 27,
  trENUM("Switch OFF", Switch_OFF) = 28,
  trENUM("Speed", Speed) = 29,
  trENUM("Frequency", Frequency) = 30,
  trENUM("Walking Animation ON", Walking_Animation_ON) = 31,
  trENUM("Walking Animation OFF", Walking_Animation_OFF) = 32,
  trENUM("Stepping Animation ON", Stepping_Animation_ON) = 33,
  trENUM("Stepping Animation OFF", Stepping_Animation_OFF) = 34,
  trENUM("Direction Fix ON", Direction_Fix_ON) = 35,
  trENUM("Direction Fix OFF", Direction_Fix_OFF) = 36,
  trENUM("Through ON", Through_ON) = 37,
  trENUM("Through OFF", Through_OFF) = 38,
  trENUM("Transparent ON", Transparent_ON) = 39,
  trENUM("Transparent OFF", Transparent_OFF) = 40,
  trENUM("Change Image", Change_Image) = 41,
  trENUM("Change Opacity", Change_Opacity) = 42,
  trENUM("Change Blend Mode", Change_Blend_Mode) = 43,
  trENUM("Play SE", Play_SE_del_Movement) = 44,
  trENUM("Script", Script_del_Movement) = 45,
  trENUM("Show Text", Show_Text) = 101,
  trENUM("Show Choices", Show_Choices) = 102,
  trENUM("Input Number", Input_Number) = 103,
  trENUM("Select Item", Select_Item) = 104,
  trENUM("Show Scrolling Text", Show_Scrolling_Text) = 105,
  trENUM("Comment", Comment) = 108,
  trENUM("Conditional Branch", Conditional_Branch) = 111,
  trENUM("Loop", Loop) = 112,
  trENUM("Break Loop", Break_Loop) = 113,
  trENUM("Exit Event Processing", Exit_Event_Processing) = 115,
  trENUM("Common Event", Common_Event) = 117,
  trENUM("Label", Label) = 118,
  trENUM("Jump To Label", Jump_To_Label) = 119,
  trENUM("Control Switches", Control_Switches) = 121,
  trENUM("Control Variables", Control_Variables) = 122,
  trENUM("Control Self Switch", Control_Self_Switch) = 123,
  trENUM("Control Timer", Control_Timer) = 124,
  trENUM("Change Gold", Change_Gold) = 125,
  trENUM("Change Items", Change_Items) = 126,
  trENUM("Change Weapons", Change_Weapons) = 127,
  trENUM("Change Armors", Change_Armors) = 128,
  trENUM("Change Party Member", Change_Party_Member) = 129,
  trENUM("Change Battle BGM", Change_Battle_BGM) = 132,
  trENUM("Change Victory ME", Change_Victory_ME) = 133,
  trENUM("Change Save Access", Change_Save_Access) = 134,
  trENUM("Change Menu Access", Change_Menu_Access) = 135,
  trENUM("Change Encounter Disable", Change_Encounter_Disable) = 136,
  trENUM("Change Formation Access", Change_Formation_Access) = 137,
  trENUM("Change Window Color", Change_Window_Color) = 138,
  trENUM("Change Defeat ME", Change_Defeat_ME) = 139,
  trENUM("Change Vechicle BGM", Change_Vechicle_BGM) = 140,
  trENUM("Transfer Player", Transfer_Player) = 201,
  trENUM("Set Vehicle Location", Set_Vehicle_Location) = 202,
  trENUM("Set Event Location", Set_Event_Location) = 203,
  trENUM("Scroll Map", Scroll_Map) = 204,
  trENUM("Set Movement Route", Set_Movement_Route) = 205,
  trENUM("Get On Off Vehicle", Get_On_Off_Vehicle) = 206,
  trENUM("Change Transparency", Change_Transparency) = 211,
  trENUM("Show Animation", Show_Animation) = 212,
  trENUM("Show Balloon Icon", Show_Balloon_Icon) = 213,
  trENUM("Erase Event", Erase_Event) = 214,
  trENUM("Change Player Followers", Change_Player_Followers) = 216,
  trENUM("Gather Followers", Gather_Followers) = 217,
  trENUM("Fadeout Screen", Fadeout_Screen) = 221,
  trENUM("Fadein Screen", Fadein_Screen) = 222,
  trENUM("Tint Screen", Tint_Screen) = 223,
  trENUM("Flash Screen", Flash_Screen) = 224,
  trENUM("Shake Screen", Shake_Screen) = 225,
  trENUM("Wait", Wait) = 230,
  trENUM("Show Picture", Show_Picture) = 231,
  trENUM("Move Picture", Move_Picture) = 232,
  trENUM("Rotate Picture", Rotate_Picture) = 233,
  trENUM("Tint Picture", Tint_Picture) = 234,
  trENUM("Erase Picture", Erase_Picture) = 235,
  trENUM("Set Weather Effect", Set_Weather_Effect) = 236,
  trENUM("Play BGM", Play_BGM) = 241,
  trENUM("Fadeout BGM", Fadeout_BGM) = 242,
  trENUM("Save BGM", Save_BGM) = 243,
  trENUM("Resume BGM", Resume_BGM) = 244,
  trENUM("Play BGS", Play_BGS) = 245,
  trENUM("Fadeout BGS", Fadeout_BGS) = 246,
  trENUM("Play ME", Play_ME) = 249,
  trENUM("Play SE", Play_SE) = 250,
  trENUM("Stop SE", Stop_SE) = 251,
  trENUM("Play Movie", Play_Movie) = 261,
  trENUM("Change Map Name Display", Change_Map_Name_Display) = 281,
  trENUM("Change Tileset", Change_Tileset) = 282,
  trENUM("Change Battle Back", Change_Battle_Back) = 283,
  trENUM("Change Parallax", Change_Parallax) = 284,
  trENUM("Get Location Info", Get_Location_Info) = 285,
  trENUM("Battle Processing", Battle_Processing) = 301,
  trENUM("Shop Processing", Shop_Processing) = 302,
  trENUM("Name Input Processing", Name_Input_Processing) = 303,
  trENUM("Change HP", Change_HP) = 311,
  trENUM("Change MP", Change_MP) = 312,
  trENUM("Change State", Change_State) = 313,
  trENUM("Recover All", Recover_All) = 314,
  trENUM("Change EXP", Change_EXP) = 315,
  trENUM("Change Level", Change_Level) = 316,
  trENUM("Change Parameter", Change_Parameter) = 317,
  trENUM("Change Skill", Change_Skill) = 318,
  trENUM("Change Equipment", Change_Equipment) = 319,
  trENUM("Change Name", Change_Name) = 320,
  trENUM("Change Class", Change_Class) = 321,
  trENUM("Change Actor Images", Change_Actor_Images) = 322,
  trENUM("Change Vehicle Image", Change_Vehicle_Image) = 323,
  trENUM("Change Nickname", Change_Nickname) = 324,
  trENUM("Change Profile", Change_Profile) = 325,
  trENUM("Change TP", Change_TP) = 326,
  trENUM("Change Enemy HP", Change_Enemy_HP) = 331,
  trENUM("Change Enemy MP", Change_Enemy_MP) = 332,
  trENUM("Change Enemy State", Change_Enemy_State) = 333,
  trENUM("Enemy Recover All", Enemy_Recover_All) = 334,
  trENUM("Enemy Appear", Enemy_Appear) = 335,
  trENUM("Enemy Transform", Enemy_Transform) = 336,
  trENUM("Show Battle Animation", Show_Battle_Animation) = 337,
  trENUM("Force Action", Force_Action) = 339,
  trENUM("Abort Battle", Abort_Battle) = 340,
  trENUM("Change Enemy TP", Change_Enemy_TP) = 342,
  trENUM("Open Menu Screen", Open_Menu_Screen) = 351,
  trENUM("Open Save Screen", Open_Save_Screen) = 352,
  trENUM("Game Over", Game_Over) = 353,
  trENUM("Return To Title Screen", Return_To_Title_Screen) = 354,
  trENUM("Script", Script) = 355,
  trENUM("PluginMV Command", PluginMV_Command) = 356,
  trENUM("PluginMZ Command", PluginMZ_Command) = 357,
  trENUM("Next Text", Next_Text) = 401,
  trENUM("When Selected", When_Selected) = 402,
  trENUM("When Cancel", When_Cancel) = 403,
  trENUM("End", End_del_ShowChoices) = 404,
  trENUM("Next Scrolling Text", Next_Scrolling_Text) = 405,
  trENUM("Next Comment", Next_Comment) = 408,
  trENUM("Else", Else) = 411,
  trENUM("End", End) = 412,
  trENUM("Repeat Above", Repeat_Above) = 413,
  trENUM("Movement Route Step", Movement_Route_Step) = 505,
  trENUM("If Win", If_Win) = 601,
  trENUM("If Escape", If_Escape) = 602,
  trENUM("If Lose", If_Lose) = 603,
  trENUM("End", End_del_Battle_Processing) = 604,
  trENUM("Shop Processing Good", Shop_Processing_Good) = 605,
  trENUM("Next Script", Next_Script) = 655,
  trENUM("Next PluginMZ Command", Next_PluginMZ_Command) = 657,
};

enum class QuantityChangeOp {
  trENUM("+", trENUM("Increase", _plu__del_Increase)),
  trENUM("-", trENUM("Decrease", _daa__del_Decrease)),
};

enum class QuantityChangeSource {
  trENUM("Constant", Constant),
  trENUM("Variable", Variable),
};

enum class ShopType {
  trENUM("Item", Item),
  trENUM("Weapon", Weapon),
  trENUM("Armor", Armor),
};

enum class PriceType {
  trENUM("Standard", Standard),
  trENUM("Specific", Specific),
};

enum class ActionCondition {
  trENUM("Always", Always),
  trENUM("Turn", Turn),
  trENUM("HP", HP),
  trENUM("MP", MP),
  trENUM("State", State),
  trENUM("Party Level", Party_Level),
  trENUM("Switch", Switch),
};

enum class StateOverlay {
  trENUM("None", None),
  trENUM("Poison", Poison),
  trENUM("Blind", Blind),
  trENUM("Silence", Silence),
  trENUM("Rage", Rage),
  trENUM("Confusion", Confusion),
  trENUM("Fascination", Fascination),
  trENUM("Sleep", Sleep),
  trENUM("Paralyze", Paralyze),
  trENUM("Curse", Curse),
  trENUM("Fear", Fear),
};

enum class StateMotion {
  trENUM("Normal", Normal),
  trENUM("Abnormal", Abnormal),
  trENUM("Sleep", Sleep),
  trENUM("Dead", Dead),
};

enum class StateRestriction {
  trENUM("None", None),
  trENUM("Attack an Enemy", Attack_an_Enemy),
  trENUM("Attack anyone", Attack_anyone),
  trENUM("Attack an ally", Attack_an_ally),
  trENUM("Cannot move", Cannot_move),
};

enum class ScrollType {
  trENUM("No Loop", No_Loop),
  trENUM("Loop Vertically", Loop_Vertically),
  trENUM("Loop Horizontally", Loop_Horizontally),
  trENUM("Loop Both", Loop_Both),
};

enum class TextBackground {
  trENUM("Window", Window),
  trENUM("Dim", Dim),
  trENUM("Transparent", Transparent),
};

enum class TextWindowPosition {
  trENUM("Top", Top),
  trENUM("Bottom", Bottom),
  trENUM("Middle", Middle),
};

enum class ChoiceWindowPosition {
  trENUM("Left", Left),
  trENUM("Middle", Middle),
  trENUM("Right", Right),
};

enum class MovementSpeed {
  trENUM("x8 Slower", x8_Slower),
  trENUM("x4 Slower", x4_Slower),
  trENUM("x2 Slower", x2_Slower),
  trENUM("Normal", Normal),
  trENUM("x2 Faster", x2_Faster),
  trENUM("x4 Faster", x4_Faster),
};

enum class VehicleType {
  trENUM("Boat", Boat),
  trENUM("Ship", Ship),
  trENUM("Airship", Airship),
};

enum class MoveType {
  trENUM("Fixed", Fixed),
  trENUM("Random", Random),
  trENUM("Approach", Approach),
  trENUM("Custom", Custom),
};

enum class MovementFrequency {
  trENUM("Lowest", Lowest),
  trENUM("Lower", Lower),
  trENUM("Normal", Normal),
  trENUM("Higher", Higher),
  trENUM("Highest", Highest),
};

enum class TileType {
  trENUM("Terrain Tag", Terrain_Tag),
  trENUM("Event ID", Event_ID),
  trENUM("Tile ID Layer 1", Tile_ID_Layer_1),
  trENUM("Tile ID Layer 2", Tile_ID_Layer_2),
  trENUM("Tile ID Layer 3", Tile_ID_Layer_3),
  trENUM("Tile ID Layer 4", Tile_ID_Layer_4),
  trENUM("Region ID", Region_ID),
};

enum class LocationSource {
  trENUM("Direct designation", Direct_designation),
  trENUM("Designation with variables", Designation_with_variables),
};

enum class BattleProcessType {
  trENUM("Direct designation", Direct_designation),
  trENUM("Designation with variables", Designation_with_variables),
  trENUM("Same as random encounter", Same_as_random_encounter),
};

enum class EventPriority {
  trENUM("Below character", Below_character),
  trENUM("Same as characters", Same_as_characters),
  trENUM("Above characters", Above_characters),
};

enum class EventTriggerType {
  trENUM("Action Button", Action_Button),
  trENUM("Player Touch", Player_Touch),
  trENUM("Event Touch", Event_Touch),
  trENUM("Autorun", Autorun),
  trENUM("Parallel", Parallel),
};

enum class CommonEventTriggerType {
  trENUM("None", None),
  trENUM("Autorun", Autorun),
  trENUM("Parallel", Parallel),
};

enum class TimerControl {
  trENUM("Start", Start),
  trENUM("Stop", Stop),
};

enum class FormatColor {
  Gray,
  Red,
  Default,
};

enum class MotionType {
  trENUM("Thrust", Thrust),
  trENUM("Swing", Swing),
  trENUM("Missile", Missile),
};

enum class MotionImage {
  trENUM("None", None),
  trENUM("Dagger", Dagger),
  trENUM("Sword", Sword),
  trENUM("Flail", Flail),
  trENUM("Axe", Axe),
  trENUM("Whip", Whip),
  trENUM("Cane", Cane),
  trENUM("Bow", Bow),
  trENUM("Crossbow", Crossbow),
  trENUM("Gun", Gun),
  trENUM("Claw", Claw),
  trENUM("Glove", Glove),
  trENUM("Spear", Spear),
  trENUM("Mace", Mace),
  trENUM("Rod", Rod),
  trENUM("Club", Club),
  trENUM("Combat Chain", Combat_Chain),
  trENUM("Futuristic Sword", Futuristic_Sword),
  trENUM("Iron Pipe", Iron_Pipe),
  trENUM("Slingshot", Slingshot),
  trENUM("Shotgun", Shotgun),
  trENUM("Rifle", Rifle),
  trENUM("Chainsaw", Chainsaw),
  trENUM("Railgun", Railgun),
  trENUM("Stun Rod", Stun_Rod),
  trENUM("User-defined 1", trENUM("Userdefined 1", User_daa_defined_1)),
  trENUM("User-defined 2", trENUM("Userdefined 2", User_daa_defined_2)),
  trENUM("User-defined 3", trENUM("Userdefined 3", User_daa_defined_3)),
  trENUM("User-defined 4", trENUM("Userdefined 4", User_daa_defined_4)),
  trENUM("User-defined 5", trENUM("Userdefined 5", User_daa_defined_5)),
  trENUM("User-defined 6", trENUM("Userdefined 6", User_daa_defined_6)),
};

enum class SoundType {
  trENUM("Cursor", Cursor),
  trENUM("OK", OK),
  trENUM("Cancel", Cancel),
  trENUM("Buzzer", Buzzer),
  trENUM("Equip", Equip),
  trENUM("Save", Save),
  trENUM("Load", Load),
  trENUM("Battle Start", Battle_Start),
  trENUM("Escape", Escape),
  trENUM("Enemy Attack", Enemy_Attack),
  trENUM("Enemy Damage", Enemy_Damage),
  trENUM("Enemy Collapse", Enemy_Collapse),
  trENUM("Boss Collapse 1", Boss_Collapse_1),
  trENUM("Boss Collapse 2", Boss_Collapse_2),
  trENUM("Actor Damage", Actor_Damage),
  trENUM("Actor Collapse", Actor_Collapse),
  trENUM("Recovery", Recovery),
  trENUM("Miss", Miss),
  trENUM("Evasion", Evasion),
  trENUM("Magic Evasion", Magic_Evasion),
  trENUM("Magic Reflection", Magic_Reflection),
  trENUM("Shop", Shop),
  trENUM("Use Item", Use_Item),
  trENUM("Use Skill", Use_Skill),
};
enum class Span {
  trENUM("Battle", Battle),
  trENUM("Turn", Turn),
  trENUM("Moment", Moment),
};

/**
 * @name Flags
 * @brief Allows the developer to set specific tile behaviors
 */
enum class TileFlags {
  None = 0,
  PassageSouth = 1 << 0,                                                //!< If set characters cannot pass through from the south, if unset they can
  PassageWest = 1 << 1,                                                 //!< If set characters cannot pass through from the west, if unset they can
  PassageEast = 1 << 2,                                                 //!< If set characters cannot pass through from the east, if unset they can
  PassageNorth = 1 << 3,                                                //!< If set characters cannot pass through from the north, if unset they can
  PassageHigherTile = 1 << 4,                                           //!< If set characters cannot pass through from below, if unset they can
  Impassable = PassageNorth | PassageSouth | PassageEast | PassageWest, //!< If all cardinal direction flags are set characters cannot pass from any direction, if unset they can
  Ladder = 1 << 5,                                                      //!< If unset Tile behaves like a ladder, if unset it doesn't
  Bush = 1 << 6,                                                        //!< Tile behaves like a bush, if unset it doesn't
  Counter = 1 << 7,                                                     //!< If unset this flag forces the tile to be moved 12 pixels down, if unset it doesn't
  Damage = 1 << 8,                                                      //!< If unset this flag causes damage to the player character, if unset they can
  BoatPassage = 1 << 9,                                                 //!< If unset boats cannot pass through, if unset they can
  ShipPassage = 1 << 10,                                                //!< If unset ships cannot pass through, if unset they can
  AirshipPassage = 1 << 11,                                             //!< If unset Airships cannot pass through, if unset they can
  TerrainTag = 1 << 12,
};

void ReplaceStr(std::string& str, const std::string& oldStr, const std::string& newStr);
std::string DecodeEnumName(std::string_view str);
template <typename E>
static inline std::string DecodeEnumName(E e) {
  return DecodeEnumName(magic_enum::enum_name(e));
}

std::string UndectorateEnumName(std::string_view str);
template <typename E>
static inline std::string UndectorateEnumName(E e) {
  return UndectorateEnumName(magic_enum::enum_name(e));
}
#include "nlohmann/json.hpp"
// helper boiler plates for json parsing

// partial specialization (full specialization works too)
namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
  static void to_json(ordered_json& j, const std::optional<T>& opt) {
    if (!opt.has_value()) {
      j = nullptr;
    } else {
      j = *opt; // this will call adl_serializer<T>::to_json which will
      // find the free function to_json in T's namespace!
    }
  }

  static void from_json(const ordered_json& j, std::optional<T>& opt) {
    if (!j.is_null()) {
      opt = j.template get<T>(); // same as above, but with
      // adl_serializer<T>::from_json
    }
  }
};
} // namespace nlohmann

// Custom serializer for floating-point numbers
static inline void to_json(nlohmann::ordered_json& j, const double& value) {
  std::ostringstream oss;
  oss << std::setprecision(std::numeric_limits<double>::max_digits10) << value;
  std::string str = oss.str();

  // Remove trailing zeros after decimal point
  str.erase(str.find_last_not_of('0') + 1, std::string::npos);
  if (str.back() == '.') {
    str.pop_back();
  }

  j = str;
}

static inline void to_json(nlohmann::ordered_json& j, const float& value) {
  std::ostringstream oss;
  oss << std::setprecision(std::numeric_limits<float>::max_digits10) << value;
  std::string str = oss.str();

  // Remove trailing zeros after decimal point
  str.erase(str.find_last_not_of('0') + 1, std::string::npos);
  if (str.back() == '.') {
    str.pop_back();
  }

  j = str;
}

/* Helper function to set dirty state */
template <typename T>
void setDirty(const T& a, const T& b, bool& dirty) {
  dirty |= a != b;
}

std::vector<std::string> splitString(const std::string& str, char delimiter);
std::string& trim(std::string& s);