#include "Database/ColorFormatter.hpp"

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"

#include <string_view>

using namespace std::string_view_literals;

// clang-format off
constexpr std::string_view indigo      = "&push-color=0x4b,0x00,0x82;"sv;
constexpr std::string_view blue        = "&push-color=0x00,0x00,0xff;"sv;
constexpr std::string_view red         = "&push-color=0xff,0x00,0x00;"sv;
constexpr std::string_view darkorange  = "&push-color=0xff,0x8c,0x00;"sv;
constexpr std::string_view magenta     = "&push-color=0xff,0x00,0xff;"sv;
constexpr std::string_view maroon      = "&push-color=0x80,0x00,0x00;"sv;
constexpr std::string_view olive       = "&push-color=0x80,0x80,0x00;"sv;
constexpr std::string_view crimson     = "&push-color=0xdc,0x14,0x3c;"sv;
constexpr std::string_view purple      = "&push-color=0x80,0x00,0x80;"sv;
constexpr std::string_view teal        = "&push-color=0x00,0x80,0x80;"sv;
constexpr std::string_view deeppink    = "&push-color=0xff,0x14,0x93;"sv;
constexpr std::string_view dodgerblue  = "&push-color=0x1e,0x90,0xff;"sv;
constexpr std::string_view darkviolet  = "&push-color=0x94,0x00,0xd3;"sv;
constexpr std::string_view gray        = "&push-color=0x80,0x80,0x80;"sv;
constexpr std::string_view green       = "&push-color=0x00,0x80,0x00;"sv;
constexpr std::string_view white       = "&push-color=0xFF,0xFF,0xFF;"sv;
// clang-format on

static std::map<EventCode, std::string_view> CommandColors{
    // clang-format off
  {EventCode::Event_Dummy,              white},
  {EventCode::Show_Text,                indigo},
  {EventCode::Next_Text,                indigo},
  {EventCode::Show_Choices,             indigo},
  {EventCode::Input_Number,             indigo},
  {EventCode::When_Selected,            indigo},
  {EventCode::End_del_ShowChoices,      indigo},
  {EventCode::Select_Item,              indigo},
  {EventCode::Show_Scrolling_Text,      indigo},
  {EventCode::Next_Scrolling_Text,      indigo},
  {EventCode::Comment,                  green},
  {EventCode::Next_Comment,             green},
  {EventCode::Conditional_Branch,       blue},
  {EventCode::Else,                     blue},
  {EventCode::End,                      blue},
  {EventCode::Loop,                     blue},
  {EventCode::Repeat_Above,             blue},
  {EventCode::Break_Loop,               blue},
  /*{EventCode::Continue_Loop,            blue}, Unused */
  {EventCode::Exit_Event_Processing,    blue},
  {EventCode::Common_Event,             blue},
  {EventCode::Label,                    blue},
  {EventCode::Jump_To_Label,            blue},
  {EventCode::Control_Switches,         red},
  {EventCode::Control_Variables,        red},
  {EventCode::Control_Self_Switch,      red},
  {EventCode::Control_Timer,            red},
  {EventCode::Change_Gold,              darkorange},
  {EventCode::Change_Items,             darkorange},
  {EventCode::Change_Weapons,           darkorange},
  {EventCode::Change_Armors,            darkorange},
  {EventCode::Change_Party_Member,      darkorange},
  {EventCode::Change_Battle_BGM,        magenta},
  {EventCode::Change_Victory_ME,        magenta},
  {EventCode::Change_Save_Access,       magenta},
  {EventCode::Change_Menu_Access,       magenta},
  {EventCode::Change_Encounter_Disable, magenta},
  {EventCode::Change_Formation_Access,  magenta},
  {EventCode::Change_Window_Color,      magenta},
  {EventCode::Change_Defeat_ME,         magenta},
  {EventCode::Change_Vechicle_BGM,      magenta},
  {EventCode::Transfer_Player,          maroon},
  {EventCode::Set_Vehicle_Location,     maroon},
  {EventCode::Set_Event_Location,       maroon},
  {EventCode::Scroll_Map,               maroon},
  {EventCode::Set_Movement_Route,       maroon},
  {EventCode::Movement_Route_Step,      maroon},
  {EventCode::Get_On_Off_Vehicle,       maroon},
  {EventCode::Change_Transparency,      maroon},
  {EventCode::Change_Player_Followers,  maroon},
  {EventCode::Gather_Followers,         maroon},
  {EventCode::Show_Animation,           maroon},
  {EventCode::Show_Balloon_Icon,        maroon},
  {EventCode::Erase_Event,              maroon},
  {EventCode::Fadeout_Screen,          olive},
  {EventCode::Fadein_Screen,           olive},
  {EventCode::Tint_Screen,              olive},
  {EventCode::Flash_Screen,             olive},
  {EventCode::Shake_Screen,             olive},
  {EventCode::Wait,                     crimson},
  {EventCode::Show_Picture,             purple},
  {EventCode::Move_Picture,             purple},
  {EventCode::Rotate_Picture,           purple},
  {EventCode::Tint_Picture,             purple},
  {EventCode::Erase_Picture,            purple},
  {EventCode::Set_Weather_Effect,       olive},
  {EventCode::Play_BGM,                 teal},
  {EventCode::Fadeout_BGM,             teal},
  {EventCode::Save_BGM,                 teal},
  {EventCode::Resume_BGM,               teal},
  {EventCode::Play_BGS,                 teal},
  {EventCode::Play_SE,                  teal},
  {EventCode::Play_Movie,               teal},
  {EventCode::Change_Map_Name_Display,  deeppink},
  {EventCode::Change_Tileset,           deeppink},
  {EventCode::Change_Battle_Back,       deeppink},
  {EventCode::Change_Parallax,          deeppink},
  {EventCode::Get_Location_Info,        deeppink},
  {EventCode::Battle_Processing,        dodgerblue},
  {EventCode::If_Win,                   dodgerblue},
  {EventCode::If_Escape,                dodgerblue},
  {EventCode::If_Lose,                  dodgerblue},
  {EventCode::End_del_Battle_Processing,dodgerblue},
  {EventCode::Shop_Processing,          dodgerblue},
  {EventCode::Shop_Processing_Good,     dodgerblue},
  {EventCode::Name_Input_Processing,    dodgerblue},
  {EventCode::Change_HP,                darkorange},
  {EventCode::Change_MP,                darkorange},
  {EventCode::Change_TP,                darkorange},
  {EventCode::Change_State,             darkorange},
  {EventCode::Recover_All,              darkorange},
  {EventCode::Change_EXP,               darkorange},
  {EventCode::Change_Level,             darkorange},
  {EventCode::Change_Parameter,         darkorange},
  {EventCode::Change_Skill,             darkorange},
  {EventCode::Change_Equipment,         darkorange},
  {EventCode::Change_Name,              darkorange},
  {EventCode::Change_Class,             darkorange},
  {EventCode::Change_Actor_Images,      magenta},
  {EventCode::Change_Vehicle_Image,     magenta},
  {EventCode::Change_Nickname,          darkorange},
  {EventCode::Change_Profile,           darkorange},
  {EventCode::Change_Enemy_HP,          darkviolet},
  {EventCode::Change_Enemy_MP,          darkviolet},
  {EventCode::Change_Enemy_TP,          darkviolet},
  {EventCode::Change_Enemy_State,       darkviolet},
  {EventCode::Enemy_Recover_All,        darkviolet},
  {EventCode::Enemy_Appear,             darkviolet},
  {EventCode::Enemy_Transform,          darkviolet},
  {EventCode::Show_Battle_Animation,    darkviolet},
  {EventCode::Force_Action,             darkviolet},
  {EventCode::Abort_Battle,             darkviolet},
  {EventCode::Open_Menu_Screen,         dodgerblue},
  {EventCode::Open_Save_Screen,         dodgerblue},
  {EventCode::Return_To_Title_Screen,   dodgerblue},
  {EventCode::Script,                   indigo},
  {EventCode::Next_Script,              indigo},
  {EventCode::PluginMV_Command,         indigo},
  {EventCode::PluginMZ_Command,         indigo},
  {EventCode::Next_PluginMZ_Command,    indigo},
    // clang-format on
};
/**
 *
 * @param command
 * @return Returns an ImGui formatted string for the specific command name.
 */
std::string ColorFormatter::getColorCode(EventCode command) {
  if (CommandColors.contains(command)) {
    return CommandColors[command].data();
  }
  return gray.data(); // Default to no color
}
/**
 *
 * @return Returns Pop() for ImGui color encoding. Used at the end of color encoding to indicate the end of it.
 */
std::string ColorFormatter::popColor() {
  return "&pop-color;"; // Default to no color
}
/**
 *
 * @param preset
 * @return Input: Color enum, returns an ImGui formatted string based off the enum.
 */
std::string ColorFormatter::getColor(Color preset) {
  switch (preset) {
  case Color::Gray:
    return gray.data();
  case Color::Red:
    return red.data();
  case Color::Default:
  default:
    return "&push-color=255,255,0;";
  }
}