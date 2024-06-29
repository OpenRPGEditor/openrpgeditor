#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementChangeBlendModeCommand : IEventCommand {
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }

  Blend mode;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(mode);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Blend Mode : " + DecodeEnumName(mode) + ColorFormatter::popColor();
  }
};
