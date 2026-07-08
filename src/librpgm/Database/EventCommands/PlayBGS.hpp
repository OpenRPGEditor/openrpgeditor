#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayBGSCommand final : IEventCommand {
  PlayBGSCommand() = default;
  explicit PlayBGSCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayBGSCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  [[nodiscard]] std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlayBGSCommand>(*this); }

  bool hasStringReference(const std::string& text, SearchType type) override {
    if (type == SearchType::Audio) {
      return text.contains(bgs.name());
    }
    return false;
  };
  Audio bgs;

  [[nodiscard]] std::vector<std::string> stringValues() const override { return {bgs.name()}; }
  [[nodiscard]] std::vector<std::string> stringValueNames() const override { return {"bgs.name"}; }
  [[nodiscard]] bool hasStringValues() const override { return true; }
};
