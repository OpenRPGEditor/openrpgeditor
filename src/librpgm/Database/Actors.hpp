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
  std::string name{"NewActor"};
  std::string nickname;
  std::string note;
  std::string profile;

  void draw();

  void setSelected(bool selected) { m_selected = selected; }
  bool selected() const { return m_selected; }

private:
  bool m_selected = false;
};

class Actors {
public:
  Actors();

  static Actors load(std::string_view filename);
  bool serialize(std::string_view filename);

  [[nodiscard]] Actor* actor(int id) {
    for (auto& item : m_actors) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Actor* actor(int id) const {
    for (const auto& set : m_actors) {
      if (set.id == id) {
        return &set;
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
  int count() const { return hasTestActor() ? m_actors.size() - 1 : m_actors.size(); }

private:
  friend class DBActorsTab;
  std::vector<Actor> m_actors;
  std::optional<Actor> m_selectedActor;
  bool m_isOpen;
};
