#include "Core/LibLCF.hpp"
#include "ImGuiExt/ImGuiUtils.hpp"
#include "MainWindow.hpp"
#include "Settings.hpp"
#include "imgui.h"
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
      if (ImGui::Button("Create Placeholder Events")) {
        // LibLCF => Map
        if (m_parent->currentMap() != nullptr) {
          for (auto& ev : map->events) {
            convertEvent(m_parent->currentMap()->createNewEvent(), ev);
          }
          // m_parent->currentMapInfo()->map()->setEvents(events);
          //  m_parent->mapEditor()->setMap(m_parent->currentMapInfo());
        }
      }
      ImGui::EndPopup();
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
  ImGui::End();
}
std::shared_ptr<IEventCommand> LibLCF::createCommand(int32_t code, int32_t indent, lcf::DBArray<int32_t> data, const std::string& strData) {

  auto codeEnum = static_cast<lcf::rpg::EventCommand::Code>(code);

  int32_t* dataPtr = data.data();

  std::vector<int> parameters;
  for (int i = 0; i < data.size(); ++i) {
    parameters.push_back(dataPtr[i]);
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
    // TODO: How does this even work with _2, probably NextComment?
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

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::CallCommonEvent)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ForceFlee)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnableCombo)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeClass)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeBattleCommands)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenLoadMenu)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ExitGame)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ToggleAtbMode)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ToggleFullscreen)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenVideoOptions)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::InputNumber)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TimerOperation)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangePartyMembers)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeLevel)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeParameters)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSkills)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEquipment)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHP)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSP)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeCondition)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::FullHeal)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::SimulatedAttack)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHeroName)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeHeroTitle)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSpriteAssociation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeActorFace)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeVehicleGraphic)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemBGM)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemSFX)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeSystemGraphics)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeScreenTransitions)) { // TODO
  }

  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnemyEncounter)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::OpenShop)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowInn)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnterHeroName)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Teleport)) {
    attachToCommand("MapID: " + std::to_string(parameters.at(0)));
    attachToCommand("X: " + std::to_string(parameters.at(1)));
    attachToCommand("Y: " + std::to_string(parameters.at(2)));
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MemorizeLocation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::RecallToLocation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EnterExitVehicle)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::SetVehicleLocation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEventLocation)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TradeEventLocations)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::StoreTerrainID)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::StoreEventID)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::WeatherEffects)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::MoveEvent)) { // TODO - Movement Route
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ProceedWithMovement)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::HaltAllMovement)) { // TODO
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::PlayMovie)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::KeyInputProc)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMapTileset)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangePBG)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEncounterSteps)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TileSubstitution)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TeleportTargets)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeTeleportAccess)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EscapeTarget)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeEscapeAccess)) { // TODO
  }
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

      newCmd.globalSwitch.switchIdx = parameters.at(1); // TODO: Needs mapping
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
    newCmd.label = strData;
    return std::make_shared<LabelCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::JumpToLabel)) {
    JumpToLabelCommand newCmd;
    newCmd.setIndent(indent);
    newCmd.label = strData;
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterHP)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterMP)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeMonsterCondition)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowHiddenMonster)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ChangeBattleBG)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ShowBattleAnimation_B)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ConditionalBranch_B)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::TerminateBattle)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::VictoryHandler)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EscapeHandler)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::DefeatHandler)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndBattle)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Transaction)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::NoTransaction)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndShop)) {
    EndCommand newCmd;
    newCmd.setIndent(indent);
    m_attachDummy = true;
    return std::make_shared<EndCommand>(newCmd);
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::Stay)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::NoStay)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndInn)) { // TODO
  }
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
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::ElseBranch_B)) { // TODO
  }
  if (code == static_cast<int>(lcf::rpg::EventCommand::Code::EndBranch_B)) {
    // TODO
  }
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
  m_commandLogger.push_back("\u272A " + text);
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
  convertCommands(&page->list(), evPage.event_commands);
  // page->setCondition(EventCondition()); // TODO
  // page->setMoveRoute() // TODO: Need to parse this
  // page->setList() // TODO: Need to parse this

  // page->setName() // RPG2000 doesn't have page names; ORE does

  // These variables are reset per page. The scope doesn't exist outside of it.
  m_windowTransparent = false;
  m_windowPosition = 2;
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