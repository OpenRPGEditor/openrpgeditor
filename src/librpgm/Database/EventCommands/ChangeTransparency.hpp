#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTransparencyCommand : IEventCommand {
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  ValueControl transparency;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
