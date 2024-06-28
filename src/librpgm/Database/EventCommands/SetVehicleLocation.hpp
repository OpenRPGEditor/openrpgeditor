#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetVehicleLocationCommand : IEventCommand {
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  VehicleType vehicle;
  TransferMode mode;
  int mapId;
  int x;
  int y;

  [[nodiscard]] std::string stringRep() const override {
    std::string prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Vehicle Location : " + DecodeEnumName(vehicle) + ",";
    std::string suffix = ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return prefix + ", {[]} ({[]},{[]}) " + suffix;
    }
    return prefix + " {}" + std::format("({}, {})", x, y) + suffix;
  }
};