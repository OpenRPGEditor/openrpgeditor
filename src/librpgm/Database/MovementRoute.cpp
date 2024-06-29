#include "MovementRoute.hpp"
#include "Database/CommandParser.hpp"

void to_json(nlohmann::json& json, const MovementRoute& route) {
  CommandParser::serialize(json["list"], route.list, true);
  json["repeat"] = route.repeat;
  json["skippable"] = route.skippable;
  json["wait"] = route.wait;
}

void from_json(const nlohmann::json& json, MovementRoute& route) {
  CommandParser parser;
  route.list = parser.parse(json["list"]);
  json["repeat"].get_to(route.repeat);
  json["skippable"].get_to(route.skippable);
  json["wait"].get_to(route.wait);
}