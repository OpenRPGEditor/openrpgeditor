#pragma once

#include "Database/EventPage.hpp"

class Troops {

  struct Member {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Member, enemyId, x, y, hidden);
    int enemyId = 0;
    int x = 0;
    int y = 0;
    bool hidden = false;
  };
  int id;
  std::vector<Member> members;
};
