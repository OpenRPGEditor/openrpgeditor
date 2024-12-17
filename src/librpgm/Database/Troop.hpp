#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

#include <nlohmann/json.hpp>

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
    int actorHp{50};
    int actorId{1};
    bool actorValid;
    int enemyIndex{0};
    int enemyHp{50};
    bool enemyValid;
    int switchId{1};
    bool switchValid;
    int turnA{0};
    int turnB{0};
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

  Member* member(int index) {
    if (index < 0 || index > m_members.size()) {
      return nullptr;
    }

    return &m_members[index];
  }

  const Member* member(int index) const {
    if (index < 0 || index > m_members.size()) {
      return nullptr;
    }

    return &m_members[index];
  }

  int id() const { return m_id; }
  void setId(const int id) { m_id = id; }

  const std::string& name() const { return m_name; }
  void setName(const std::string& name) { m_name = name; }

  int m_id;
  std::vector<Member> m_members;
  std::string m_name;
  std::vector<Page> m_pages;

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