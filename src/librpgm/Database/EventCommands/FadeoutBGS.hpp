#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutBGS : IEventCommand {
  FadeoutBGS() = default;
  explicit FadeoutBGS(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~FadeoutBGS() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_BGS; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<FadeoutBGS>(*this); }

  int duration{10};
};
