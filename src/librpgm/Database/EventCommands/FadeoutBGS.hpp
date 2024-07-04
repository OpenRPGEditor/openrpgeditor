#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGS : IEventCommand {
  ~FadeoutBGS() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_BGS; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
