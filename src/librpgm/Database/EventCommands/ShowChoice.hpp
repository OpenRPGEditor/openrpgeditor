#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ShowChoiceCommand final : IEventCommand {
  ShowChoiceCommand() = default;
  explicit ShowChoiceCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Choices; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowChoiceCommand>(*this); }

  TextBackground background = TextBackground::Window;
  ChoiceWindowPosition positionType = ChoiceWindowPosition::Right;
  std::vector<std::string> choices{"Yes", "No"};
  int cancelType{1};  // < 0 == disallow/branch
  int defaultType{0}; // -1 is none
};

struct WhenSelectedCommand final : IEventCommand {
  WhenSelectedCommand() = default;
  explicit WhenSelectedCommand(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters);
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<WhenSelectedCommand>(*this); }

  int param1;
  std::string choice;
};

struct WhenCancelCommand final : IEventCommand {
  WhenCancelCommand() = default;
  explicit WhenCancelCommand(const std::optional<int>& _indent, const nlohmann::json& parameters) : IEventCommand(_indent, parameters) {}
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<WhenCancelCommand>(*this); }
};

struct ShowChoicesEndCommand final : IEventCommand {
  ShowChoicesEndCommand() = default;
  explicit ShowChoicesEndCommand(const std::optional<int>& _indent, const nlohmann::json& parameters) : IEventCommand(_indent, parameters) {}
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowChoicesEndCommand>(*this); }
};