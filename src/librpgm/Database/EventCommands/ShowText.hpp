#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextTextCommand final : IEventCommand {
  NextTextCommand() = default;
  NextTextCommand(const std::optional<int>& _indent, nlohmann::json& parameters) : IEventCommand(_indent, parameters) {
    parameters[0].get_to(text);
  }

  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }
  void serializeParameters(nlohmann::json& out) override { out.push_back(text); }

  std::string text;
};

struct ShowTextCommand final : IEventCommand {
  ShowTextCommand(const std::optional<int>& _indent, nlohmann::json& parameters) : IEventCommand(_indent, parameters) {
    parameters[0].get_to(faceImage);
    parameters[1].get_to(faceIndex);
    parameters[2].get_to(background);
    parameters[3].get_to(position);
  }
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(faceImage);
    out.push_back(faceIndex);
    out.push_back(background);
    out.push_back(position);
  }

  void addText(NextTextCommand* nextText) { text.emplace_back(nextText); }

  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;
};
