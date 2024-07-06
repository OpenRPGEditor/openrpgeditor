#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PluginCommandMV : IEventCommand {
  PluginCommandMV() = default;
  explicit PluginCommandMV(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~PluginCommandMV() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::PluginMV_Command; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::string command;

};