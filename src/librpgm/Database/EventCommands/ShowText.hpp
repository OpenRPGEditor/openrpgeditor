#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextTextCommand final : IEventCommand {
  NextTextCommand() = default;
  NextTextCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }
  void serializeParameters(nlohmann::ordered_json& out) const override;

  std::string text;
};

struct ShowTextCommand final : IEventCommand {
  ShowTextCommand() = default;
  ShowTextCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addText(NextTextCommand* nextText) { text.emplace_back(nextText); }

  std::string faceImage;
  int faceIndex{0};
  TextBackground background = TextBackground::Window;
  TextWindowPosition position = TextWindowPosition::Bottom;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;
};
