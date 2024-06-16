#pragma once

#include "Database/EventPage.hpp"

#include "nlohmann/json.hpp"

struct Troop {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Troop, id, members, name);

  struct Member {
    int enemyId = 0;
    int x = 0;
    int y = 0;
    bool hidden = false;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Member, enemyId, x, y, hidden);
  };
  int id;
  std::string name;
  std::vector<Member> members;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};

class Troops {
public:
  static Troops load(std::string_view filepath);

  bool serialize(std::string_view filename);

  [[nodiscard]] Troop* troop(int id) {
    for (auto& item : m_troops) {
      if (item.id == id) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Troop* troop(int id) const {
    for (const auto& set : m_troops) {
      if (set.id == id) {
        return &set;
      }
    }

    return nullptr;
  }

private:
  std::vector<Troop> m_troops;
};
