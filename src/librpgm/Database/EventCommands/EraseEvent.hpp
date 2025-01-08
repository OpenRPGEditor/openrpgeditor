#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EraseEventCommand final : IEventCommand {
  EraseEventCommand() = default;
  explicit EraseEventCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<EraseEventCommand>(*this); }
};
