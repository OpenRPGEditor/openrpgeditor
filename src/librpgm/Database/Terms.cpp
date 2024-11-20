#include "Database/Terms.hpp"

void to_json(nlohmann::json& j, const Terms& terms) {
  j = {
      {"basic", terms.basic},
      {"commands", terms.commands},
      {"messages", terms.messages},
      {"params", terms.params},
  };
}

void from_json(const nlohmann::json& j, Terms& terms) {
  terms.basic = j.value("basic", terms.basic);
  terms.commands = j.value("commands", terms.commands);
  terms.messages = j.value("messages", terms.messages);
  terms.params = j.value("params", terms.params);
}
