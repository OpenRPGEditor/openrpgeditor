#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ErasePictureCommand final : IEventCommand {
  ErasePictureCommand() = default;
  explicit ErasePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ErasePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ErasePictureCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::PictureId) {
      return targetId == picture;
    }
    return false;
  };
  int picture;
};
