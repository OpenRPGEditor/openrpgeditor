#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PluginCommandMV : IEventCommand {
  PluginCommandMV() = default;
  explicit PluginCommandMV(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PluginCommandMV() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Plugin_Command_del_MV; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PluginCommandMV>(*this); }

  std::string command;
};