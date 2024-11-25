#pragma once
#include "Database/Event.hpp"

struct EventParser {
  nlohmann::json parser;
  int index = 0;

  Event parse(const nlohmann::ordered_json& data);
  static void serialize(nlohmann::ordered_json& data, Event& ev);
};