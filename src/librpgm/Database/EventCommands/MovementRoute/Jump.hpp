#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementJumpCommand : IEventCommand {
  ~MovementJumpCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump; }
  int x;
  int y;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(x);
    out.push_back(y);
  }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + std::format("◇Jump {}, {}", x, y) + ColorFormatter::popColor();
  }
};
