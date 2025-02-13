#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintPictureCommand final : IEventCommand {
  TintPictureCommand() = default;
  explicit TintPictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~TintPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<TintPictureCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::PictureId) {
      return picture == targetId;
    }
    return false;
  };
  int picture{1};

  struct {
    int r{0};
    int g{0};
    int b{0};
    int gray{0};
  } color;

  int duration{60};
  bool waitForCompletion{true};
};