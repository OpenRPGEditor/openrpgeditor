#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGS : IEventCommand {
  FadeoutBGS() = default;
  explicit FadeoutBGS(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~FadeoutBGS() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_BGS; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  int duration{10};
};
