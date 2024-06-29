#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementChangeOpacityCommand : IEventCommand {
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }

  int opacity;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(opacity);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Opacity : " + std::to_string(opacity) + ColorFormatter::popColor();
  }
};
