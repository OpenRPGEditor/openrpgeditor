#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextTextCommand : IEventCommand {
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }
  void serializeParameters(nlohmann::json& out) override {
    out.push_back(text);
  }

  std::string text;
};

struct ShowTextCommand : IEventCommand {
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(faceImage);
    out.push_back(faceIndex);
    out.push_back(background);
    out.push_back(position);
  }

  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;
};
