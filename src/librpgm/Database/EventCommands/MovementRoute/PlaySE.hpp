#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementPlaySECommand final : IMovementRouteStep {
  MovementPlaySECommand() = default;
  explicit MovementPlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementPlaySECommand>(*this); }
  bool hasStringReference(const std::string& text, SearchType type) override {
    if (type == SearchType::Audio) {
      return text.contains(se.name());
    }
    return false;
  };
  Audio se;
};
