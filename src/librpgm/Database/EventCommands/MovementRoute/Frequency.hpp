#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementFrequencyCommand : IEventCommand {
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  int frequency;
  void serializeParameters(nlohmann::json& out) override {
    out.push_back(frequency);
  }
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Frequency " + std::to_string(frequency) + ColorFormatter::popColor();
  }
};
