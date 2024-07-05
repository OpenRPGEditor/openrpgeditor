#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTransparencyCommand final : IEventCommand {
  ChangeTransparencyCommand() = default;
  explicit ChangeTransparencyCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(transparency);
  }
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  ValueControl transparency;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
