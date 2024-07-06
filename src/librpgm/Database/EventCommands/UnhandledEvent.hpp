#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct UnhandledEventCommand final : IEventCommand {
  ~UnhandledEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return m_code; }
  void serialize(nlohmann::json& out, const bool doIndent = true, const bool doParameters = true) const override;
  std::string stringRep(const Database& db) const override;
  EventCode m_code{};
  nlohmann::json data;
};

