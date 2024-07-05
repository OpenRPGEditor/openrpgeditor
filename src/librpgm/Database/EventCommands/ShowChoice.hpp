#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ShowChoiceCommand final : IEventCommand {
  ShowChoiceCommand() = default;
  explicit ShowChoiceCommand(const std::optional<int>& _indent, nlohmann::json& parameters)
  : IEventCommand(_indent, parameters) {
    parameters[0].get_to(choices);
    parameters[1].get_to(cancelType);
    if (parameters.size() > 2) {
      parameters[2].get_to(defaultType);
    } else {
      defaultType = 0;
    }
    if (parameters.size() > 3) {
      parameters[3].get_to(positionType);
    } else {
      positionType = ChoiceWindowPosition::Right;
    }

    if (parameters.size() > 4) {
      parameters[4].get_to(background);
    } else {
      background = TextBackground::Window;
    }

    if (cancelType >= choices.size()) {
      cancelType = -2;
    }
  }
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Choices; }
  TextBackground background{};
  ChoiceWindowPosition positionType{};
  std::vector<std::string> choices;
  int cancelType{};  // < 0 == disallow/branch
  int defaultType{}; // -1 is none

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct WhenSelectedCommand final : IEventCommand {
  WhenSelectedCommand() = default;
  explicit WhenSelectedCommand(const std::optional<int>& _indent, nlohmann::json& parameters)
  : IEventCommand(_indent, parameters) {
    parameters[0].get_to(param1);
    parameters[1].get_to(choice);
  }
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct WhenCancelCommand final : IEventCommand {
  WhenCancelCommand() = default;
  explicit WhenCancelCommand(const std::optional<int>& _indent, nlohmann::json& parameters)
  : IEventCommand(_indent, parameters) {}

  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct ShowChoicesEndCommand final : IEventCommand {
  ShowChoicesEndCommand() = default;
  explicit ShowChoicesEndCommand(const std::optional<int>& _indent, nlohmann::json& parameters)
  : IEventCommand(_indent, parameters) {}
  ~ShowChoicesEndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_ShowChoices; }
};