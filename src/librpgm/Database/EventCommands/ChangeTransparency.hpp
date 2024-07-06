#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTransparencyCommand final : IEventCommand {
  ChangeTransparencyCommand() = default;
  explicit ChangeTransparencyCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeTransparencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Transparency; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ValueControl transparency;
};
