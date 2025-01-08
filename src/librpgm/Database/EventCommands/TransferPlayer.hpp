#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TransferPlayerCommand final : IEventCommand {
  TransferPlayerCommand() = default;
  explicit TransferPlayerCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<TransferPlayerCommand>(*this); }

  TransferMode mode = TransferMode::Direct;
  int mapId = Database::instance() != nullptr && Database::instance()->mapInfos.currentMap() != nullptr ? Database::instance()->mapInfos.currentMap()->id() : 1;
  int x{0};
  int y{0};
  Direction direction = Direction::Retain;
  Fade fade = Fade::Black;
};
