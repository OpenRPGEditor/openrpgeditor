#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNameCommand final : IEventCommand {
  ChangeNameCommand() = default;
  explicit ChangeNameCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeNameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Name; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeNameCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return targetId == actor;
    }
    return false;
  };
  int actor = 1;
  std::string name;
};
