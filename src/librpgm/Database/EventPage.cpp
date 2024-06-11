#include "EventPage.hpp"

void to_json(nlohmann::json& json, const EventPage& eventPage) {
  json["conditions"] = eventPage.conditions;
  json["directionFix"] = eventPage.directionFix;
  json["image"] = eventPage.image;
  CommandParser cmdParser;
  cmdParser.serialize(json["list"], eventPage.list);
  json["moveFrequency"] = eventPage.moveFrequency;
  json["moveRoute"] = eventPage.moveRoute;
  json["moveSpeed"] = eventPage.moveSpeed;
  json["moveType"] = eventPage.moveType;
  json["priorityType"] = eventPage.priorityType;
  json["stepAnime"] = eventPage.stepAnime;
  json["through"] = eventPage.through;
  json["trigger"] = eventPage.trigger;
  json["walkAnime"] = eventPage.walkAnime;
  if (!eventPage.name.empty()) {
    json["name"] = eventPage.name;
  }
}

void from_json(const nlohmann::json& json, EventPage& eventPage) {
  json["conditions"].get_to(eventPage.conditions);
  json["directionFix"].get_to(eventPage.directionFix);
  json["image"].get_to(eventPage.image);
  CommandParser cmdParser;
  eventPage.list = cmdParser.parse(json["list"]);
  json["moveFrequency"].get_to(eventPage.moveFrequency);
  json["moveRoute"].get_to(eventPage.moveRoute);
  json["moveSpeed"].get_to(eventPage.moveSpeed);
  json["moveType"].get_to(eventPage.moveType);
  json["priorityType"].get_to(eventPage.priorityType);
  json["stepAnime"].get_to(eventPage.stepAnime);
  json["through"].get_to(eventPage.through);
  json["trigger"].get_to(eventPage.trigger);
  json["walkAnime"].get_to(eventPage.walkAnime);
  if (json.count("name") != 0) {
    json["name"].get_to(eventPage.name);
  }
}
