#include "EventPage.hpp"

void to_json(nlohmann::json& json, const EventPage& eventPage) {

}

void from_json(const nlohmann::json& json, EventPage& eventPage) {
  json["conditions"].get_to(eventPage.conditions);
  json["directionFix"].get_to(eventPage.directionFix);
  json["image"].get_to(eventPage.image);
  CommandParser cmdParser;
  eventPage.list = cmdParser.parse(json["list"]);
  json["moveFrequency"].get_to(eventPage.moveFrequency);
  //json["moveRoute"].get_to(event.moveRoute);
  json["moveSpeed"].get_to(eventPage.moveSpeed);
  json["moveType"].get_to(eventPage.moveType);
  json["priorityType"].get_to(eventPage.priorityType);
  json["stepAnime"].get_to(eventPage.stepAnime);
  json["through"].get_to(eventPage.through);
  json["trigger"].get_to(eventPage.trigger);
  json["walkAnime"].get_to(eventPage.walkAnime);
}
