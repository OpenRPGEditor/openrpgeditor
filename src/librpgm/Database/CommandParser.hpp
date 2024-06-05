#pragma once
#include "Database/Globals.hpp"

#include "nlohmann/json.hpp"

#include <memory>


struct IEventCommand {
  int indent;
  virtual ~IEventCommand() = default;
  [[nodiscard]] virtual EventCode code() const=0;
};

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
};

struct NextTextCommand : IEventCommand {
  ~NextTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Next_Text; }

  std::string text;
};


struct ShowTextCommand : IEventCommand {
  ~ShowTextCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  std::string faceImage;
  int faceIndex;
  TextBackground background;
  TextWindowPosition position;

  std::vector<std::shared_ptr<NextTextCommand>> text;
};

struct ShowChoiceCommand : IEventCommand {
  ~ShowChoiceCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Text; }
  TextBackground background;
  ChoiceWindowPosition positionType;
  std::vector<std::string> choices;
  int cancelType;
  int defaultType;
};

struct WhenSelectedCommand : IEventCommand {
  ~WhenSelectedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Selected; }

  int param1;
  std::string choice;
};

struct WhenCancelCommand : IEventCommand {
  ~WhenCancelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::When_Cancel; }
};


struct PlaySECommand : IEventCommand {
  ~PlaySECommand() override=default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
};

struct CommandParser {
  nlohmann::json parser;
  int index = 0;

  std::vector<std::shared_ptr<IEventCommand>> parse(const nlohmann::json& data);
  void serialize(nlohmann::json& data, const std::vector<std::shared_ptr<IEventCommand>>& list);

  EventCode nextEventCommand() {
    EventCode ret;
    parser[index + 1].at("code").get_to(ret);
    return ret;
  }

  nlohmann::json currentCommand() {
    return parser[index];
  }
};