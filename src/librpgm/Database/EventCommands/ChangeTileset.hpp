#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTilesetCommand final : IEventCommand {
  ChangeTilesetCommand() = default;
  explicit ChangeTilesetCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeTilesetCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Tileset; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeTilesetCommand>(*this); }

  int tileset;
};