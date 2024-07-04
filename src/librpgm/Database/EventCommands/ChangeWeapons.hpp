#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeWeaponsCommmand : IEventCommand {
  ~ChangeWeaponsCommmand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Weapons; }
  int item;
  QuantityChangeOp operation;
  QuantityChangeSource operandSource;
  int operand;
  bool includeEquipment;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
