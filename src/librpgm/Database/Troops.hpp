#pragma once

#include "Database/EventPage.hpp"

#include "nlohmann/json.hpp"

struct Troop {
  friend void to_json(nlohmann::ordered_json& j, const Troop& t);
  friend void from_json(const nlohmann::ordered_json& j, Troop& t);

  struct Member {
    friend void to_json(nlohmann::ordered_json& j, const Member& m);
    friend void from_json(const nlohmann::ordered_json& j, Member& m);
    int enemyId = 0;
    int x = 0;
    int y = 0;
    bool hidden = false;
  };

  struct Conditions {
    friend void to_json(nlohmann::ordered_json& j, const Conditions& c);
    friend void from_json(const nlohmann::ordered_json& j, Conditions& c);
    int actorHp;
    int actorId;
    bool actorValid;
    int enemyIndex;
    int enemyHp;
    bool enemyValid;
    int switchId;
    bool switchValid;
    int turnA;
    int turnB;
    bool turnEnding;
    bool turnValid;
  };
  struct Page {
    friend void to_json(nlohmann::ordered_json& j, const Page& p);
    friend void from_json(const nlohmann::ordered_json& j, Page& p);

    Conditions conditions;
    std::vector<std::shared_ptr<IEventCommand>> list;
    int span;
  };
  int id;
  std::vector<Member> members;
  std::string name;
  std::vector<Page> pages;

  Member* member(int index) {
    if (index < 0 || index > members.size()) {
      return nullptr;
    }

    return &members[index];
  }

  const Member* member(int index) const {
    if (index < 0 || index > members.size()) {
      return nullptr;
    }

    return &members[index];
  }

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::ordered_json& j, const Troop::Conditions& c);
void from_json(const nlohmann::ordered_json& j, Troop::Conditions& c);
void to_json(nlohmann::ordered_json& j, const Troop::Page& p);
void from_json(nlohmann::ordered_json& j, Troop::Page& p);
void to_json(nlohmann::ordered_json& j, const Troop::Member& m);
void from_json(const nlohmann::ordered_json& j, Troop::Member& m);
void to_json(nlohmann::ordered_json& j, const Troop& t);
void from_json(const nlohmann::ordered_json& j, Troop& t);

class Troops {
public:
  static Troops load(std::string_view filepath);

  bool serialize(std::string_view filename);

  [[nodiscard]] Troop* troop(int id) {
    for (auto& troop : m_troops) {
      if (troop.id == id && troop.m_isValid) {
        return &troop;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Troop* troop(int id) const {
    for (const auto& troop : m_troops) {
      if (troop.id == id && troop.m_isValid) {
        return &troop;
      }
    }

    return nullptr;
  }
  std::vector<Troop>& troops() { return m_troops; }
  const std::vector<Troop>& troops() const { return m_troops; }
  int count() { return m_troops.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    int oldSize = m_troops.size();
    m_troops.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_troops.size(); ++i) {
        m_troops[i].id = i;
      }
    }
  }

private:
  std::vector<Troop> m_troops;
};
