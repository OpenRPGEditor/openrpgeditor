#include "Database/ColorFormatter.hpp"

#include "Database/CommandParser.hpp"
#include "Database/Globals.hpp"

#include <string_view>

using namespace std::string_view_literals;
// clang-format off
namespace solarized {
constexpr auto indigo  = "&push-color=#8d66c6;"sv;
constexpr auto blue    = "&push-color=#268BD2;"sv;
constexpr auto red     = "&push-color=#cb6a6a;"sv;
constexpr auto yellow  = "&push-color=#B58900;"sv;
constexpr auto orange  = "&push-color=#CB4B16;"sv;
constexpr auto magenta = "&push-color=#D33682;"sv;
constexpr auto cyan    = "&push-color=#2AA198;"sv;
constexpr auto violet  = "&push-color=#6C71C4;"sv;
constexpr auto base00  = "&push-color=#657B83;"sv;
constexpr auto base01  = "&push-color=#586E75;"sv;
constexpr auto base02  = "&push-color=#073642;"sv;
constexpr auto base03  = "&push-color=#000236;"sv;
constexpr auto base0   = "&push-color=#839496;"sv;
constexpr auto base1   = "&push-color=#96A1A1;"sv;
constexpr auto base2   = "&push-color=#EEE8D5;"sv;
constexpr auto base3   = "&push-color=#FDF6E3;"sv;
constexpr auto purple  = "&push-color=#817DFF;"sv;
constexpr auto maroon  = "&push-color=#c87a7a;"sv;
}
namespace raw {
constexpr auto indigo      = "&push-color=#4b0082;"sv;
constexpr auto blue        = "&push-color=#0000ff;"sv;
constexpr auto red         = "&push-color=#ff0000;"sv;
constexpr auto darkorange  = "&push-color=#ff8c00;"sv;
constexpr auto magenta     = "&push-color=#ff00ff;"sv;
constexpr auto maroon      = "&push-color=#800000;"sv;
constexpr auto olive       = "&push-color=#808000;"sv;
constexpr auto crimson     = "&push-color=#dc143c;"sv;
constexpr auto purple      = "&push-color=#800080;"sv;
constexpr auto teal        = "&push-color=#008080;"sv;
constexpr auto deeppink    = "&push-color=#ff1493;"sv;
constexpr auto dodgerblue  = "&push-color=#1e90ff;"sv;
constexpr auto darkviolet  = "&push-color=#9400d3;"sv;
constexpr auto gray        = "&push-color=#808080;"sv;
constexpr auto green       = "&push-color=#008000;"sv;
constexpr auto white       = "&push-color=#FFFFFF;"sv;
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
    return {};
  }
  switch (preset) {
  case FormatColor::Gray:
    return raw::gray.data();
  case FormatColor::Red:
    return solarized::red.data();
  case FormatColor::Default:
  default:
    return "&push-color=#FFFF00;";
  }
}