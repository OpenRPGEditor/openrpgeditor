#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct UnhandledEventCommand final : IEventCommand {
  ~UnhandledEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return m_code; }
  void serialize(nlohmann::ordered_json& out, const bool doIndent = true, const bool doParameters = true, bool oldValues = false) const override;
  std::string stringRep(const Database& db) const override;
  EventCode m_code{};
  nlohmann::ordered_json data;
};
