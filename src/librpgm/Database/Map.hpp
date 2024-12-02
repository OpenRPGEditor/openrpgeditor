#pragma once

#include "Database/Audio.hpp"
#include "Database/Event.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/Globals.hpp"

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
    std::vector<int> regionSet{};
    int troopId{};
    int weight{};
  };

  static Map load(std::string_view filepath);
  bool serialize(std::string_view filepath) const;

  [[nodiscard]] std::vector<std::optional<Event>> getSorted() const {
    std::vector<std::optional<Event>> ret = events;
    std::ranges::sort(ret, [](const std::optional<Event>& a, const std::optional<Event>& b) {
      if (!a || !b) {
        return false;
      }
      return a->y < b->y;
    });
    return std::move(ret);
  }

  Event* event(int id) {

    if (const auto it = std::ranges::find_if(events, [&id](const auto& ev) { return ev && ev->id == id; }); it != events.end()) {
      return &it->value();
    }
    return nullptr;
  }

  Event* eventAt(int x, int y) {
    if (const auto it = std::ranges::find_if(events, [&x, &y](const auto& ev) { return ev && ev->x == x && ev->y == y; }); it != events.end()) {
      return &it->value();
    }

    return nullptr;
  }

  std::vector<Event*> eventsAt(const int x, const int y) {
    std::vector<Event*> ret;
    std::ranges::for_each(events, [&ret, &x, &y](auto& ev) {
      if (ev && ev->x == x && ev->y == y) {
        ret.push_back(&ev.value());
      }
    });
    return ret;
  }

  Event* createNewEvent() {
    if (events.empty()) {
      events.emplace_back();
    }
    const auto it = std::find_if(events.begin() + 1, events.end(), [](const auto& ev) { return !ev; });
    Event* ret;
    if (it != events.end()) {
      *it = Event();
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
  Event* createEventFromTemplate(const Event& ev) {
    // Inserts new template
    if (ev.id == events.size()) {
      events.emplace_back(ev);
    } else {
      events.insert(events.begin() + ev.id, ev);
    }
    // Resort and rename
    for (int i = ev.id + 1; i < events.size(); ++i) {
      events.at(i)->id = i + 1;
      if (events.at(i)->name.contains("EV")) {
        events.at(i)->name = std::format("EV{:03} ", i);
      }
    }
    return &(*events.at(ev.id));
  }

  void deleteEvent(int id) {
    if (const auto it = std::ranges::find_if(events, [&id](const auto& ev) { return ev && ev->id == id; }); it != events.end()) {
      it->reset();
      m_isDirty = true;
    }
  }

  int findOrMakeFreeId() const {
    if (events.empty()) {
      return 1;
    }

    for (int i = 0; i < events.size(); ++i) {
      if (!events[i]) {
        return i;
      }
    }

    return events.size();
  }

  void resize(int newWidth, int newHeight);

  bool autoplayBgm{};
  bool autoplayBgs{};
  std::string battleback1Name;
  std::string battleback2Name;
  Audio bgm;
  Audio bgs;
  bool disableDashing{};
  std::string displayName;
  std::vector<Encounter> encounterList;
  int encounterStep{};
  std::string note;
  bool parallaxLoopX{};
  bool parallaxLoopY{};
  std::string parallaxName;
  bool parallaxShow{};
  int parallaxSx{};
  int parallaxSy{};
  ScrollType scrollType{};
  bool specifyBattleback{};
  int tilesetId{};
  int width{};
  int height{};

  std::vector<std::optional<int>> data;
  std::vector<std::optional<Event>> events;

  bool m_isDirty = false;
  bool m_isValid{false};

  bool isDirty() {
    m_isDirty |= std::ranges::any_of(events, [](const auto& e) { return e && e->isDirty(); });

    return m_isDirty;
  }
};

void to_json(nlohmann::ordered_json& json, const Map& map);
void from_json(const nlohmann::ordered_json& json, Map& map);
void to_json(nlohmann::ordered_json& json, const Map::Encounter& encounter);
void from_json(const nlohmann::ordered_json& json, Map::Encounter& encounter);