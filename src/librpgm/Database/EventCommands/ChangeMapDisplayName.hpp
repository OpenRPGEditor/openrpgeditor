#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeMapNameDisplayCommand final : IEventCommand {
  ~ChangeMapNameDisplayCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Map_Name_Display; }
  ValueControl checkIfOn;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};