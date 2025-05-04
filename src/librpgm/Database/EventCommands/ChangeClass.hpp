#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeClassCommand final : IEventCommand {
  ChangeClassCommand() = default;
  explicit ChangeClassCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeClassCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return targetId == actor;
    }
    if (type == SearchType::Class) {
      return targetId == classId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Actor) {
        actor = newId;
      } else if (type == SearchType::Class) {
        classId = newId;
      }
      return true;
    }
    return false;
  }
  int actor{1};
  int classId{1};
  bool saveLevel{false};
};