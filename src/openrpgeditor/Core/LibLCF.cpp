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
std::shared_ptr<IEventCommand> LibLCF::createCommand(int32_t code, int32_t indent) {
  switch (code) {
  case lcf::rpg::EventCommand::Code::END:
    EndCommand newCmd = EndCommand();
    newCmd.setIndent(indent);
    return std::make_shared<EndCommand>(newCmd);
  default:
    break;
  }
}
void LibLCF::convertEvent(Event* event, const lcf::rpg::Event& ev) {
  // event->setName(ev.name.data()); Name field, not entirely needed
  m_lcfLogger.clear();
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
  m_lcfLogger.clear();
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

  // Data structures
  convertCommands(&page->list(), evPage.event_commands);
  // page->setCondition(EventCondition()); // TODO
  // page->setMoveRoute() // TODO: Need to parse this
  // page->setList() // TODO: Need to parse this

  // page->setName() // RPG2000 doesn't have page names; ORE does
}
void LibLCF::convertCommands(std::vector<std::shared_ptr<IEventCommand>>* r_cmds, const std::vector<lcf::rpg::EventCommand>& s_cmds) {
  for (auto& cmd : s_cmds) {
    const int* test = cmd.parameters.data();
    // const std::string teststring = ToString(cmd.string);
    r_cmds->insert(r_cmds->end() - 1, createCommand(cmd.code, cmd.indent));

    // CommentCommand cmd = CommentCommand();
    // cmd.text = str;
    // cmd.setIndent(0);
    // page->list().insert(page->list().end() - 1, std::make_shared<CommentCommand>(cmd));
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
    return MovementFrequency::Highest;
    log("Frequency: Higher than 6");
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