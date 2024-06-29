#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct WaitCommand : IEventCommand {
  int duration;
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(duration);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Wait : " + std::to_string(duration) + " frames";
  }
};
