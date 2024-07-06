#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetEventLocationCommand final : IEventCommand {
  SetEventLocationCommand() = default;
  explicit SetEventLocationCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~SetEventLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Event_Location; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  TransferMode mode;
  int event;
  int x;
  int y;
  Direction direction;
};
