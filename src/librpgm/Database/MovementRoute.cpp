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
  route.repeat = json.value("repeat", route.repeat);
  route.skippable = json.value("skippable", route.skippable);
  route.wait = json.value("wait", route.wait);
}