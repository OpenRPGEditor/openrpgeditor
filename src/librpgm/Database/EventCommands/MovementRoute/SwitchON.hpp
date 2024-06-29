#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementSwitchONCommand : IEventCommand {
  ~MovementSwitchONCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_ON; }
  int id;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(id);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Switch " + std::format("{:04}", id) + " ON" + ColorFormatter::popColor();
  }
};
