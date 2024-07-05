#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGM : IEventCommand {
  FadeoutBGM() = default;
  explicit FadeoutBGM(const std::optional<int>& indent, nlohmann::json& parameters);
  ~FadeoutBGM() override = default;
  int duration;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_BGM; }
  std::string stringRep(const Database& db) const override;
};
