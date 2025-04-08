#include "Database/ColorFormatter.hpp"

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"

#include <string_view>

using namespace std::string_view_literals;
// clang-format off
namespace solarized {
constexpr auto indigo  = "&push-color=0x8d,0x66,0xc6;"sv;
constexpr auto blue    = "&push-color=0x26,0x8B,0xD2;"sv;
constexpr auto red     = "&push-color=0xcb,0x6a,0x6a;"sv;
constexpr auto yellow  = "&push-color=0xB5,0x89,0x00;"sv;
constexpr auto orange  = "&push-color=0xCB,0x4B,0x16;"sv;
constexpr auto magenta = "&push-color=0xD3,0x36,0x82;"sv;
constexpr auto cyan    = "&push-color=0x2A,0xA1,0x98;"sv;
constexpr auto violet  = "&push-color=0x6C,0x71,0xC4;"sv;
constexpr auto base00  = "&push-color=0x65,0x7B,0x83;"sv;
constexpr auto base01  = "&push-color=0x58,0x6E,0x75;"sv;
constexpr auto base02  = "&push-color=0x07,0x36,0x42;"sv;
constexpr auto base03  = "&push-color=0x00,0x02,0x36;"sv;
constexpr auto base0   = "&push-color=0x83,0x94,0x96;"sv;
constexpr auto base1   = "&push-color=0x96,0xA1,0xA1;"sv;
constexpr auto base2   = "&push-color=0xEE,0xE8,0xD5;"sv;
constexpr auto base3   = "&push-color=0xFD,0xF6,0xE3;"sv;
constexpr auto purple  = "&push-color=0x81,0x7D,0xFF;"sv;
constexpr auto maroon  = "&push-color=0xc8,0x7a,0x7a;"sv;
}
namespace raw {
constexpr auto indigo      = "&push-color=0x4b,0x00,0x82;"sv;
constexpr auto blue        = "&push-color=0x00,0x00,0xff;"sv;
constexpr auto red         = "&push-color=0xff,0x00,0x00;"sv;
constexpr auto darkorange  = "&push-color=0xff,0x8c,0x00;"sv;
constexpr auto magenta     = "&push-color=0xff,0x00,0xff;"sv;
constexpr auto maroon      = "&push-color=0x80,0x00,0x00;"sv;
constexpr auto olive       = "&push-color=0x80,0x80,0x00;"sv;
constexpr auto crimson     = "&push-color=0xdc,0x14,0x3c;"sv;
constexpr auto purple      = "&push-color=0x80,0x00,0x80;"sv;
constexpr auto teal        = "&push-color=0x00,0x80,0x80;"sv;
constexpr auto deeppink    = "&push-color=0xff,0x14,0x93;"sv;
constexpr auto dodgerblue  = "&push-color=0x1e,0x90,0xff;"sv;
constexpr auto darkviolet  = "&push-color=0x94,0x00,0xd3;"sv;
constexpr auto gray        = "&push-color=0x80,0x80,0x80;"sv;
constexpr auto green       = "&push-color=0x00,0x80,0x00;"sv;
constexpr auto white       = "&push-color=0xFF,0xFF,0xFF;"sv;
}
// clang-format on

static std::map<EventCode, std::string_view> CommandColors{
    // clang-format off
  {EventCode::Event_Dummy,              raw::white},
  {EventCode::Show_Text,                solarized::indigo},
  {EventCode::Next_Text,                solarized::indigo},
  {EventCode::Show_Choices,             solarized::indigo},
  {EventCode::Input_Number,             solarized::indigo},
  {EventCode::When_Selected,            solarized::indigo},
  {EventCode::When_Cancel,              solarized::indigo},
  {EventCode::End_del_ShowChoices,      solarized::indigo},
  {EventCode::Select_Item,              solarized::indigo},
  {EventCode::Show_Scrolling_Text,      solarized::indigo},
  {EventCode::Next_Scrolling_Text,      solarized::indigo},
  {EventCode::Comment,                  raw::green},
  {EventCode::Next_Comment,             raw::green},
  {EventCode::Conditional_Branch,       solarized::blue},
  {EventCode::Else,                     solarized::blue},
  {EventCode::End,                      solarized::blue},
  {EventCode::Loop,                     solarized::blue},
  {EventCode::Repeat_Above,             solarized::blue},
  {EventCode::Break_Loop,               solarized::blue},
  /*{EventCode::Continue_Loop,            solarized::blue}, Unused */
  {EventCode::Exit_Event_Processing,    solarized::blue},
  {EventCode::Common_Event,             solarized::blue},
  {EventCode::Label,                    solarized::blue},
  {EventCode::Jump_To_Label,            solarized::blue},
  {EventCode::Control_Switches,         solarized::red},
  {EventCode::Control_Variables,        solarized::red},
  {EventCode::Control_Self_Switch,      solarized::red},
  {EventCode::Control_Timer,            solarized::red},
  {EventCode::Change_Gold,              raw::darkorange},
  {EventCode::Change_Items,             raw::darkorange},
  {EventCode::Change_Weapons,           raw::darkorange},
  {EventCode::Change_Armors,            raw::darkorange},
  {EventCode::Change_Party_Member,      raw::darkorange},
  {EventCode::Change_Battle_BGM,        solarized::magenta},
  {EventCode::Change_Victory_ME,        solarized::magenta},
  {EventCode::Change_Save_Access,       solarized::magenta},
  {EventCode::Change_Menu_Access,       solarized::magenta},
  {EventCode::Change_Encounter_Disable, solarized::magenta},
  {EventCode::Change_Formation_Access,  solarized::magenta},
  {EventCode::Change_Window_Color,      solarized::magenta},
  {EventCode::Change_Defeat_ME,         solarized::magenta},
  {EventCode::Change_Vechicle_BGM,      solarized::magenta},
  {EventCode::Transfer_Player,          solarized::maroon},
  {EventCode::Set_Vehicle_Location,     solarized::maroon},
  {EventCode::Set_Event_Location,       solarized::maroon},
  {EventCode::Scroll_Map,               solarized::maroon},
  {EventCode::Set_Movement_Route,       solarized::maroon},
  {EventCode::Movement_Route_Step,      solarized::maroon},
  {EventCode::Get_On_Off_Vehicle,       solarized::maroon},
  {EventCode::Change_Transparency,      solarized::maroon},
  {EventCode::Change_Player_Followers,  solarized::maroon},
  {EventCode::Gather_Followers,         solarized::maroon},
  {EventCode::Show_Animation,           solarized::maroon},
  {EventCode::Show_Balloon_Icon,        solarized::maroon},
  {EventCode::Erase_Event,              solarized::maroon},
  {EventCode::Fadeout_Screen,           raw::olive},
  {EventCode::Fadein_Screen,            raw::olive},
  {EventCode::Tint_Screen,              raw::olive},
  {EventCode::Flash_Screen,             raw::olive},
  {EventCode::Shake_Screen,             raw::olive},
  {EventCode::Wait,                     raw::crimson},
  {EventCode::Show_Picture,             solarized::purple},
  {EventCode::Move_Picture,             solarized::purple},
  {EventCode::Rotate_Picture,           solarized::purple},
  {EventCode::Tint_Picture,             solarized::purple},
  {EventCode::Erase_Picture,            solarized::purple},
  {EventCode::Set_Weather_Effect,       raw::olive},
  {EventCode::Play_BGM,                 raw::teal},
  {EventCode::Fadeout_BGM,              raw::teal},
  {EventCode::Save_BGM,                 raw::teal},
  {EventCode::Resume_BGM,               raw::teal},
  {EventCode::Play_BGS,                 raw::teal},
  {EventCode::Fadeout_BGS,              raw::teal},
  {EventCode::Play_ME,                  raw::teal},
  {EventCode::Play_SE,                  raw::teal},
  {EventCode::Stop_SE,                  raw::teal},
  {EventCode::Play_Movie,               raw::teal},
  {EventCode::Change_Map_Name_Display,  raw::deeppink},
  {EventCode::Change_Tileset,           raw::deeppink},
  {EventCode::Change_Battle_Back,       raw::deeppink},
  {EventCode::Change_Parallax,          raw::deeppink},
  {EventCode::Get_Location_Info,        raw::deeppink},
  {EventCode::Battle_Processing,        raw::dodgerblue},
  {EventCode::If_Win,                   raw::dodgerblue},
  {EventCode::If_Escape,                raw::dodgerblue},
  {EventCode::If_Lose,                  raw::dodgerblue},
  {EventCode::End_del_Battle_Processing,raw::dodgerblue},
  {EventCode::Shop_Processing,          raw::dodgerblue},
  {EventCode::Shop_Processing_Good,     raw::dodgerblue},
  {EventCode::Name_Input_Processing,    raw::dodgerblue},
  {EventCode::Change_HP,                raw::darkorange},
  {EventCode::Change_MP,                raw::darkorange},
  {EventCode::Change_TP,                raw::darkorange},
  {EventCode::Change_State,             raw::darkorange},
  {EventCode::Recover_All,              raw::darkorange},
  {EventCode::Change_EXP,               raw::darkorange},
  {EventCode::Change_Level,             raw::darkorange},
  {EventCode::Change_Parameter,         raw::darkorange},
  {EventCode::Change_Skill,             raw::darkorange},
  {EventCode::Change_Equipment,         raw::darkorange},
  {EventCode::Change_Name,              raw::darkorange},
  {EventCode::Change_Class,             raw::darkorange},
  {EventCode::Change_Actor_Images,      solarized::magenta},
  {EventCode::Change_Vehicle_Image,     solarized::magenta},
  {EventCode::Change_Nickname,          raw::darkorange},
  {EventCode::Change_Profile,           raw::darkorange},
  {EventCode::Change_Enemy_HP,          solarized::violet},
  {EventCode::Change_Enemy_MP,          solarized::violet},
  {EventCode::Change_Enemy_TP,          solarized::violet},
  {EventCode::Change_Enemy_State,       solarized::violet},
  {EventCode::Enemy_Recover_All,        solarized::violet},
  {EventCode::Enemy_Appear,             solarized::violet},
  {EventCode::Enemy_Transform,          solarized::violet},
  {EventCode::Show_Battle_Animation,    solarized::violet},
  {EventCode::Force_Action,             solarized::violet},
  {EventCode::Abort_Battle,             solarized::violet},
  {EventCode::Open_Menu_Screen,         raw::dodgerblue},
  {EventCode::Open_Save_Screen,         raw::dodgerblue},
  {EventCode::Return_To_Title_Screen,   raw::dodgerblue},
  {EventCode::Game_Over,                raw::dodgerblue},
  {EventCode::Script,                   solarized::indigo},
  {EventCode::Next_Script,              solarized::indigo},
  {EventCode::PluginMV_Command,         solarized::indigo},
  {EventCode::PluginMZ_Command,         solarized::indigo},
  {EventCode::Next_PluginMZ_Command,    solarized::indigo},
    // clang-format on
};
/**
 *
 * @param command
 * @param color
 * @return Returns an ImGui formatted string for the specific command name.
 */
std::string ColorFormatter::getColorCode(const EventCode command, const bool color) {
  if (!color) {
    return "";
  }

  if (CommandColors.contains(command)) {
    return CommandColors[command].data();
  }
  return raw::gray.data(); // Default to no color
}
/**
 *
 * @return Returns Pop() for ImGui color encoding. Used at the end of color encoding to indicate the end of it.
 */
std::string ColorFormatter::popColor(const bool color) {
  return color ? "&pop-color;" : ""; // Default to no color
}
/**
 *
 * @param preset
 * @param color
 * @return Input: Color enum, returns an ImGui formatted string based off the enum.
 */
std::string ColorFormatter::getColor(const FormatColor preset, const bool color) {
  if (!color) {
    return "";
  }
  switch (preset) {
  case FormatColor::Gray:
    return raw::gray.data();
  case FormatColor::Red:
    return solarized::red.data();
  case FormatColor::Default:
  default:
    return "&push-color=255,255,0;";
  }
}