#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetOnOffVehicleCommand : IEventCommand {
  ~GetOnOffVehicleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_On_Off_Vehicle; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Get on/off Vehicle" + ColorFormatter::popColor();
  }
};
