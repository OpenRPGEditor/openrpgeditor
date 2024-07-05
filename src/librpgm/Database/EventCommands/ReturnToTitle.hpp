#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ReturnToTitleCommand final : IEventCommand {
  ReturnToTitleCommand() = default;
  explicit ReturnToTitleCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~ReturnToTitleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Return_To_Title_Screen; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};