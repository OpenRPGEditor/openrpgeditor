#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeActorImageCommand final : IEventCommand {
  ChangeActorImageCommand() = default;
  explicit ChangeActorImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeActorImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Actor_Images; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeActorImageCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return actor == targetId;
    }
    return false;
  };
  int actor{1};
  std::string charPicture;
  int charIndex;
  std::string facePicture;
  int faceIndex;
  std::string battlerPicture;
};
