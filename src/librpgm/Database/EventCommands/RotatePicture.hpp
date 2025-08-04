#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand final : IEventCommand {
  RotatePictureCommand() = default;
  explicit RotatePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<RotatePictureCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::PictureId) {
      return picture == targetId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      picture = newId;
      return true;
    }
    return false;
  }
  int picture = 1;
  int rotation = 0;
};
