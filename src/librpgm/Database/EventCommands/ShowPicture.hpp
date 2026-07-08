#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowPictureCommand final : IEventCommand {
  ShowPictureCommand() = default;
  explicit ShowPictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShowPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  [[nodiscard]] std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowPictureCommand>(*this); }
  bool hasReference(const int targetId, const SearchType st) override {
    if (st == SearchType::PictureId) {
      return number == targetId;
    }
    return false;
  };
  bool setReference(const int targetId, const int newId, const SearchType st) override {
    if (hasReference(targetId, st)) {
      number = newId;
      return true;
    }
    return false;
  }
  int number{1};
  std::string imageName;
  PictureOrigin origin = PictureOrigin::Upper_Left;
  PictureDesignationSource type = PictureDesignationSource::Direct_designation;
  int value1{1}; // direct X value or indirect from global variables
  int value2{1}; // direct Y value or indirect from global variables
  int zoomX{100};
  int zoomY{100};
  int opacityValue{255};
  Blend blendMode = Blend::Normal;

  [[nodiscard]] std::vector<std::string> stringValues() const override { return {imageName}; }
  [[nodiscard]] std::vector<std::string> stringValueNames() const override { return {"imageName"}; }
  [[nodiscard]] bool hasStringValues() const override { return true; }
};
