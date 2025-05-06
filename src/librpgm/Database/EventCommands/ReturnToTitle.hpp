#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ReturnToTitleCommand final : IEventCommand {
  ReturnToTitleCommand() = default;
  explicit ReturnToTitleCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~ReturnToTitleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Return_To_Title_Screen; }
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ReturnToTitleCommand>(*this); }
};