#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEquipmentCommand final : IEventCommand {
  ChangeEquipmentCommand() = default;
  explicit ChangeEquipmentCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEquipmentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Equipment; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEquipmentCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return actorId == targetId;
    }
    if (type == SearchType::Equipment) {
      return equipment == targetId; // Might need some verification
    }
    return false;
  };
  int actorId{1};
  int equipType{0};
  int equipment{0};
};
