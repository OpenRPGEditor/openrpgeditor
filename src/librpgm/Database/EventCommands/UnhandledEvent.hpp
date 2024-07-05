#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct UnhandledEventCommand final : IEventCommand {
  ~UnhandledEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return m_code; }

  EventCode m_code{};
  nlohmann::json data;
};

