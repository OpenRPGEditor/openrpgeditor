#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct SelectItemCommand : IEventCommand {
  ~SelectItemCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Select_Item; }
  int item;
  ItemType type;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};