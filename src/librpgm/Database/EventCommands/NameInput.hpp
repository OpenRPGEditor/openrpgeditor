#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NameInputCommand final : IEventCommand {
  NameInputCommand() = default;
  explicit NameInputCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actorId{};
  int maxChar{8};
};
