#include "EventPage.hpp"

void to_json(nlohmann::json& to, const EventCondition& cond) {
  to = {
      {"actorId", cond.actorId},
      {"actorValid", cond.actorValid},
      {"itemId", cond.itemId},
      {"itemValid", cond.itemValid},
      {"selfSwitchCh", cond.selfSwitchCh},
      {"selfSwitchValid", cond.selfSwitchValid},
      {"switch1Id", cond.switch1Id},
      {"switch1Valid", cond.switch1Valid},
      {"switch2Id", cond.switch2Id},
      {"switch2Valid", cond.switch2Valid},
      {"variableId", cond.variableId},
      {"variableValid", cond.variableValid},
      {"variableValue", cond.variableValue},
  };
}
void from_json(const nlohmann::json& from, EventCondition& cond) {
  cond.actorId = from.value("actorId", cond.actorId);
  cond.actorValid = from.value("actorValid", cond.actorValid);
  cond.itemId = from.value("itemId", cond.itemId);
  cond.itemValid = from.value("itemValid", cond.itemValid);
  cond.selfSwitchCh = from.value("selfSwitchCh", cond.selfSwitchCh);
  cond.selfSwitchValid = from.value("selfSwitchValid", cond.selfSwitchValid);
  cond.switch1Id = from.value("switch1Id", cond.switch1Id);
  cond.switch1Valid = from.value("switch1Valid", cond.switch1Valid);
  cond.switch2Id = from.value("switch2Id", cond.switch2Id);
  cond.switch2Valid = from.value("switch2Valid", cond.switch2Valid);
  cond.variableId = from.value("variableId", cond.variableId);
  cond.variableValid = from.value("variableValid", cond.variableValid);
  cond.variableValue = from.value("variableValue", cond.variableValue);
}

void to_json(nlohmann::json& to, const EventImage& image) {
  to = {
      {"tileId", image.tileId},   {"characterName", image.characterName},   {"direction", image.direction},
      {"pattern", image.pattern}, {"characterIndex", image.characterIndex},
  };
}
void from_json(const nlohmann::json& from, EventImage& image) {
  image.tileId = from.value("tileId", image.tileId);
  image.characterName = from.value("characterName", image.characterName);
  image.direction = from.value("direction", image.direction);
  image.pattern = from.value("pattern", image.pattern);
  image.characterIndex = from.value("characterIndex", image.characterIndex);
}

void to_json(nlohmann::json& json, const EventPage& eventPage) {
  json["conditions"] = eventPage.conditions;
  json["directionFix"] = eventPage.directionFix;
  json["image"] = eventPage.image;
  CommandParser::serialize(json["list"], eventPage.list);
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
  eventPage.conditions = json.value("conditions", eventPage.conditions);
  eventPage.directionFix = json.value("directionFix", eventPage.directionFix);
  eventPage.image = json.value("image", eventPage.image);
  CommandParser cmdParser;
  eventPage.list = cmdParser.parse(json["list"]);
  eventPage.moveFrequency = json.value("moveFrequency", eventPage.moveFrequency);
  eventPage.moveRoute = json.value("moveRoute", eventPage.moveRoute);
  eventPage.moveSpeed = json.value("moveSpeed", eventPage.moveSpeed);
  eventPage.moveType = json.value("moveType", eventPage.moveType);
  eventPage.priorityType = json.value("priorityType", eventPage.priorityType);
  eventPage.stepAnime = json.value("stepAnime", eventPage.stepAnime);
  eventPage.through = json.value("through", eventPage.through);
  eventPage.trigger = json.value("trigger", eventPage.trigger);
  eventPage.walkAnime = json.value("walkAnime", eventPage.walkAnime);
  eventPage.name = json.value("name", eventPage.name);
}
