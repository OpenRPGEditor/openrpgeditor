#include "Database/EventParser.hpp"
#include "Database/Event.hpp"

#include <iostream>

using json = nlohmann::ordered_json;

Event EventParser::parse(const json& _json) {
  Event ret;
  parser = _json;
  from_json(parser, ret);
  return ret;
}
EventPage EventParser::parsePage(const json& _json) {
  EventPage ret;
  parser = _json;
  from_json(parser, ret);
  return ret;
}

void EventParser::serialize(nlohmann::ordered_json& data, Event& ev) { to_json(data, ev); }
