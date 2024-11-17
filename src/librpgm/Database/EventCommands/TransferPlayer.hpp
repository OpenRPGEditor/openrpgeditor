#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Database.hpp"
#include <format>

struct TransferPlayerCommand final : IEventCommand {
  TransferPlayerCommand() = default;
  explicit TransferPlayerCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  TransferMode mode = TransferMode::Direct;
  int mapId = Database::instance() != nullptr && Database::instance()->mapInfos.currentMap() != nullptr
                  ? Database::instance()->mapInfos.currentMap()->id
                  : 1;
  int x{0};
  int y{0};
  Direction direction = Direction::Retain;
  Fade fade = Fade::Black;
};
