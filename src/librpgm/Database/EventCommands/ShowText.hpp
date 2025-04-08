#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NextTextCommand final : IEventCommand {
  NextTextCommand() = default;
  NextTextCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<NextTextCommand>(*this); }

  std::string text;
};

struct ShowTextCommand final : IEventCommand {
  ShowTextCommand() = default;
  ShowTextCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowTextCommand>(*this); }

  void addText(NextTextCommand* nextText) { text.emplace_back(nextText); }

  std::string faceImage;
  int faceIndex{0};
  TextBackground background = TextBackground::Window;
  TextWindowPosition position = TextWindowPosition::Bottom;
  std::string textLine;
  std::vector<std::shared_ptr<NextTextCommand>> text;

  bool hasStringReference(const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      for (auto& cmd : text) {
        if (cmd->text.contains(str)) {
          return true;
        }
      }
    }
    return false;
  }
  bool setStringReference(const std::string& replaceStr, const std::string& str, SearchType type) override {
    if (type == SearchType::Text) {
      for (auto& cmd : text) {
        if (cmd->text.contains(replaceStr)) {
          size_t pos = cmd->text.find(replaceStr);
          if (pos != std::string::npos) {
            cmd->text.replace(pos, replaceStr.size(), str);
          }
        }
      }
    }
    return true;
  }
};
