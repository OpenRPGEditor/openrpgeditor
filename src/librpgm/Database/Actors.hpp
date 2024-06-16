#pragma once

#include "Database/Trait.hpp"

#include <string>
#include <array>
#include <vector>
#include <string_view>

#include "nlohmann/json.hpp"

class Actor {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Actor, id, battlerName, characterIndex, characterName, classId, equips,
                                              faceIndex, faceName, traits, initialLevel, maxLevel, name, nickname, note,
                                              profile);
  int id = 0;
  std::string battlerName;
  int characterIndex = 0;
  std::string characterName;
  int classId = 1;
  std::vector<int> equips;
  int faceIndex = 0;
  std::string faceName;
  std::vector<Trait> traits;
  int initialLevel = 1;
  int maxLevel = 99;
  std::string name;
  std::string nickname;
  std::string note;
  std::string profile;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   *
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

class Actors {
public:
  static Actors load(std::string_view filename);
  bool serialize(std::string_view filename);

  [[nodiscard]] Actor* actor(int id) {
    for (auto& actor : m_actors) {
      if (actor.id == id && actor.m_isValid) {
        return &actor;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Actor* actor(int id) const {
    for (const auto& actor : m_actors) {
      if (actor.id == id && actor.m_isValid) {
        return &actor;
      }
    }

    return nullptr;
  }

  void resize(int newSize) {
    if (hasTestActor()) {
      ++newSize;
    }
    int oldSize = m_actors.size();
    m_actors.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_actors.size(); ++i) {
        m_actors[i].id = i;
      }
    }
  }

  bool hasTestActor() const {
    return std::find_if(m_actors.begin(), m_actors.end(), [](const auto& act) { return act.id == 0; }) !=
           m_actors.end();
  }
  int count() const { return m_actors.size() - 1; }

private:
  friend class DBActorsTab;
  std::vector<Actor> m_actors;
};
