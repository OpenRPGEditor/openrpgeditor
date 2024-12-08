#pragma once

#include "Database/Actor.hpp"
#include "Database/IModifiable.hpp"

#include <vector>

#include "nlohmann/json.hpp"

class Actors final : public IModifiable {
public:
  [[nodiscard]] Actor* actor(const int id) {
    for (auto& actor : m_actors) {
      if (actor.id() == id && actor.isValid()) {
        return &actor;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Actor* actor(const int id) const {
    for (const auto& actor : m_actors) {
      if (actor.id() == id && actor.isValid()) {
        return &actor;
      }
    }

    return nullptr;
  }

  void resize(int newSize) {
    if (hasTestActor()) {
      ++newSize;
    }
    const int oldSize = m_actors.size();
    m_actors.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_actors.size(); ++i) {
        m_actors[i].setId(i);
      }
    }
    onModified().fire(this);
  }

  bool hasTestActor() const {
    return std::ranges::find_if(m_actors, [](const auto& act) { return act.id() == 0; }) != m_actors.end();
  }
  int count() const { return m_actors.size() - 1; }

  std::vector<Actor>& actorList() { return m_actors; }
  const std::vector<Actor>& actorList() const { return m_actors; }

private:
  friend class DBActorsTab;
  std::vector<Actor> m_actors;
};
