#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/CommonEvent.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/CommonEvent.hpp"

using namespace std::string_view_literals;

class Dialog_CommonEvent final : public IEventDialogController {
public:
  Dialog_CommonEvent() = delete;
  explicit Dialog_CommonEvent(const std::string& name, const std::shared_ptr<CommonEventCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new CommonEventCommand());
    }
    eventId = command->event;
    m_picker = ObjectPicker("Common Events"sv, Database::instance()->commonEvents.events(), eventId);
    m_picker->setOpen(true);
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