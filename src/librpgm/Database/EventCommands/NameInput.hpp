#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NameInputCommand final : IEventCommand {
  NameInputCommand() = default;
  explicit NameInputCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actorId{1};
  int maxChar{8};
};
