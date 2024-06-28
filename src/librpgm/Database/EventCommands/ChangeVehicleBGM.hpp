#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>
struct ChangeVehicleBGMCommand : IEventCommand {
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  VehicleType vehicle;
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Vehicle BGM : " + DecodeEnumName(vehicle) + ", " + (me.name == "" ? "None" : me.name) +
           " " + std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
  }
};

