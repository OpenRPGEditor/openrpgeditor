#pragma once

#include "Database/EventPage.hpp"

#include "nlohmann/json.hpp"

class Troops {

  struct Member {
    int enemyId = 0;
    int x = 0;
    int y = 0;
    bool hidden = false;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Member, enemyId, x, y, hidden);
  };
  int id;
  std::vector<Member> members;
};
