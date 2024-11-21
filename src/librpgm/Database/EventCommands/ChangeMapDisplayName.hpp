#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeMapNameDisplayCommand final : IEventCommand {
  ChangeMapNameDisplayCommand() = default;
  explicit ChangeMapNameDisplayCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeMapNameDisplayCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Map_Name_Display; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ValueControl checkIfOn;
};