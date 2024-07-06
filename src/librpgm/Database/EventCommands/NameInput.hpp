#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NameInputCommand final : IEventCommand {
  NameInputCommand() = default;
  explicit NameInputCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actorId{};
  int maxChar{8};
};
