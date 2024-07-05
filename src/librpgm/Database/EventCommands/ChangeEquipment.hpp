#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEquipmentCommand final : IEventCommand {
  ~ChangeEquipmentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Equipment; }

  int actorId;
  int equipType;
  int equipment;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
