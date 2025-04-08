#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGM : IEventCommand {
  FadeoutBGM() = default;
  explicit FadeoutBGM(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~FadeoutBGM() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_BGM; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<FadeoutBGM>(*this); }

  int duration;
};
