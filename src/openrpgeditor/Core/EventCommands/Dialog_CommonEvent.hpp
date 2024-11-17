#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/EventCommands/CommonEvent.hpp"
#include "Database/Database.hpp"

using namespace std::string_view_literals;

struct Dialog_CommonEvent : IEventDialogController {
  Dialog_CommonEvent() = delete;
  explicit Dialog_CommonEvent(const std::string& name,
                              const std::shared_ptr<CommonEventCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new CommonEventCommand());
    }
    eventId = command->event;
    m_picker = ObjectPicker("Common Events"sv, Database::instance().commonEvents.events(), 0);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int eventId;
  bool m_confirmed{false};
  std::shared_ptr<CommonEventCommand> command;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> m_picker;
  std::tuple<bool, bool> result;
};