#include "Database/Event.hpp"

void to_json(nlohmann::ordered_json& to, const Event& event) {
  to = {
      {"id", event.id},       {"name", event.name}, {"note", event.note},
      {"pages", event.pages}, {"x", event.x},       {"y", event.y},
  };
}
void from_json(const nlohmann::ordered_json& from, Event& event) {
  event.id = from.value("id", event.id);
  event.name = from.value("name", event.name);
  event.note = from.value("note", event.note);
  event.pages = from.value("pages", event.pages);
  event.x = from.value("x", event.x);
  event.y = from.value("y", event.y);
}