#pragma once

#include "Database/Globals.hpp"
#include "Database/Audio.hpp"
#include "Database/Event.hpp"
#include "Database/EventCommands/EventDummy.hpp"

#include <format>
#include <string>

#include "nlohmann/json.hpp"

class Map {
public:
  friend void to_json(nlohmann::ordered_json& json, const Map& map);
  friend void from_json(const nlohmann::ordered_json& json, Map& map);
  struct Encounter {
    friend void to_json(nlohmann::ordered_json& json, const Encounter& encounter);
    friend void from_json(const nlohmann::ordered_json& json, Encounter& encounter);
    std::array<int, 3> regionSet{};
    int troopId{};
    int weight{};
  };

  static Map load(std::string_view filepath);
  bool serialize(std::string_view filepath) const;

  [[nodiscard]] std::vector<std::optional<Event>> getSorted() const {
    std::vector<std::optional<Event>> ret = events;
    std::sort(ret.begin(), ret.end(), [](const std::optional<Event>& a, const std::optional<Event>& b) {
      if (!a || !b) {
        return false;
      }
      return a->y < b->y;
    });
    return std::move(ret);
  }

  Event* event(int id) {
    auto it = std::find_if(events.begin(), events.end(), [&id](const auto& ev) { return ev && ev->id == id; });

    if (it != events.end()) {
      return &it->value();
    }
    return nullptr;
  }

  Event* eventAt(int x, int y) {
    auto it =
        std::find_if(events.begin(), events.end(), [&x, &y](const auto& ev) { return ev && ev->x == x && ev->y == y; });
    if (it != events.end()) {
      return &it->value();
    }

    return nullptr;
  }

  Event* createNewEvent() {
    if (events.empty()) {
      events.emplace_back();
    }
    auto it = std::find_if(events.begin() + 1, events.end(), [](const auto& ev) { return !ev; });
    Event* ret;
    if (it != events.end()) {
      (*it) = Event();
      (*it)->id = it - events.begin();
      ret = &it->value();
    } else {
      ret = &events.emplace_back(Event()).value();
      ret->id = events.size() - 1;
    }
    ret->name = std::format("EV{:03}", ret->id);
    ret->pages.emplace_back();
    ret->pages.back().list.emplace_back(new EventDummy());
    ret->pages.back().list.back()->indent = 0;
    m_isDirty = true;
    return ret;
  }

  void deleteEvent(int id) {
    auto it = std::find_if(events.begin(), events.end(), [&id](const auto& ev) { return ev && ev->id == id; });
    if (it != events.end()) {
      (*it).reset();
      m_isDirty = true;
    }
  }

  bool autoPlayBgm{};
  bool autoPlayBgs{};
  std::string battleBack1Name;
  std::string battleBack2Name;
  Audio bgm;
  Audio bgs;
  bool disableDashing{};
  std::string displayName;
  std::vector<Encounter> encounters;
  int encounterStep{};
  std::string note;
  bool parallaxLoopX{};
  bool parallaxLoopY{};
  std::string parallaxName;
  bool parallaxShow{};
  int parallaxSx{};
  int parallaxSy{};
  ScrollType scrollType{};
  bool specifyBattleBack{};
  int tilesetId{};
  int width{};
  int height{};

  std::vector<std::optional<int>> data;
  std::vector<std::optional<Event>> events;

  bool m_isDirty = false;
  bool m_isValid{false};

  bool isDirty() {
    m_isDirty |= std::any_of(events.begin(), events.end(), [](const auto& e) { return e && e->isDirty(); });

    return m_isDirty;
  }
};

void to_json(nlohmann::ordered_json& json, const Map& map);
void from_json(const nlohmann::ordered_json& json, Map& map);
void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter);
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter);