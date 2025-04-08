#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeDefeatMECommand final : IEventCommand {
  ChangeDefeatMECommand() = default;
  explicit ChangeDefeatMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeDefeatMECommand>(*this); }
  bool hasStringReference(const std::string& text, SearchType type) override {
    if (type == SearchType::Audio) {
      return text.contains(me.name());
    }
    return false;
  };
  Audio me;
};