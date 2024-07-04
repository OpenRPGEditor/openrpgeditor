#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PluginCommandMV : IEventCommand {
  ~PluginCommandMV() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::PluginMV_Command; }
  std::string command;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};