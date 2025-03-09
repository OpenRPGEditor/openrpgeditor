#include "Core/LibLCF.hpp"
#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixOFF.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixON.hpp"
#include "Database/EventCommands/MovementRoute/Move1StepForward.hpp"
#include "Database/EventCommands/MovementRoute/MoveAtRandom.hpp"
#include "Database/EventCommands/MovementRoute/MoveAwayFromPlayer.hpp"
#include "Database/EventCommands/MovementRoute/MoveDown.hpp"
#include "Database/EventCommands/MovementRoute/MoveLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveLowerLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveLowerRight.hpp"
#include "Database/EventCommands/MovementRoute/MoveRight.hpp"
#include "Database/EventCommands/MovementRoute/MoveTowardPlayer.hpp"
#include "Database/EventCommands/MovementRoute/MoveUp.hpp"
#include "Database/EventCommands/MovementRoute/MoveUpperLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveUpperRight.hpp"
#include "Database/EventCommands/MovementRoute/Script.hpp"
#include "Database/EventCommands/MovementRoute/SteppingAnimationOFF.hpp"
#include "Database/EventCommands/MovementRoute/SteppingAnimationON.hpp"
#include "Database/EventCommands/MovementRoute/ThroughOFF.hpp"
#include "Database/EventCommands/MovementRoute/ThroughON.hpp"
#include "Database/EventCommands/MovementRoute/Turn180Deg.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegLeft.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegLeftOrRight.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegRight.hpp"
#include "Database/EventCommands/MovementRoute/TurnAtRandom.hpp"
#include "Database/EventCommands/MovementRoute/TurnAwayFromPlayer.hpp"
#include "Database/EventCommands/MovementRoute/TurnDown.hpp"
#include "Database/EventCommands/MovementRoute/TurnLeft.hpp"
#include "Database/EventCommands/MovementRoute/TurnRight.hpp"
#include "Database/EventCommands/MovementRoute/TurnTowardPlayer.hpp"
#include "Database/EventCommands/MovementRoute/TurnUp.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"
#include "MainWindow.hpp"
#include "Settings.hpp"
#include "imgui.h"
#include "lcf/reader_util.h"
#include "nfd.h"

void LibLCF::draw() {
  if (!m_isOpen) {
    return;
  }
  if (ImGui::Begin("LCF")) {
    if (ImGui::Button("Select an RPG Maker 2000 project...")) {
      nfdu8char_t* loc;
      const auto result = NFD_PickFolder(&loc, !Settings::instance()->lastDirectory.empty() ? Settings::instance()->lastDirectory.c_str() : nullptr);
      if (result == NFD_OKAY) {
        const std::filesystem::path path{loc};
        lcf.setProject(path);
        lcf.loadProject();
        selectedMapIndex = -1;
        selectedPage = -1;
        NFD_FreePathU8(loc);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("...")) {}
    if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
    {
      ImGui::BeginDisabled(m_unresolvedError);
      if (ImGui::Button("Create Events")) {
        // LibLCF => Map
        if (m_unresolvedError) {
          m_unresolvedError = lcf.mapper()->hasUnresolvedPairs();
        }
        if (m_parent->currentMap() != nullptr) {
          for (auto& ev : map->events) {
            if (!lcf.mapper()->isUnresolved()) {
              convertEvent(m_parent->currentMap()->createNewEvent(), ev);
            } else {
              m_unresolvedError = true;
            }
          }
          if (m_unresolvedError) {
            lcf.mapper()->save(Database::instance()->basePath);
          }
        }
      }
      ImGui::EndDisabled();
      ImGui::EndPopup();
    }
    if (m_unresolvedError) {
      ImGui::SameLine();
      ImGui::TextUnformatted("Unresolved mappings pending!");
    }
    ImGui::SameLine();
    if (ImGui::Button("\u2316")) {
      selectedMapIndex = m_parent->currentMapInfo()->id();
      selectedPage = -1;
      selectedEvent = -1;
      map = std::move(lcf.loadMap(selectedMapIndex));
      m_navigate = true;
    }

    if (ImGui::BeginListBox("##lcf_map_list", ImVec2(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2, 0))) {
      if (lcf.treeMap()) {
        for (int i = 1; i < lcf.treeMap()->maps.size(); i++) {
          if (ImGui::Selectable(lcf.treeMap()->maps[i].name.c_str(), selectedMapIndex == i)) {
            selectedMapIndex = i;
            selectedPage = -1;
            selectedEvent = -1;
            map = std::move(lcf.loadMap(i));
          }
          if (i == selectedMapIndex && m_navigate) {
            ImGui::SetScrollHereY();
            m_navigate = false;
          }
        }
      }
      ImGui::EndListBox();
    }
    std::string preview =
        map && selectedEvent != -1 ? std::format("{} {}x{}", map->events[selectedEvent].name.c_str(), map->events[selectedEvent].x, map->events[selectedEvent].y) : "No Event Selected";
    ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x / 2);
    if (ImGui::BeginCombo("##map_event_combo", preview.c_str())) {
      if (map) {
        for (int i = 0; i < map->events.size(); i++) {
          if (ImGui::Selectable(std::format("{} {}x{}", map->events[i].name.c_str(), map->events[i].x, map->events[i].y).c_str(), selectedEvent == i)) {
            selectedEvent = i;
            selectedPage = 0;
          }
          if (i == selectedEvent) {
            ImGui::SetItemDefaultFocus();
          }
        }
      }
      ImGui::EndCombo();
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - ImGui::GetStyle().FramePadding.x);
    if (ImGui::BeginCombo("##map_event_page_combo", map && selectedEvent != -1 && selectedPage != -1 ? std::format("Page {}", selectedPage + 1).c_str() : "No Page Selected")) {
      if (map && selectedEvent != -1) {
        for (int i = 0; i < map->events[selectedEvent].pages.size(); i++) {
          if (ImGui::Selectable(std::format("Page {}", i + 1).c_str(), selectedPage == i)) {
            selectedPage = i;
          }
          if (i == selectedPage) {
            ImGui::SetItemDefaultFocus();
          }
        }
      }
      ImGui::EndCombo();
    }
    if (ImGui::BeginListBox("##lcf_event", ImVec2(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x * 2, ImGui::GetContentRegionAvail().y - ImGui::GetStyle().FramePadding.y * 2))) {
      if (map) {
        ImGui::Text("Event %s", selectedEvent != -1 ? map->events[selectedEvent].name.c_str() : "");
        ImGui::Text("Commands:");
        if (selectedEvent != -1 && selectedPage != -1) {
          for (int step = 0; const auto& cmd : map->events[selectedEvent].pages[selectedPage].event_commands) {
            switch (static_cast<lcf::rpg::EventCommand::Code>(cmd.code)) {
            case lcf::rpg::EventCommand::Code::CallEvent: {
              const auto type = cmd.parameters[0];
              std::string rep = std::string(cmd.indent * 2, ' ') + "@> Call Event: ";
              switch (type) {
              case 0: {
                // Common Event
                auto id = cmd.parameters[1];
                auto event = std::ranges::find_if(lcf.database()->commonevents, [&](const auto& e) { return e.ID == id; });
                if (event != lcf.database()->commonevents.end()) {
                  rep += event->name.c_str();
                }
              } break;
              case 1: {
                // Map Event
                const auto id = cmd.parameters[1];
                const auto event = std::ranges::find_if(map->events, [&](const auto& e) { return e.ID == id; });
                if (event != map->events.end()) {
                  rep += std::format("[{}][{}]", event->name.c_str(), cmd.parameters[2]);
                }
              } break;
              case 2: {
                // Map Event (Variable)
                const auto eventId = cmd.parameters[1];
                const auto event = std::ranges::find_if(lcf.database()->variables, [&](const auto& e) { return e.ID == eventId; });
                const auto pageId = cmd.parameters[2];
                const auto page = std::ranges::find_if(lcf.database()->variables, [&](const auto& e) { return e.ID == pageId; });
                if (event != lcf.database()->variables.end() && page != lcf.database()->variables.end()) {
                  rep += std::format("[{}][{}]", event->name.c_str(), page->name.c_str());
                }
              } break;
              default:
                break;
              }
              ImGui::Selectable(rep.c_str());
            } break;
            default: {
              std::string parameters = "[";
              for (int i = 0; const auto parameter : cmd.parameters) {
                parameters += (i == 0) ? std::to_string(parameter) : ", " + std::to_string(parameter);
                ++i;
              }
              parameters += "]";
              if (ImGui::Selectable(std::format("{}{:05}-{}: {}, {}, {}###{}", std::string(cmd.indent * 4, ' '), cmd.code, lcf::rpg::EventCommand::kCodeTags[cmd.code], cmd.indent, cmd.string.c_str(),
                                                parameters, step)
                                        .c_str())) {}
            } break;
            }
            step++;
          }
        }
      }
      ImGui::EndListBox();
    }
  }
  if (ImGui::Begin("Mapping")) {

    auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNO");
    calc.y *= 16;
    calc.y += ImGui::GetStyle().ItemSpacing.y * 32;

    if (ImGui::BeginTabBar("##lcf_mapping_tabbar")) {
      if (lcf.treeMap()) {
        if (ImGui::BeginTabItem("Switches##lcf_mapping_switches")) {
          if (ImGui::BeginTable("##lcf_mapping_table", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableColumnFlags_WidthFixed,
                                ImVec2{ImGui::GetContentRegionAvail().x, calc.y})) {

            ImGui::TableSetupColumn("RPG2000");
            ImGui::TableSetupColumn("MV/MZ");
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            int index{0};
            for (auto& pair : lcf.mapper()->switch_mapping) {
              ImGui::PushID(std::format("{}_data_{}", pair.first, index).c_str());
              // ImGui::TextUnformatted(std::to_string(pair.first).c_str());
              ImGui::TextUnformatted(std::format("{} ({})", ToString(lcf.database()->switches.at(pair.first).name), pair.first).c_str());
              ImGui::PopID();
              ImGui::TableNextColumn();
              ImGui::PushID(std::format("{}_data_{}", pair.second, index).c_str());
              ImGui::TextUnformatted(Database::instance()->switchName(pair.second).c_str());
              ImGui::PopID();
              ImGui::InputInt(std::format("##lcf_mapping_text_{}", index).c_str(), &m_int_switch_mapping[pair.first], 0);
              ImGui::TableNextColumn();
              index++;
            }
            ImGui::EndTable();
          }
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Variables##lcf_mapping_variables")) {}
        if (ImGui::BeginTabItem("Sounds##lcf_mapping_sound_effects")) {}
        if (ImGui::BeginTabItem("Images##lcf_mapping_switches")) {}
      }
      if (lcf.treeMap()) {
        if (ImGui::TabItemButton("(Save Changes)##lcf_mapping_save_all")) {
          // Save and double-check all unresolved
          lcf.mapper()->save(Database::instance()->basePath);
          m_unresolvedError = lcf.mapper()->hasUnresolvedPairs();
        }
      }
      ImGui::EndTabBar();
    }
  }
  ImGui::End();
  ImGui::End();
}
std::shared_ptr<IEventCommand> LibLCF::createCommand(int32_t code, int32_t indent, const lcf::DBArray<int32_t> data, const std::string& strData) {

  auto codeEnum = static_cast<lcf::rpg::EventCommand::Code>(code);

  std::vector<int> parameters;
  for (auto it = data.begin(); it < data.end(); it++) {
    parameters.push_back(*it);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ControlVars)) {
    if (parameters.at(1) == parameters.at(2) && parameters.at(0) == 0) {
      ControlVariables newCmd;
      newCmd.setIndent(indent);

      // TODO: We want to do mapping variables between RPG2000 and ORE at this point to find the right values.
      newCmd.start = parameters.at(1);
      newCmd.end = parameters.at(2);

      // Set => + - * / Mod
      newCmd.operation = static_cast<VariableControlOperation>(parameters.at(3));

      // if (parameters.at(4) == 0)                   // Constant
      //   if (parameters.at(4) == 1)                 // Variable (Not supporting right now)
      //     if (parameters.at(4) == 2)               // Variable => Variable No (Not supported)
      //       if (parameters.at(4) == 3)             // Random
      //         if (parameters.at(4) == 4)           // Item (Not supported)
      //           if (parameters.at(4) == 5)         // Hero (Not supported)
      //             if (parameters.at(4) == 6)       // Event (Not supported)
      //               if (parameters.at(4) == 7)     // Other (Not supported)

      if (parameters.at(4) == 0) { // Constant
        newCmd.operand = static_cast<VariableControlOperand>(parameters.at(4));
      } else if (parameters.at(4) == 3) { // Random
        newCmd.operand = static_cast<VariableControlOperand>(parameters.at(4));
        newCmd.random.min = parameters.at(1);
        newCmd.random.max = parameters.at(2);
      } else {
        return Comment(lcf::rpg::EventCommand::kCodeTags[code], "Non-constant", indent);
      }
      newCmd.constant = parameters.at(5);
      return std::make_shared<ControlVariables>(newCmd);
    }
    if (parameters.at(0) == 1) {
      return Comment(lcf::rpg::EventCommand::kCodeTags[code], "Unsupported range conversion", indent);
    }
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], "Unsupported VariableNo conversion", indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ControlSwitches)) {
    ControlSwitches newCmd;
    newCmd.setIndent(indent);

    newCmd.start = parameters.at(1);
    newCmd.end = parameters.at(2);

    newCmd.turnOff = static_cast<ValueControl>(parameters.at(3));

    if (parameters.at(3) > 1) {
      return Comment(lcf::rpg::EventCommand::kCodeTags[code], "Unsupported ON/OFF Trigger", indent);
    }
    return std::make_shared<ControlSwitches>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::END)) {
    EventDummy newCmd;
    newCmd.setIndent(indent);
    // m_attachDummy = true;
    return std::make_shared<EventDummy>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndLoop)) {
    RepeatAboveCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<RepeatAboveCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Comment)) {
    CommentCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.text = strData;
    return std::make_shared<CommentCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Comment_2)) {
    // Note: This is used to build a comment command but we're just keeping them seperate
    CommentCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.text = strData;
    return std::make_shared<CommentCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Loop)) {
    LoopCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<LoopCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::BreakLoop)) {
    BreakLoopCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<BreakLoopCommand>(newCmd);
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::CallCommonEvent)) {
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], "Call Common Event ID: " + std::to_string(parameters.at(0)), indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ForceFlee)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnableCombo)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeClass)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeBattleCommands)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenLoadMenu)) {
    OpenMenuCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<OpenMenuCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ExitGame)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ToggleAtbMode)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ToggleFullscreen)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenVideoOptions)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowMessage)) {
    ShowTextCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.position = static_cast<TextWindowPosition>(m_windowPosition);
    newCmd.background = m_windowTransparent ? TextBackground::Transparent : TextBackground::Window;

    // newCmd.textLine = "{}";
    NextTextCommand nextText;
    nextText.text = "{}";
    nextText.setIndent(indent);
    // newCmd.addText(&nextText);
    newCmd.text.push_back(std::make_shared<NextTextCommand>(nextText));
    return std::make_shared<ShowTextCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MessageOptions)) {
    m_windowTransparent = parameters.at(0) != 0;
    m_windowPosition = parameters.at(1);
    // parameters.at(2) // Prevent heroes from hiding (not supported)
    // parameters.at(3) // Allow other events to continue (not supported)
    return nullptr;
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeFaceGraphic)) {
    PluginCommandMV newCmd;
    newCmd.setIndent(indent);

    if (strData.empty()) {
      newCmd.command = "ClearCurrentFace";
    } else {
      newCmd.command = std::format("SetFace {} {}", strData, parameters.at(0));
    }
    return std::make_shared<PluginCommandMV>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowChoice)) {
    ShowChoiceCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.choices.clear();
    for (auto& str : splitString(strData, '/')) {
      newCmd.choices.push_back(str);
    }

    // parameters.at(1) =>
    // 0 = disallow
    // 1 - 4 = choose choice
    // 5 = branch
    if (parameters.at(0) == 5) {
      newCmd.cancelType = -2;
    } else if (parameters.at(0) == 0) {
      newCmd.cancelType = -1;
    } else {
      newCmd.cancelType = parameters.at(0) - 1;
    }
    return std::make_shared<ShowChoiceCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::InputNumber)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TimerOperation)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeGold)) {
    ChangeGoldCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.operation = static_cast<QuantityChangeOp>(parameters.at(0));
    newCmd.operandSource = static_cast<QuantityChangeSource>(parameters.at(1));
    newCmd.operand = parameters.at(2);

    return std::make_shared<ChangeGoldCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeItems)) {
    ChangeItemsCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.operation = static_cast<QuantityChangeOp>(parameters.at(0));
    newCmd.operandSource = static_cast<QuantityChangeSource>(parameters.at(3));
    newCmd.operand = parameters.at(4);
    attachToCommand("Manual selection required!");

    return std::make_shared<ChangeItemsCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangePartyMembers)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeExp)) {
    ChangeEXPCommand newCmd;
    newCmd.setIndent(indent);

    if (parameters.at(0) == 0) {
      newCmd.value = 0;
    } else {
      newCmd.comparison = static_cast<ActorComparisonSource>(parameters.at(0) - 1);
      newCmd.value = parameters.at(1); // TODO: Needs mapping or warning
    }
    newCmd.quantityOp = static_cast<QuantityChangeOp>(parameters.at(2));
    newCmd.quantitySource = static_cast<QuantityChangeSource>(parameters.at(3));
    newCmd.quantity = parameters.at(4); // TODO: Needs mapping or warning
    newCmd.showLevelUp = parameters.at(5) != 0;

    return std::make_shared<ChangeEXPCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeLevel)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeParameters)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSkills)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEquipment)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHP)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSP)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeCondition)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::FullHeal)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::SimulatedAttack)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHeroName)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHeroTitle)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSpriteAssociation)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeActorFace)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeVehicleGraphic)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemBGM)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemSFX)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemGraphics)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeScreenTransitions)) {}

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnemyEncounter)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenShop)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowInn)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnterHeroName)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Teleport)) {
    attachToCommand("MapID: " + std::to_string(parameters.at(0)));
    attachToCommand("X: " + std::to_string(parameters.at(1)));
    attachToCommand("Y: " + std::to_string(parameters.at(2)));
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MemorizeLocation)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::RecallToLocation)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnterExitVehicle)) {
    GetOnOffVehicleCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<GetOnOffVehicleCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::SetVehicleLocation)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEventLocation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TradeEventLocations)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::StoreTerrainID)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::StoreEventID)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EraseScreen)) {
    // TODO: These have unique fade-outs not supported by modern versions
    FadeoutScreenCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<FadeoutScreenCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowScreen)) {
    FadeinScreenCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<FadeinScreenCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TintScreen)) {
    TintScreenCommand newCmd;
    newCmd.setIndent(indent);

    double r = -255 + parameters.at(0) / 100 * 255;
    double g = -255 + parameters.at(1) / 100 * 255;
    double b = -255 + parameters.at(2) / 100 * 255;
    double gray = parameters.at(3) > 100 ? 0 : 255 - ((parameters.at(3) / 100) * 255);
    if (parameters.at(3) > 100) {
      attachToCommand("Chroma is greater than 100");
    }

    newCmd.color.r = static_cast<int>(r);
    newCmd.color.g = static_cast<int>(g);
    newCmd.color.b = static_cast<int>(b);
    newCmd.color.gray = static_cast<int>(gray);
    newCmd.duration = parameters.at(4) == 0 ? 1 : parameters.at(4) * 6;

    newCmd.waitForCompletion = parameters.at(5) != 0;

    return std::make_shared<TintScreenCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::FlashScreen)) {
    FlashScreenCommand newCmd;
    newCmd.setIndent(indent);

    double rgbVal = 255 / 31;
    newCmd.color.r = parameters.at(0) * rgbVal;
    newCmd.color.g = parameters.at(1) * rgbVal;
    newCmd.color.b = parameters.at(2) * rgbVal;
    newCmd.color.intensity = parameters.at(3) * rgbVal;
    newCmd.duration = parameters.at(4) == 0 ? 1 : parameters.at(4) * 6;
    newCmd.waitForCompletion = parameters.at(5) != 0;

    return std::make_shared<FlashScreenCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShakeScreen)) {
    ShakeScreenCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.power = parameters.at(0);
    newCmd.speed = parameters.at(1);
    newCmd.duration = parameters.at(2) == 0 ? 1 : parameters.at(2) * 6;
    newCmd.waitForCompletion = parameters.at(3) != 0;

    return std::make_shared<ShakeScreenCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PanScreen)) {
    PluginCommandMV newCmd;
    ScriptCommand newCmd_script;
    newCmd.setIndent(indent);

    int scrollType = parameters.at(0); // 0 = fixed; 1 = cancel fixed; 2 = move pos; 3 = return pos;
    Direction scrollDirection = getDirection(parameters.at(1));
    int tileCount = parameters.at(2);
    int scrollSpeed = parameters.at(3);
    int waitForCompletion = parameters.at(4) != 0;

    if (scrollType == 0) {
      newCmd_script.script = "VH_Plus.fixCamera(true);";
      return std::make_shared<ScriptCommand>(newCmd_script);
    }
    if (scrollType == 1) {
      newCmd_script.script = "VH_Plus.fixCamera(false);";
      return std::make_shared<ScriptCommand>(newCmd_script);
    }
    if (scrollType == 2) {
      newCmd.command = std::format("ScrollMap {} {} {} {}", std::to_string(static_cast<int>(scrollDirection)), tileCount, scrollSpeed, waitForCompletion != 0 ? "true" : "false");
    }
    if (scrollType == 3) {
      newCmd.command = std::format("ScrollReturn {} {}", scrollSpeed, waitForCompletion != 0 ? "true" : "false");
    }

    return std::make_shared<PluginCommandMV>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::WeatherEffects)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowPicture)) {
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], strData, indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MovePicture)) {
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], "id: " + std::to_string(parameters.at(0)), indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ErasePicture)) {
    ErasePictureCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.picture = parameters.at(0);

    return std::make_shared<ErasePictureCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowBattleAnimation)) {
    ShowAnimationCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.animation = parameters.at(0); // TODO: Needs mapping

    if (parameters.at(1) == 10001) {
      newCmd.character = -1;
    } // Hero
    else if (parameters.at(1) == 10005) { // This Event
      newCmd.character = 0;
    } else {
      if (parameters.at(1) > 0) {
        newCmd.character = 0;
        attachToCommand("Manual event ID needed: " + std::to_string(parameters.at(1)));
      }
    }
    newCmd.waitForCompletion = parameters.at(2) != 0;

    return std::make_shared<ShowAnimationCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlayerVisibility)) {
    ChangeTransparencyCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.transparency = static_cast<ValueControl>(parameters.at(0));

    return std::make_shared<ChangeTransparencyCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::FlashSprite)) {
    double rgbVal = 255 / 31;

    attachToCommand(std::format("{}, {}, {}, {}, {}", parameters.at(0) * rgbVal, parameters.at(1) * rgbVal, parameters.at(2) * rgbVal, parameters.at(3) * rgbVal,
                                parameters.at(4) != 0 ? "wait: true" : "wait: false")
                        .c_str());

    ShowAnimationCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.waitForCompletion = parameters.at(4) != 0;

    return std::make_shared<ShowAnimationCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MoveEvent)) {
    SetMovementRouteCommand newCmd;
    newCmd.setIndent(indent);

    if (parameters.at(0) == 10001) {
      newCmd.character = -1;
    } // Hero
    else if (parameters.at(0) == 10005) { // This Event
      newCmd.character = 0;
    } else {
      if (parameters.at(0) > 0) {
        newCmd.character = 0;
        attachToCommand("Manual event ID needed: " + std::to_string(parameters.at(0)));
      }
    }
    // parameters.at(1) => Movement Frequency (Not supported)
    // parameters.at(2) => Repeat Movements
    // parameters.at(3) => Skip if cannot move
    // Wait For Completion (MV) => Halt All Movement (manual action required)
    newCmd.route.setRepeat(parameters.at(2) != 0);
    newCmd.route.setSkippable(parameters.at(3) != 0);
    newCmd.route.setWait(false);

    std::vector<int> parametersTest;
    for (auto itt = data.begin() + 4; itt < data.end();) {

      if (m_jumpProcessing) {
        // Process commands through jump processing if it was activated from StartJump
        processJumpParameters(*itt, newCmd.route.list().back());
        itt++;
      } else {
        std::shared_ptr<IEventCommand> moveCmd = convertMovementCommand(DecodeMove(itt), indent);
        if (moveCmd != nullptr) {
          newCmd.route.list().push_back(moveCmd);
        }
      }
      // lcf::rpg::MoveCommand baseCmd;
      // baseCmd.command_id = *itt;
      //// Normal processing or if after EndJump
      // if (baseCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::switch_off) || baseCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::switch_on)) {
      //   baseCmd.parameter_a = DecodeInt(itt);
      // }
      // if (baseCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::change_graphic)) {
      //   baseCmd.parameter_string = lcf::DBString(DecodeString(itt));
      //   baseCmd.parameter_a = DecodeInt(itt);
      //   std::string test = ToString(baseCmd.parameter_string);
      // }
      // if (baseCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::play_sound_effect)) {
      //   baseCmd.parameter_string = lcf::DBString(DecodeString(itt));
      //   baseCmd.parameter_a = DecodeInt(itt);
      //   baseCmd.parameter_b = DecodeInt(itt);
      //   baseCmd.parameter_c = DecodeInt(itt);
      // }
      // parametersTest.push_back(*itt);
    }

    for (auto& stepCmd : newCmd.route.list()) {
      MovementRouteStepCommand cmd;
      cmd.step = stepCmd;

      newCmd.editNodes.push_back(std::make_shared<MovementRouteStepCommand>(cmd));
    }
    newCmd.route.list().push_back(std::make_shared<EventDummy>(EventDummy()));
    // Process commands after returning the route command
    // m_movementProcessing = true;

    return std::make_shared<SetMovementRouteCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ProceedWithMovement)) {
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], "( Wait for Completion )", indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::HaltAllMovement)) {
    return Comment(lcf::rpg::EventCommand::kCodeTags[code], "", indent);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Wait)) {
    WaitCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.duration = parameters.at(0) * 6;

    return std::make_shared<WaitCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlayBGM)) {
    PlayBGMCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.audio.setName(strData);
    newCmd.audio.setVolume(parameters.at(0));
    newCmd.audio.setPan(parameters.at(1));
    newCmd.audio.setPitch(parameters.at(2));

    return std::make_shared<PlayBGMCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::FadeOutBGM)) {
    FadeoutBGM newCmd;
    newCmd.setIndent(indent);
    newCmd.duration = parameters.at(0) == 0 ? 1 : parameters.at(0) / 1000;
    return std::make_shared<FadeoutBGM>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MemorizeBGM)) {
    SaveBGMCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<SaveBGMCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlayMemorizedBGM)) {
    ResumeBGMCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<ResumeBGMCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlaySound)) {
    PlaySECommand newCmd;
    newCmd.setIndent(indent);

    newCmd.audio.setName(strData);
    newCmd.audio.setVolume(parameters.at(0));
    newCmd.audio.setPan(parameters.at(1));
    newCmd.audio.setPitch(parameters.at(2));

    return std::make_shared<PlaySECommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlayMovie)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::KeyInputProc)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMapTileset)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangePBG)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEncounterSteps)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TileSubstitution)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TeleportTargets)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeTeleportAccess)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EscapeTarget)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEscapeAccess)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenSaveMenu)) {
    OpenSaveCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<OpenSaveCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSaveAccess)) {
    ChangeSaveAccessCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.access = static_cast<AccessMode>(parameters.at(0));
    return std::make_shared<ChangeSaveAccessCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenMainMenu)) {
    OpenMenuCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<OpenMenuCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMainMenuAccess)) {
    ChangeMenuAccessCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.access = static_cast<AccessMode>(parameters.at(0));
    return std::make_shared<ChangeMenuAccessCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ConditionalBranch)) {
    ConditionalBranchCommand newCmd;
    newCmd.setIndent(indent);

    // parameters.at(0) // Switch
    // => parameters.at(1) // Switch ID
    // => parameters.at(2) // True or False

    // parameters.at(1) // Variable
    // => parameters.at(1) // Variable ID
    // => parameters.at(2) // Constant or Variable Type
    // => parameters.at(3) // Data Value
    // => parameters.at(4) // Operand

    // parameters.at(2) // Timer Control
    // parameters.at(3) // Gold
    // parameters.at(4) // Item
    // parameters.at(5) // Actor
    // parameters.at(6) // Event
    // parameters.at(7) // Vehicle

    if (parameters.at(0) == 0) { // Switch
      newCmd.type = ConditionType::Switch;

      newCmd.globalSwitch.switchIdx = lcf.mapper()->switchValue(parameters.at(1));
      newCmd.globalSwitch.checkIfOn = static_cast<ValueControl>(parameters.at(2));
    }
    if (parameters.at(0) == 1) { // Variable
      newCmd.type = ConditionType::Variable;

      newCmd.variable.comparison = static_cast<VariableComparisonType>(parameters.at(4));
      newCmd.variable.id = parameters.at(1); // TODO: Needs mapping
      newCmd.variable.source = static_cast<VariableComparisonSource>(parameters.at(2));
      if (parameters.at(2) == 1) {
        // Variable Type
        newCmd.variable.otherId = parameters.at(3); // TODO: Needs mapping
      } else {
        // Constant Type
        newCmd.variable.constant = parameters.at(3);
      }
    }
    if (parameters.at(0) == 2) { // Timer Control
      attachToCommand("Timer Branch");
      newCmd.type = ConditionType::Timer;
      newCmd.timer.sec = parameters.at(1);
      newCmd.timer.comparison = static_cast<TimerComparisonType>(parameters.at(2));
    }
    if (parameters.at(0) == 3) { // Gold
      newCmd.type = ConditionType::Gold;
      newCmd.gold.type = static_cast<GoldComparisonType>(parameters.at(2));
      newCmd.gold.value = parameters.at(1);
    }
    if (parameters.at(0) == 4) { // Item
      newCmd.type = ConditionType::Item;
      attachToCommand("Manual item assignment required!");
    }
    if (parameters.at(0) == 5) { // Actor
      attachToCommand("Actor Branch");
      newCmd.type = ConditionType::Actor;
    }
    if (parameters.at(0) == 6) { // Event
      newCmd.type = ConditionType::Character;
      if (parameters.at(1) == 10001) {
        newCmd.character.id = -1;
      } else if (parameters.at(1) == 10005) {
        newCmd.character.id = 0;
      } else {
        attachToCommand("Manual event assignment required!");
        newCmd.character.id = 0;
      }
      newCmd.character.facing = getDirection(parameters.at(2));
      attachToCommand(DecodeEnumName(newCmd.character.facing));
    }
    if (parameters.at(0) == 7) { // Event
      newCmd.type = ConditionType::Vehicle;
      newCmd.vehicle.id = static_cast<VehicleType>(parameters.at(1));
    }

    return std::make_shared<ConditionalBranchCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Label)) {
    LabelCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.label = std::to_string(parameters.at(0));
    return std::make_shared<LabelCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::JumpToLabel)) {
    JumpToLabelCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.label = std::to_string(parameters.at(0));
    return std::make_shared<JumpToLabelCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndEventProcessing)) {
    ExitEventProcessingCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<ExitEventProcessingCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EraseEvent)) {
    EraseEventCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<EraseEventCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::CallEvent)) {
    CommonEventCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.event = parameters.at(0); // TODO: Needs mapping

    return std::make_shared<CommonEventCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::GameOver)) {
    GameOverCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<GameOverCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ReturntoTitleScreen)) {
    ReturnToTitleCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<ReturnToTitleCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterHP)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterMP)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterCondition)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowHiddenMonster)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeBattleBG)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowBattleAnimation_B)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ConditionalBranch_B)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TerminateBattle)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowMessage_2)) {
    // Skip this
    return nullptr;
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowChoiceOption)) {
    WhenSelectedCommand newCmd;
    newCmd.setIndent(indent);

    newCmd.choice = strData;
    newCmd.param1 = parameters.at(0);
    return std::make_shared<WhenSelectedCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowChoiceEnd)) {
    ShowChoicesEndCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<ShowChoicesEndCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::VictoryHandler)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EscapeHandler)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::DefeatHandler)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndBattle)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Transaction)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::NoTransaction)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndShop)) {
    EndCommand newCmd;
    newCmd.setIndent(indent);
    m_attachDummy = true;
    return std::make_shared<EndCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Stay)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::NoStay)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndInn)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ElseBranch)) {
    ElseCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<ElseCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndBranch)) {
    EndCommand newCmd;
    newCmd.setIndent(indent);
    m_attachDummy = true;
    return std::make_shared<EndCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ElseBranch_B)) {}
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndBranch_B)) {}
  // EasyRpg_TriggerEventAt
  // EasyRpg_SmartMoveRoute
  // EasyRpg_SmartStepToward
  // EasyRpg_CallMovementAction
  // EasyRpg_WaitForSingleMovement
  // EasyRpg_AnimateVariable
  // EasyRpg_SetInterpreterFlag
  // EasyRpg_ProcessJson
  // EasyRpg_CloneMapEvent
  // EasyRpg_DestroyMapEvent
  // EasyRpg_StringPictureMenu
  // Maniac_GetSaveInfo
  // Maniac_Save
  // Maniac_Load
  // Maniac_EndLoadProcess
  // Maniac_GetMousePosition
  // Maniac_SetMousePosition
  // Maniac_ShowStringPicture
  // Maniac_GetPictureInfo
  // Maniac_ControlBattle
  // Maniac_ControlAtbGauge
  // Maniac_ChangeBattleCommandEx
  // Maniac_GetBattleInfo
  // Maniac_ControlVarArray
  // Maniac_KeyInputProcEx
  // Maniac_RewriteMap
  // Maniac_ControlGlobalSave
  // Maniac_ChangePictureId
  // Maniac_SetGameOption
  // Maniac_CallCommand
  // Maniac_ControlStrings
  // Maniac_GetGameInfo
  // Maniac_EditPicture
  // Maniac_WritePicture
  // Maniac_AddMoveRoute
  // Maniac_EditTile
  // Maniac_ControlTextProcessing

  CommentCommand newCmd;
  newCmd.setIndent(indent);
  newCmd.text = "Unimplemented code: " + std::string(lcf::rpg::EventCommand::kCodeTags[codeEnum]);
  return std::make_shared<CommentCommand>(newCmd);
}
std::shared_ptr<IEventCommand> LibLCF::Comment(std::string codeName, std::string text, int indent) {
  CommentCommand newCmd;
  newCmd.setIndent(indent);
  newCmd.text = std::format("({}) {}", codeName, text);
  return std::make_shared<CommentCommand>(newCmd);
}
void LibLCF::attachToCommand(std::string text) {
  m_commandLogger.push_back("\u2605 " + text);
  m_attachToCommand = true;
}
void LibLCF::convertEvent(Event* event, const lcf::rpg::Event& ev) {
  // event->setName(ev.name.data()); Name field, not entirely needed
  event->setX(ev.x);
  event->setY(ev.y);

  for (int i = 0; i < ev.pages.size(); i++) {
    if (event->pages().size() - 1 < i) {
      event->pages().emplace_back();
    }
    convertPage(event->page(i), ev.pages[i]);
  }
}
void LibLCF::convertPage(EventPage* page, const lcf::rpg::EventPage& evPage) {
  // Image
  page->image().setCharacterName("001_AAA"); // Temporary for now. Maybe we add mapping?
  page->image().setDirection(getDirection(evPage.character_direction));
  page->image().setCharacterIndex(evPage.character_index);

  // page->image().setTileId(evPage.character_index); // Not sure about this.
  page->image().setPattern(evPage.character_pattern);
  // Movement
  page->setMoveSpeed(static_cast<MovementSpeed>(evPage.move_speed));
  page->setTrigger(static_cast<EventTriggerType>(evPage.trigger));
  page->setMoveFrequency(getMoveFrequency(evPage.move_frequency));
  page->setPriorityType(static_cast<EventPriority>(evPage.layer));
  page->setMoveType(getMoveType(evPage.move_type));

  // Animation
  // page->setThrough(false); // RPG2000 does not support Through on event pages
  if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_continuous) {
    log("Common, With Stepping");
    page->setWalkAnime(true);
    page->setStepAnime(true);
    page->setDirectionFix(false);
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_fixed_continuous) {
    log("Fixed Direction");
    page->setWalkAnime(false);
    page->setStepAnime(true);
    page->setDirectionFix(true);
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_fixed_graphic) {
    log("Fixed Graphic");
    page->setWalkAnime(false);
    page->setStepAnime(false);
    page->setDirectionFix(true);
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_fixed_non_continuous) {
    log("Without Stepping");
    page->setWalkAnime(false);
    page->setStepAnime(false);
    page->setDirectionFix(true);
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_non_continuous) {
    log("Common, without stepping");
    page->setWalkAnime(true);
    page->setStepAnime(false);
    page->setDirectionFix(false);
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_step_frame_fix) {
    log("Animation Type: Step Frame Fix (not implemented)");
  } else if (evPage.animation_type == lcf::rpg::EventPage::AnimType::AnimType_spin) {
    log("Animation Type: Spin (not implemented)");
  }
  for (auto& str : m_lcfLogger) {
    CommentCommand cmd = CommentCommand();
    cmd.text = str;
    cmd.setIndent(0);
    page->list().insert(page->list().end() - 1, std::make_shared<CommentCommand>(cmd));
  }
  m_lcfLogger.clear();

  // Data structures
  if (evPage.condition.flags.switch_a) {
    // Switch 1
    page->conditions().setSwitch1Valid(true);
    page->conditions().setSwitch1Id(evPage.condition.switch_a_id); // TODO: Needs mapping
  }
  if (evPage.condition.flags.switch_b) {
    page->conditions().setSwitch2Valid(true);
    page->conditions().setSwitch2Id(evPage.condition.switch_b_id); // TODO: Needs mapping
  }
  if (evPage.condition.flags.variable) {
    page->conditions().setVariableValid(true);
    page->conditions().setVariableId(evPage.condition.variable_id); // TODO: Needs mapping
    page->conditions().setVariableValue(evPage.condition.variable_value);
  }
  if (evPage.condition.flags.item) {
    attachToCommand("Item Condition");
  }
  if (evPage.condition.flags.actor) {
    attachToCommand("Actor Condition");
  }
  if (evPage.condition.flags.timer || evPage.condition.flags.timer2) {
    attachToCommand("Timer Page Condition");
  }
  convertCommands(&page->list(), evPage.event_commands);

  if (page->moveType() == MoveType::Custom) {
    int index{0};
    page->moveRoute().setRepeat(evPage.move_route.repeat != 0);
    page->moveRoute().setSkippable(evPage.move_route.skippable != 0);
    page->moveRoute().setWait(false);
    for (auto& rpgMoveCmd : evPage.move_route.move_commands) {
      page->moveRoute().addCommand(convertMovementCommand(rpgMoveCmd, 0), index);
      index++;
    }
  }
  // page->setName() // RPG2000 doesn't have page names; ORE does

  // These variables are reset per page. The scope doesn't exist outside of it.
  m_windowTransparent = false;
  m_windowPosition = 2;
}
void LibLCF::processJumpParameters(int32_t code, std::shared_ptr<IEventCommand>& cmd) {
  std::shared_ptr<MovementJumpCommand> jumpCmd = static_pointer_cast<MovementJumpCommand>(cmd);

  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_right)) {
    jumpCmd->x += 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_up)) {
    jumpCmd->y -= 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_down)) {
    jumpCmd->y += 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_left)) {
    jumpCmd->x -= 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_upright)) {
    jumpCmd->x -= 1;
    jumpCmd->y += 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_downright)) {
    jumpCmd->x += 1;
    jumpCmd->y += 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_downleft)) {
    jumpCmd->y += 1;
    jumpCmd->x -= 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::move_upleft)) {
    jumpCmd->y -= 1;
    jumpCmd->x -= 1;
    return;
  }
  if (code == static_cast<int>(lcf::rpg::MoveCommand::Code::end_jump)) {
    m_jumpProcessing = false;
    return;
  }
  jumpCmd->x = 99; // Unrecognized commands
  jumpCmd->y = 99; // Unrecognized commands
}
void LibLCF::convertCommands(std::vector<std::shared_ptr<IEventCommand>>* r_cmds, const std::vector<lcf::rpg::EventCommand>& s_cmds) {
  for (auto& cmd : s_cmds) {

    std::shared_ptr<IEventCommand> newCmd = createCommand(cmd.code, cmd.indent, cmd.parameters, ToString(cmd.string));

    if (newCmd == nullptr) {
      continue; // Skip commands if null
    }

    if (m_attachToCommand) {
      for (auto& str : m_commandLogger) {
        CommentCommand strCmd;
        strCmd.setIndent(cmd.indent);
        strCmd.text = str;
        r_cmds->insert(r_cmds->end() - 1, std::make_shared<CommentCommand>(strCmd));
      }
      m_commandLogger.clear();
      m_attachToCommand = false;
    }
    r_cmds->insert(r_cmds->end() - 1, newCmd);

    // if (m_attachDummy) {
    //   EventDummy eventDummy;
    //   eventDummy.setIndent(cmd.indent);
    //   r_cmds->insert(r_cmds->end() - 1, std::make_shared<EventDummy>(eventDummy));
    //   m_attachDummy = false;
    // }
  }
}
std::shared_ptr<IEventCommand> LibLCF::convertMovementCommand(lcf::rpg::MoveCommand moveCmd, int32_t indent) {
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::begin_jump)) {
    MovementJumpCommand newCmd;
    newCmd.setIndent(indent);
    m_jumpProcessing = true;
    return std::make_shared<MovementJumpCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_right)) {
    MovementMoveRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_up)) {
    MovementMoveUpCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveUpCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_down)) {
    MovementMoveDownCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveDownCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_left)) {
    MovementMoveLeftCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveLeftCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_upright)) {
    MovementMoveUpperRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveUpperRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_downright)) {
    MovementMoveLowerRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveLowerRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_downleft)) {
    MovementMoveLowerLeftCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveLowerLeftCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_upleft)) {
    MovementMoveUpperLeftCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveUpperLeftCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_random)) {
    MovementMoveAtRandomCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveAtRandomCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_towards_hero)) {
    MovementMoveTowardPlayerCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveTowardPlayerCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_away_from_hero)) {
    MovementMoveAwayFromPlayerCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMoveAwayFromPlayerCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::move_forward)) {
    MovementMove1StepForwardCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementMove1StepForwardCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_up)) {
    MovementTurnUpCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnUpCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_right)) {
    MovementTurnRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_down)) {
    MovementTurnDownCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnDownCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_left)) {
    MovementTurnLeftCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnLeftCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::turn_90_degree_right)) {
    MovementTurn90DegRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurn90DegRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::turn_90_degree_left)) {
    MovementTurn90DegLeftCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurn90DegLeftCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::turn_180_degree)) {
    MovementTurn180DegCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurn180DegCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::turn_90_degree_random)) {
    MovementTurn90DegLeftOrRightCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurn90DegLeftOrRightCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_random_direction)) {
    MovementTurnAtRandomCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnAtRandomCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_hero)) {
    MovementTurnTowardPlayerCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnTowardPlayerCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::face_away_from_hero)) {
    MovementTurnAwayFromPlayerCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementTurnAwayFromPlayerCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::wait)) {
    MovementWaitCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.duration = 6;
    return std::make_shared<MovementWaitCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::end_jump)) {
    m_jumpProcessing = false;
    return nullptr;
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::lock_facing)) {
    MovementDirectionFixONCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementDirectionFixONCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::unlock_facing)) {
    MovementDirectionFixOFFCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementDirectionFixOFFCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::increase_movement_speed)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.increaseSpeed();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::decrease_movement_speed)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.decreaseSpeed();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::increase_movement_frequence)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.increaseFrequency();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::decrease_movement_frequence)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.decreaseFrequency();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::switch_on)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "SWITCH ON: " + std::to_string(moveCmd.parameter_a); // TODO: Needs mapping
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::switch_off)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "SWITCH OFF " + std::to_string(moveCmd.parameter_a); // TODO: Needs mapping
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::change_graphic)) {
    if (moveCmd.parameter_string == "AAA") {
      MovementChangeImageCommand imageCmd;
      imageCmd.setIndent(indent);
      return std::make_shared<MovementChangeImageCommand>(imageCmd);
    }
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = std::format("IMAGE: {}, Index: {}", ToString(moveCmd.parameter_string), std::to_string(moveCmd.parameter_a)); // TODO: Needs mapping
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::play_sound_effect)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = std::format("SOUND: {}, ({},{},{})", ToString(moveCmd.parameter_string), std::to_string(moveCmd.parameter_a), std::to_string(moveCmd.parameter_b),
                                std::to_string(moveCmd.parameter_c)); // TODO: Needs mapping
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::walk_everywhere_on)) {
    MovementThroughONCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementThroughONCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::walk_everywhere_off)) {
    MovementThroughOFFCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementThroughOFFCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::stop_animation)) {
    MovementSteppingAnimationOFFCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementSteppingAnimationOFFCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::start_animation)) {
    MovementSteppingAnimationONCommand newCmd;
    newCmd.setIndent(indent);
    return std::make_shared<MovementSteppingAnimationONCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::increase_transp)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.increaseTransparency();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  if (moveCmd.command_id == static_cast<int>(lcf::rpg::MoveCommand::Code::decrease_transp)) {
    MovementScriptCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.script = "this.decreaseTransparency();";
    return std::make_shared<MovementScriptCommand>(newCmd);
  }
  MovementScriptCommand newCmd;
  newCmd.setIndent(indent);
  newCmd.script = std::format("Unrecognized command: {}", std::to_string(moveCmd.command_id));
  return std::make_shared<MovementScriptCommand>(newCmd);
}
MoveType LibLCF::getMoveType(int32_t move_type) {
  if (move_type == lcf::rpg::EventPage::MoveType::MoveType_stationary) {
    return MoveType::Fixed;
  }
  if (move_type == lcf::rpg::EventPage::MoveType::MoveType_toward) {
    return MoveType::Approach;
  }
  if (move_type == lcf::rpg::EventPage::MoveType::MoveType_random) {
    return MoveType::Random;
  }
  if (move_type == lcf::rpg::EventPage::MoveType::MoveType_custom) {
    return MoveType::Custom;
  }
  return MoveType::Custom; // Anything else isn't currently supported by MV. We'll need to handle these later // TODO
}
MovementFrequency LibLCF::getMoveFrequency(int32_t move_freq) {
  if (move_freq > 6) {
    log("Frequency: Higher than 6");
    return MovementFrequency::Highest;
  }
  return static_cast<MovementFrequency>(move_freq);
}
Direction LibLCF::getDirection(int32_t direction) {
  if (direction == lcf::rpg::EventPage::Direction::Direction_up) {
    return Direction::Up;
  }
  if (direction == lcf::rpg::EventPage::Direction::Direction_down) {
    return Direction::Down;
  }
  if (direction == lcf::rpg::EventPage::Direction::Direction_left) {
    return Direction::Left;
  }
  return Direction::Right;
}
lcf::rpg::MoveCommand LibLCF::DecodeMove(lcf::DBArray<int32_t>::const_iterator& it) {
  lcf::rpg::MoveCommand cmd;
  cmd.command_id = *it++;

  switch (cmd.command_id) {
  case 32: // Switch ON
  case 33: // Switch OFF
    cmd.parameter_a = DecodeInt(it);
    break;
  case 34: // Change Graphic
    cmd.parameter_string = lcf::DBString(DecodeString(it));
    cmd.parameter_a = DecodeInt(it);
    break;
  case 35: // Play Sound Effect
    cmd.parameter_string = lcf::DBString(DecodeString(it));
    cmd.parameter_a = DecodeInt(it);
    cmd.parameter_b = DecodeInt(it);
    cmd.parameter_c = DecodeInt(it);
    break;
  }

  return cmd;
}

int LibLCF::DecodeInt(lcf::DBArray<int32_t>::const_iterator& it) {
  int value = 0;

  for (;;) {
    int x = *it++;
    value <<= 7;
    value |= x & 0x7F;
    if (!(x & 0x80))
      break;
  }

  return value;
}

const std::string LibLCF::DecodeString(lcf::DBArray<int32_t>::const_iterator& it) {
  std::ostringstream out;
  int len = DecodeInt(it);

  for (int i = 0; i < len; i++)
    out << (char)*it++;

  std::string result = lcf::ReaderUtil::Recode(out.str(), "UTF-8");

  return result;
}