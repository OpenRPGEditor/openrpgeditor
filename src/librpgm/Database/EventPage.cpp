#include "EventPage.hpp"

void to_json(nlohmann::json& json, const EventPage& eventPage) {

}

void from_json(const nlohmann::json& json, EventPage& eventPage) {
  json["conditions"].get_to(eventPage.conditions);
  json["directionFix"].get_to(eventPage.directionFix);
  json["image"].get_to(eventPage.image);
  CommandParser cmdParser;
  eventPage.list = cmdParser.parse(json["list"]);
}
